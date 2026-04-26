#ifndef COMMAND_H
#define COMMAND_H

#include <string>

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class InsertTextCommand : public Command {
private:
    class Editor* editor;
    std::string text;
    int position;

public:
    InsertTextCommand(class Editor* ed, const std::string& txt, int pos);
    void execute() override;
    void undo() override;
};

class DeleteTextCommand : public Command {
private:
    class Editor* editor;
    std::string deletedText;
    int position;

public:
    DeleteTextCommand(class Editor* ed, int pos, int length);
    void execute() override;
    void undo() override;
};

#endif // COMMAND_H
