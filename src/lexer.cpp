#include "lexer.h"
#include <cctype>
#include <iostream>

Lexer::Lexer(const std::string& source) : source(source), currentPosition(0), currentLine(1) {
    currentChar = source[currentPosition];
    std::cout << "Initializing lexer with source: " << source << std::endl;
}

void Lexer::advance() {
    if (currentChar == '\n') {
        currentLine++;
    }
    currentPosition++;
    if (currentPosition < source.size()) {
        currentChar = source[currentPosition];
    } else {
        currentChar = '\0';
    }
}

void Lexer::skipWhitespace() {
    while (std::isspace(currentChar)) {
        advance();
    }
}

Token Lexer::identifier() {
    std::string result;
    int line = currentLine;
    while (std::isalnum(currentChar) || currentChar == '_') {
        result += currentChar;
        advance();
    }
    if (result == "print" || result == "fun" || result == "return") {
        std::cout << "Identified keyword: " << result << std::endl;
        return {TokenType::Keyword, result, line};
    }
    std::cout << "Identified identifier: " << result << std::endl;
    return {TokenType::Identifier, result, line};
}

Token Lexer::number() {
    std::string result;
    int line = currentLine;
    while (std::isdigit(currentChar)) {
        result += currentChar;
        advance();
    }
    std::cout << "Identified number: " << result << std::endl;
    return {TokenType::Number, result, line};
}

Token Lexer::string() {
    std::string result;
    int line = currentLine;
    advance(); // Skip the opening quote
    while (currentChar != '"' && currentChar != '\0') {
        result += currentChar;
        advance();
    }
    advance(); // Skip the closing quote
    std::cout << "Identified string: " << result << std::endl;
    return {TokenType::String, result, line};
}

Token Lexer::symbol_or_operator() {
    char sym = currentChar;
    int line = currentLine;
    advance();
    if (sym == '=') {
        std::cout << "Identified operator: =" << std::endl;
        return {TokenType::Operator, "=", line};
    }
    std::cout << "Identified symbol: " << sym << std::endl;
    return {TokenType::Symbol, std::string(1, sym), line};
}

Token Lexer::nextToken() {
    while (currentChar != '\0') {
        if (std::isspace(currentChar)) {
            skipWhitespace();
            continue;
        }

        if (std::isalpha(currentChar) || currentChar == '_') {
            return identifier();
        }

        if (std::isdigit(currentChar)) {
            return number();
        }

        if (currentChar == '"') {
            return string();
        }

        if (currentChar == ';') {
            int line = currentLine;
            advance();
            std::cout << "Identified semicolon" << std::endl;
            return {TokenType::Semicolon, ";", line};
        }

        return symbol_or_operator();
    }

    return {TokenType::EndOfFile, "", currentLine};
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    Token token = nextToken();
    while (token.type != TokenType::EndOfFile) {
        tokens.push_back(token);
        token = nextToken();
    }
    tokens.push_back(token); // Add EndOfFile token
    return tokens;
}