#include "syntaxhighlighter.h"
#include <regex>
#include <cctype>

CppHighlighter::CppHighlighter() {
    keywords = {
        "int", "float", "double", "char", "bool", "void", "unsigned", "signed",
        "if", "else", "else if", "switch", "case", "default", "break", "continue",
        "while", "for", "do", "return", "goto",
        "class", "struct", "union", "enum", "namespace", "template",
        "public", "private", "protected", "virtual", "const", "static",
        "volatile", "mutable", "extern", "inline", "auto", "register",
        "new", "delete", "this", "nullptr", "true", "false",
        "try", "catch", "throw", "typename"
    };

    colorMap["keyword"] = "#0000FF";    
    colorMap["comment"] = "#008000";   
    colorMap["string"] = "#FF0000";    
    colorMap["number"] = "#FF6600";      
}

std::vector<SyntaxHighlighter::TokenInfo> CppHighlighter::highlight(const std::string& text) {
    std::vector<TokenInfo> tokens;

    size_t i = 0;
    while (i < text.length()) {
        if (std::isspace(text[i])) {
            i++;
            continue;
        }

        if (i + 1 < text.length() && text[i] == '/' && text[i + 1] == '/') {
            size_t end = text.find('\n', i);
            if (end == std::string::npos) end = text.length();
            tokens.push_back({static_cast<int>(i), static_cast<int>(end - i), "comment", colorMap["comment"]});
            i = end;
            continue;
        }

        if (i + 1 < text.length() && text[i] == '/' && text[i + 1] == '*') {
            size_t end = text.find("*/", i);
            if (end == std::string::npos) end = text.length();
            else end += 2;
            tokens.push_back({static_cast<int>(i), static_cast<int>(end - i), "comment", colorMap["comment"]});
            i = end;
            continue;
        }

        if (text[i] == '"') {
            size_t start = i;
            i++;
            while (i < text.length() && text[i] != '"') {
                if (text[i] == '\\') i++;
                i++;
            }
            if (i < text.length()) i++;
            tokens.push_back({static_cast<int>(start), static_cast<int>(i - start), "string", colorMap["string"]});
            continue;
        }

        if (text[i] == '\'') {
            size_t start = i;
            i++;
            while (i < text.length() && text[i] != '\'') {
                if (text[i] == '\\') i++;
                i++;
            }
            if (i < text.length()) i++;
            tokens.push_back({static_cast<int>(start), static_cast<int>(i - start), "string", colorMap["string"]});
            continue;
        }

        if (std::isdigit(text[i])) {
            size_t start = i;
            while (i < text.length() && (std::isdigit(text[i]) || text[i] == '.')) {
                i++;
            }
            tokens.push_back({static_cast<int>(start), static_cast<int>(i - start), "number", colorMap["number"]});
            continue;
        }

        if (std::isalpha(text[i]) || text[i] == '_') {
            size_t start = i;
            while (i < text.length() && (std::isalnum(text[i]) || text[i] == '_')) {
                i++;
            }
            std::string word = text.substr(start, i - start);

            bool isKeyword = false;
            for (const auto& kw : keywords) {
                if (word == kw) {
                    isKeyword = true;
                    break;
                }
            }

            if (isKeyword) {
                tokens.push_back({static_cast<int>(start), static_cast<int>(i - start), "keyword", colorMap["keyword"]});
            }
            continue;
        }

        i++;
    }

    return tokens;
}

std::string CppHighlighter::getLanguageName() const {
    return "C++";
}

PythonHighlighter::PythonHighlighter() {
    keywords = {
        "def", "class", "if", "else", "elif", "for", "while", "return",
        "import", "from", "as", "try", "except", "finally", "with",
        "pass", "break", "continue", "raise", "yield", "lambda",
        "True", "False", "None", "and", "or", "not", "in", "is",
        "assert", "del", "global", "nonlocal"
    };

    colorMap["keyword"] = "#FF7700";   
    colorMap["comment"] = "#008000";   
    colorMap["string"] = "#FF0000";    
    colorMap["number"] = "#FF6600";   
}

