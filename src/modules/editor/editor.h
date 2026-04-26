#ifndef EDITOR_H
#define EDITOR_H

#include <string>
#include <vector>

class Editor {
private:
    std::string content;
    int cursorPosition;

public:
    Editor();

    void insertText(const std::string& text, int position);
    void deleteText(int position, int length);
    std::string getText() const;
    void setText(const std::string& text);

    void setCursorPosition(int pos);
    int getCursorPosition() const;

    std::vector<int> findText(const std::string& pattern);

    int getLength() const;
    std::string getLine(int lineNumber) const;
};

#endif // EDITOR_H
