#include "editor.h"
#include <algorithm>

Editor::Editor() : cursorPosition(0) {}

void Editor::insertText(const std::string& text, int position) {
    if (position < 0 || position > (int)content.length()) {
        position = content.length();
    }
    content.insert(position, text);
    cursorPosition = position + text.length();
}

void Editor::deleteText(int position, int length) {
    if (position >= 0 && position + length <= (int)content.length()) {
        content.erase(position, length);
        cursorPosition = position;
    }
}

std::string Editor::getText() const {
    return content;
}

void Editor::setText(const std::string& text) {
    content = text;
    cursorPosition = 0;
}

void Editor::setCursorPosition(int pos) {
    if (pos >= 0 && pos <= (int)content.length()) {
        cursorPosition = pos;
    }
}

int Editor::getCursorPosition() const {
    return cursorPosition;
}

std::vector<int> Editor::findText(const std::string& pattern) {
    std::vector<int> positions;
    size_t pos = 0;
    while ((pos = content.find(pattern, pos)) != std::string::npos) {
        positions.push_back(static_cast<int>(pos));
        pos += pattern.length();
    }
    return positions;
}

int Editor::getLength() const {
    return content.length();
}

std::string Editor::getLine(int lineNumber) const {
    size_t start = 0;
    int currentLine = 0;

    for (size_t i = 0; i < content.length(); ++i) {
        if (currentLine == lineNumber) {
            size_t end = content.find('\n', i);
            if (end == std::string::npos) {
                return content.substr(i);
            }
            return content.substr(i, end - i);
        }
        if (content[i] == '\n') {
            currentLine++;
            start = i + 1;
        }
    }
    return "";
}