std::vector<SyntaxHighlighter::TokenInfo> PythonHighlighter::highlight(const std::string& text) {
    std::vector<TokenInfo> tokens;

    size_t i = 0;
    while (i < text.length()) {
        if (std::isspace(text[i])) {
            i++;
            continue;
        }

        if (text[i] == '#') {
            size_t end = text.find('\n', i);
            if (end == std::string::npos) end = text.length();
            tokens.push_back({static_cast<int>(i), static_cast<int>(end - i), "comment", colorMap["comment"]});
            i = end;
            continue;
        }

        if (i + 2 < text.length() && text.substr(i, 3) == "\"\"\"") {
            size_t start = i;
            i += 3;
            size_t end = text.find("\"\"\"", i);
            if (end == std::string::npos) end = text.length();
            else end += 3;
            tokens.push_back({static_cast<int>(start), static_cast<int>(end - start), "string", colorMap["string"]});
            i = end;
            continue;
        }

        if (text[i] == '"') {
            size_t start = i;
            i++;
            while (i < text.length() && text[i] != '"') {
                if (text[i] == '\\') i++;
                i++;
            }
            if (i < text.length()) i++;
            tokens.push_back({static_cast<int>(start), static_cast<int>(i - start), "string", colorMap["string"]});
            continue;
        }

        if (std::isdigit(text[i])) {
            size_t start = i;
            while (i < text.length() && (std::isdigit(text[i]) || text[i] == '.')) {
                i++;
            }
            tokens.push_back({static_cast<int>(start), static_cast<int>(i - start), "number", colorMap["number"]});
            continue;
        }

        if (std::isalpha(text[i]) || text[i] == '_') {
            size_t start = i;
            while (i < text.length() && (std::isalnum(text[i]) || text[i] == '_')) {
                i++;
            }
            std::string word = text.substr(start, i - start);

            bool isKeyword = false;
            for (const auto& kw : keywords) {
                if (word == kw) {
                    isKeyword = true;
                    break;
                }
            }

            if (isKeyword) {
                tokens.push_back({static_cast<int>(start), static_cast<int>(i - start), "keyword", colorMap["keyword"]});
            }
            continue;
        }

        i++;
    }

    return tokens;
}

std::string PythonHighlighter::getLanguageName() const {
    return "Python";
}

JavaHighlighter::JavaHighlighter() {
    keywords = {
        "public", "private", "protected", "static", "final", "abstract",
        "int", "float", "double", "boolean", "char", "void", "long", "byte", "short",
        "if", "else", "switch", "case", "default", "break", "continue",
        "while", "for", "do", "return",
        "class", "interface", "extends", "implements", "new", "this", "super",
        "try", "catch", "finally", "throw", "throws",
        "true", "false", "null", "instanceof"
    };

    colorMap["keyword"] = "#7F0055";     
    colorMap["comment"] = "#3F7F5F";   
    colorMap["string"] = "#2A00FF";   
    colorMap["number"] = "#00AA00";    
}

std::vector<SyntaxHighlighter::TokenInfo> JavaHighlighter::highlight(const std::string& text) {
    std::vector<TokenInfo> tokens;

    size_t i = 0;
    while (i < text.length()) {
        if (std::isspace(text[i])) {
            i++;
            continue;
        }

        if (i + 1 < text.length() && text[i] == '/' && text[i + 1] == '/') {
            size_t end = text.find('\n', i);
            if (end == std::string::npos) end = text.length();
            tokens.push_back({static_cast<int>(i), static_cast<int>(end - i), "comment", colorMap["comment"]});
            i = end;
            continue;
        }

        if (i + 1 < text.length() && text[i] == '/' && text[i + 1] == '*') {
            size_t end = text.find("*/", i);
            if (end == std::string::npos) end = text.length();
            else end += 2;
            tokens.push_back({static_cast<int>(i), static_cast<int>(end - i), "comment", colorMap["comment"]});
            i = end;
            continue;
        }

        if (text[i] == '"') {
            size_t start = i;
            i++;
            while (i < text.length() && text[i] != '"') {
                if (text[i] == '\\') i++;
                i++;
            }
            if (i < text.length()) i++;
            tokens.push_back({static_cast<int>(start), static_cast<int>(i - start), "string", colorMap["string"]});
            continue;
        }

        if (std::isdigit(text[i])) {
            size_t start = i;
            while (i < text.length() && (std::isdigit(text[i]) || text[i] == '.')) {
                i++;
            }
            tokens.push_back({static_cast<int>(start), static_cast<int>(i - start), "number", colorMap["number"]});
            continue;
        }

        if (std::isalpha(text[i]) || text[i] == '_') {
            size_t start = i;
            while (i < text.length() && (std::isalnum(text[i]) || text[i] == '_')) {
                i++;
            }
            std::string word = text.substr(start, i - start);

            bool isKeyword = false;
            for (const auto& kw : keywords) {
                if (word == kw) {
                    isKeyword = true;
                    break;
                }
            }

            if (isKeyword) {
                tokens.push_back({static_cast<int>(start), static_cast<int>(i - start), "keyword", colorMap["keyword"]});
            }
            continue;
        }

        i++;
    }

    return tokens;
}

std::string JavaHighlighter::getLanguageName() const {
    return "Java";
}

std::unique_ptr<SyntaxHighlighter> HighlighterFactory::createHighlighter(const std::string& language) {
    std::string lang = language;

    for (auto& c : lang) c = std::tolower(c);

    if (lang == "cpp" || lang == "cc" || lang == "cxx" || lang == "c++") {
        return std::make_unique<CppHighlighter>();
    } else if (lang == "py" || lang == "python") {
        return std::make_unique<PythonHighlighter>();
    } else if (lang == "java") {
        return std::make_unique<JavaHighlighter>();
    }

    return std::make_unique<CppHighlighter>();
}

std::vector<std::string> HighlighterFactory::getSupportedLanguages() {
    return {"C++", "Python", "Java"};
}
