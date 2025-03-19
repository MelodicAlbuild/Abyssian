#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum class TokenType {
    Identifier,
    Number,
    String,
    Keyword,
    Symbol,
    Operator,
    Semicolon,
    EndOfFile
};

struct Token {
    TokenType type;
    std::string value;
    int line; // Line number for error reporting
};

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t currentPosition;
    char currentChar;
    int currentLine; // Current line number

    void advance();
    void skipWhitespace();
    void skipComment();
    Token identifier();
    Token number();
    Token string();
    Token symbol_or_operator();
    Token nextToken();
    char peek() const;
};

#endif // LEXER_H