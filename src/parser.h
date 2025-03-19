#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <memory>
#include "ast.h"
#include "lexer.h"

struct Token;

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::unique_ptr<ASTNode> parse();

private:
    std::vector<Token> tokens;
    Token currentToken;
    size_t currentPosition;

    void advance();
    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ASTNode> parseAssignmentOrFunctionCall();
    std::unique_ptr<ASTNode> parsePrintStatement();
    std::unique_ptr<ASTNode> parseFunctionDefinition();
    std::unique_ptr<ASTNode> parseReturnStatement();
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parsePrimary();
    std::unique_ptr<ASTNode> parseBinaryExpression();
};

#endif