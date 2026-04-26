#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <string>
#include <vector>
#include <map>
#include <memory>

class SyntaxHighlighter {
public:
    struct TokenInfo {
        int start;
        int length;
        std::string type; 
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

class CppHighlighter : public SyntaxHighlighter {
public:
    CppHighlighter();
    std::vector<TokenInfo> highlight(const std::string& text) override;
    std::string getLanguageName() const override;
};

class PythonHighlighter : public SyntaxHighlighter {
public:
    PythonHighlighter();
    std::vector<TokenInfo> highlight(const std::string& text) override;
    std::string getLanguageName() const override;
};

class JavaHighlighter : public SyntaxHighlighter {
public:
    JavaHighlighter();
    std::vector<TokenInfo> highlight(const std::string& text) override;
    std::string getLanguageName() const override;
};

class HighlighterFactory {
public:
    static std::unique_ptr<SyntaxHighlighter> createHighlighter(const std::string& language);
    static std::vector<std::string> getSupportedLanguages();
};

#endif // SYNTAXHIGHLIGHTER_H
