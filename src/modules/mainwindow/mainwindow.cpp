#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../command/command.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextBlock>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      editor(std::make_unique<Editor>()),
      history(std::make_unique<CommandHistory>()),
      fileManager(std::make_unique<FileManager>()),
      searchManager(std::make_unique<SearchManager>()),
      currentSearchResultIndex(0) {
    
    ui->setupUi(this);
    ui->textEdit->setFont(QFont("Courier New", 10));
    
    setupLanguageCombo();
    setupConnections();
    
    searchManager->subscribe(this);
    
    setWindowTitle("Text Editor with Syntax Highlighting");
    resize(1000, 700);
    updateStatusBar();
}

MainWindow::~MainWindow() {
    searchManager->unsubscribe(this);
    delete ui;
}

void MainWindow::setupConnections() {
    connect(ui->openBtn, &QPushButton::clicked, this, &MainWindow::onOpen);
    connect(ui->saveBtn, &QPushButton::clicked, this, &MainWindow::onSave);
    connect(ui->saveAsBtn, &QPushButton::clicked, this, &MainWindow::onSaveAs);
    connect(ui->clearBtn, &QPushButton::clicked, this, &MainWindow::onClear);
    
    connect(ui->undoBtn, &QPushButton::clicked, this, &MainWindow::onUndo);
    connect(ui->redoBtn, &QPushButton::clicked, this, &MainWindow::onRedo);
    
    connect(ui->findNextBtn, &QPushButton::clicked, this, &MainWindow::onFindNext);
    connect(ui->findPrevBtn, &QPushButton::clicked, this, &MainWindow::onFindPrev);
    
    connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::onTextChanged);
    connect(ui->textEdit, &QTextEdit::cursorPositionChanged, this, &MainWindow::onCursorPositionChanged);
    
    connect(ui->languageCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onLanguageChanged);
    
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpen);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::onSave);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::onSaveAs);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExit);
    connect(ui->actionUndo, &QAction::triggered, this, &MainWindow::onUndo);
    connect(ui->actionRedo, &QAction::triggered, this, &MainWindow::onRedo);
    connect(ui->actionClear, &QAction::triggered, this, &MainWindow::onClear);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);
}

void MainWindow::setupLanguageCombo() {
    ui->languageCombo->clear();
    ui->languageCombo->addItem("C++");
    ui->languageCombo->addItem("Python");
    ui->languageCombo->setCurrentIndex(0);
}

void MainWindow::onOpen() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open File", "",
        "All Files (*);;C++ Files (*.cpp *.h);;Python Files (*.py);;Text Files (*.txt)");
    
    if (filePath.isEmpty()) return;

    try {
        std::string content = fileManager->openFile(filePath.toStdString());
        ui->textEdit->setPlainText(QString::fromStdString(content));
        currentFilePath = filePath.toStdString();
        
        detectLanguageFromFile();
        ui->fileInfoLabel->setText(QString::fromStdString(fileManager->getFileName(currentFilePath)));
        ui->statusLabel->setText("File opened successfully");
        
        history->clear();
        updateStatusBar();
    } catch (const FileException& e) {
        QMessageBox::critical(this, "Error", QString::fromStdString(e.what()));
        ui->statusLabel->setText("Error opening file");
    }
}

void MainWindow::onSave() {
    if (currentFilePath.empty()) {
        onSaveAs();
        return;
    }
    
    try {
        fileManager->saveFile(currentFilePath, ui->textEdit->toPlainText().toStdString());
        ui->statusLabel->setText("File saved successfully");
    } catch (const FileException& e) {
        QMessageBox::critical(this, "Error", QString::fromStdString(e.what()));
        ui->statusLabel->setText("Error saving file");
    }
}

void MainWindow::onSaveAs() {
    QString filePath = QFileDialog::getSaveFileName(this, "Save File As", "",
        "All Files (*);;C++ Files (*.cpp *.h);;Python Files (*.py);;Text Files (*.txt)");
    
    if (filePath.isEmpty()) return;
    
    currentFilePath = filePath.toStdString();
    onSave();
    detectLanguageFromFile();
    ui->fileInfoLabel->setText(QString::fromStdString(fileManager->getFileName(currentFilePath)));
}

