#ifndef COMMANDHISTORY_H
#define COMMANDHISTORY_H

#include "command.h"
#include <memory>
#include <vector>
#include <stack>

class CommandHistory {
private:
    std::stack<std::shared_ptr<Command>> undoStack;
    std::stack<std::shared_ptr<Command>> redoStack;

    static constexpr int MAX_HISTORY_SIZE = 100;

public:
    CommandHistory();

    void executeCommand(std::shared_ptr<Command> command);
    void undo();
    void redo();

    bool canUndo() const;
    bool canRedo() const;
    void clear();
};

#endif // COMMANDHISTORY_H
