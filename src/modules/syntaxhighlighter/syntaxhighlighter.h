#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <string>
#include <vector>
#include <map>
#include <memory>

// Паттерн Strategy для разных стратегий подсветки синтаксиса
class SyntaxHighlighter {
public:
    struct TokenInfo {
        int start;
        int length;
        std::string type;  // "keyword", "comment", "string", "number"
        std::string color;
    };

protected:
    std::vector<std::string> keywords;
    std::map<std::string, std::string> colorMap;

public:
    virtual ~SyntaxHighlighter() = default;
    virtual std::vector<TokenInfo> highlight(const std::string& text) = 0;
    virtual std::string getLanguageName() const = 0;
};

// Конкретная реализация для C++
class CppHighlighter : public SyntaxHighlighter {
public:
    CppHighlighter();
    std::vector<TokenInfo> highlight(const std::string& text) override;
    std::string getLanguageName() const override;
};

// Конкретная реализация для Python
class PythonHighlighter : public SyntaxHighlighter {
public:
    PythonHighlighter();
    std::vector<TokenInfo> highlight(const std::string& text) override;
    std::string getLanguageName() const override;
};

// Конкретная реализация для Java
class JavaHighlighter : public SyntaxHighlighter {
public:
    JavaHighlighter();
    std::vector<TokenInfo> highlight(const std::string& text) override;
    std::string getLanguageName() const override;
};

// Фабрика для создания подсветчиков (паттерн Factory Method)
class HighlighterFactory {
public:
    static std::unique_ptr<SyntaxHighlighter> createHighlighter(const std::string& language);
    static std::vector<std::string> getSupportedLanguages();
};

#endif // SYNTAXHIGHLIGHTER_H
