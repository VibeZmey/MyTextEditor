#include "commandhistory.h"

CommandHistory::CommandHistory() {}

void CommandHistory::executeCommand(std::shared_ptr<Command> command) {
    if (command) {
        command->execute();
        undoStack.push(command);

        while (!redoStack.empty()) {
            redoStack.pop();
        }

        if ((int)undoStack.size() > MAX_HISTORY_SIZE) {
            std::stack<std::shared_ptr<Command>> tempStack;
            int count = 0;
            while (!undoStack.empty() && count < MAX_HISTORY_SIZE - 1) {
                tempStack.push(undoStack.top());
                undoStack.pop();
                count++;
            }
            undoStack = tempStack;
        }
    }
}

void CommandHistory::undo() {
    if (!undoStack.empty()) {
        auto command = undoStack.top();
        undoStack.pop();
        command->undo();
        redoStack.push(command);
    }
}

void CommandHistory::redo() {
    if (!redoStack.empty()) {
        auto command = redoStack.top();
        redoStack.pop();
        command->execute();
        undoStack.push(command);
    }
}

bool CommandHistory::canUndo() const {
    return !undoStack.empty();
}

bool CommandHistory::canRedo() const {
    return !redoStack.empty();
}

void CommandHistory::clear() {
    while (!undoStack.empty()) undoStack.pop();
    while (!redoStack.empty()) redoStack.pop();
}
