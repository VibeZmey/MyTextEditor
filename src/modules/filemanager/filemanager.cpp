#include "filemanager.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

FileManager::FileManager() {}

std::string FileManager::openFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw FileException("Cannot open file: " + filepath);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    return buffer.str();
}

void FileManager::saveFile(const std::string& filepath, const std::string& content) {
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        throw FileException("Cannot save file: " + filepath);
    }
    
    file << content;
    file.close();
}

bool FileManager::fileExists(const std::string& filepath) const {
    std::ifstream file(filepath);
    return file.good();
}

std::string FileManager::getFileExtension(const std::string& filepath) const {
    size_t lastDot = filepath.find_last_of('.');
    if (lastDot == std::string::npos) {
        return "";
    }
    std::string ext = filepath.substr(lastDot + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

std::string FileManager::getFileName(const std::string& filepath) const {
    size_t lastSlash = filepath.find_last_of("/\\");
    if (lastSlash == std::string::npos) {
        return filepath;
    }
    return filepath.substr(lastSlash + 1);
}