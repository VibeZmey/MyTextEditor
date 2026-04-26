#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <stdexcept>

class FileException : public std::runtime_error {
public:
    explicit FileException(const std::string& message) : std::runtime_error(message) {}
};

class FileManager {
public:
    FileManager();

    std::string openFile(const std::string& filepath);
    void saveFile(const std::string& filepath, const std::string& content);
    bool fileExists(const std::string& filepath) const;
    std::string getFileExtension(const std::string& filepath) const;
    std::string getFileName(const std::string& filepath) const;
    unsigned long getFileSize(const std::string& filepath) const;
    bool isTextFile(const std::string& filepath) const;
};

#endif // FILEMANAGER_H
