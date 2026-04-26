#include "command.h"
#include "../editor/editor.h"

InsertTextCommand::InsertTextCommand(Editor* ed, const std::string& txt, int pos)
    : editor(ed), text(txt), position(pos) {}

void InsertTextCommand::execute() {
    if (editor) {
        editor->insertText(text, position);
    }
}

void InsertTextCommand::undo() {
    if (editor) {
        editor->deleteText(position, text.length());
    }
}

DeleteTextCommand::DeleteTextCommand(Editor* ed, int pos, int length)
    : editor(ed), position(pos) {
    if (ed) {
        std::string fullText = ed->getText();
        if (pos >= 0 && pos + length <= (int)fullText.length()) {
            deletedText = fullText.substr(pos, length);
        }
    }
}

void DeleteTextCommand::execute() {
    if (editor) {
        editor->deleteText(position, deletedText.length());
    }
}

void DeleteTextCommand::undo() {
    if (editor) {
        editor->insertText(deletedText, position);
    }
}
