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
    std::unique_ptr<ASTNode> parseFunctionCall(const std::string& identifier);
    std::unique_ptr<ASTNode> parsePrintStatement();
    std::unique_ptr<ASTNode> parseFunctionDefinition();
    std::unique_ptr<ASTNode> parseReturnStatement();
    std::unique_ptr<ASTNode> parseForeachLoop();
    std::unique_ptr<ASTNode> parseEventListener();
    std::unique_ptr<ASTNode> parseNPCAction();
    std::unique_ptr<ASTNode> parseForLoop();
    std::unique_ptr<ASTNode> parseWhileLoop();
    std::unique_ptr<ASTNode> parseInputStatement();
    std::unique_ptr<ASTNode> parseExpression();

    std::unique_ptr<ASTNode> parseTerm();

    std::unique_ptr<ASTNode> parseFactor();

    std::unique_ptr<ASTNode> parsePrimary();
};

#endif