void MainWindow::onExit() {
    close();
}

void MainWindow::onUndo() {
    if (history->canUndo()) {
        history->undo();
        ui->statusLabel->setText("Undo executed");
        updateStatusBar();
    }
}

void MainWindow::onRedo() {
    if (history->canRedo()) {
        history->redo();
        ui->statusLabel->setText("Redo executed");
        updateStatusBar();
    }
}

void MainWindow::onClear() {
    if (QMessageBox::question(this, "Clear", "Clear all text?",
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        ui->textEdit->clear();
        editor->setText("");
        history->clear();
        ui->statusLabel->setText("Cleared");
        updateStatusBar();
    }
}

void MainWindow::onFindNext() {
    std::string query = ui->searchBox->text().toStdString();
    if (query.empty()) return;
    
    if (currentSearchResults.empty()) {
        std::string text = ui->textEdit->toPlainText().toStdString();
        searchManager->search(text, query);
    }
    
    if (!currentSearchResults.empty()) {
        currentSearchResultIndex = (currentSearchResultIndex + 1) % currentSearchResults.size();
        int pos = currentSearchResults[currentSearchResultIndex];
        
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.setPosition(pos);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, query.length());
        ui->textEdit->setTextCursor(cursor);
        
        ui->statusLabel->setText(QString("Found %1 of %2").arg(currentSearchResultIndex + 1)
                                 .arg(currentSearchResults.size()));
    }
}

void MainWindow::onFindPrev() {
    std::string query = ui->searchBox->text().toStdString();
    if (query.empty()) return;
    
    if (currentSearchResults.empty()) {
        std::string text = ui->textEdit->toPlainText().toStdString();
        searchManager->search(text, query);
    }
    
    if (!currentSearchResults.empty()) {
        currentSearchResultIndex = (currentSearchResultIndex - 1 + (int)currentSearchResults.size()) 
                                   % currentSearchResults.size();
        int pos = currentSearchResults[currentSearchResultIndex];
        
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.setPosition(pos);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, query.length());
        ui->textEdit->setTextCursor(cursor);
        
        ui->statusLabel->setText(QString("Found %1 of %2").arg(currentSearchResultIndex + 1)
                                 .arg(currentSearchResults.size()));
    }
}

void MainWindow::onTextChanged() {
    editor->setText(ui->textEdit->toPlainText().toStdString());
    updateStatusBar();
}

void MainWindow::onLanguageChanged(int index) {
    Q_UNUSED(index);
    std::string language = ui->languageCombo->currentText().toStdString();
    highlighter = HighlighterFactory::createHighlighter(language);
}

void MainWindow::onCursorPositionChanged() {
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextBlock block = ui->textEdit->document()->findBlock(cursor.position());
    int line = block.blockNumber() + 1;
    int col = cursor.positionInBlock() + 1;
    ui->cursorLabel->setText(QString("Line: %1, Col: %2").arg(line).arg(col));
}

void MainWindow::onSearchResultsChanged(const std::vector<int>& results) {
    currentSearchResults = results;
    currentSearchResultIndex = 0;
    if (!results.empty()) {
        ui->statusLabel->setText(QString("Found %1 results").arg(results.size()));
    } else {
        ui->statusLabel->setText("No results found");
    }
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "About",
        "Text Editor with Syntax Highlighting v1.0\n\n"
        "© 2026");
}

void MainWindow::detectLanguageFromFile() {
    if (currentFilePath.empty()) return;
    std::string ext = fileManager->getFileExtension(currentFilePath);
    
    int index = 0;
    if (ext == "py") index = 1;
    
    ui->languageCombo->setCurrentIndex(index);
}

void MainWindow::updateStatusBar() {
    ui->undoBtn->setEnabled(history->canUndo());
    ui->redoBtn->setEnabled(history->canRedo());
}