#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <memory>
#include "../editor/editor.h"
#include "../command/commandhistory.h"
#include "../syntaxhighlighter/syntaxhighlighter.h"
#include "../filemanager/filemanager.h"
#include "../searchmanager/searchmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public SearchObserver {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onOpen();
    void onSave();
    void onSaveAs();
    void onExit();
    
    void onUndo();
    void onRedo();
    void onClear();
    
    void onFindNext();
    void onFindPrev();
    
    void onTextChanged();
    void onLanguageChanged(int index);
    void onCursorPositionChanged();
    
    void onSearchResultsChanged(const std::vector<int>& results) override;
    
    void onAbout();

private:
    void setupConnections();
    void setupLanguageCombo();
    void detectLanguageFromFile();
    void updateStatusBar();

    Ui::MainWindow* ui;

    std::unique_ptr<Editor> editor;
    std::unique_ptr<CommandHistory> history;
    std::unique_ptr<FileManager> fileManager;
    std::unique_ptr<SearchManager> searchManager;
    std::unique_ptr<SyntaxHighlighter> highlighter;

    std::string currentFilePath;
    std::vector<int> currentSearchResults;
    int currentSearchResultIndex;
};

#endif // MAINWINDOW_H
