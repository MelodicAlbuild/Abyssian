#include "parser.h"
#include <stdexcept>
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), currentPosition(0) {
    currentToken = tokens[currentPosition];
    std::cout << "Initializing parser with first token: " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
}

void Parser::advance() {
    currentPosition++;
    if (currentPosition < tokens.size()) {
        currentToken = tokens[currentPosition];
        std::cout << "Advanced to token: " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
    } else {
        currentToken = {TokenType::EndOfFile, "", currentToken.line};
    }
}

std::unique_ptr<ASTNode> Parser::parse() {
    auto block = std::make_unique<BlockNode>();
    while (currentToken.type != TokenType::EndOfFile) {
        block->statements.push_back(parseStatement());
        if (currentToken.type == TokenType::Semicolon) {
            advance();
        }
    }
    return block;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    std::cout << "Parsing statement starting with token: " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;

    if (currentToken.type == TokenType::Identifier) {
        return parseAssignmentOrFunctionCall();
    } else if (currentToken.type == TokenType::Keyword) {
        if (currentToken.value == "print") {
            return parsePrintStatement();
        } else if (currentToken.value == "fun") {
            return parseFunctionDefinition();
        } else if (currentToken.value == "return") {
            return parseReturnStatement();
        } else {
            std::cerr << "Unknown keyword: " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
            throw std::runtime_error("Unknown keyword at line " + std::to_string(currentToken.line));
        }
    } else if (currentToken.type == TokenType::Symbol) {
        return parseExpression();
    } else {
        std::cerr << "Unknown statement type: " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Unknown statement type at line " + std::to_string(currentToken.line));
    }
}

std::unique_ptr<ASTNode> Parser::parseAssignmentOrFunctionCall() {
    std::string identifier = currentToken.value;
    advance();

    if (currentToken.type == TokenType::Operator && currentToken.value == "=") {
        advance();
        auto expression = parseExpression();
        if (currentToken.type == TokenType::Semicolon) {
            advance();
        }
        return std::make_unique<AssignmentNode>(identifier, std::move(expression));
    } else if (currentToken.type == TokenType::Symbol && currentToken.value == "(") {
        // Function call
        auto call = std::make_unique<FunctionCallNode>(identifier);
        advance();  // Skip '('
        while (currentToken.type != TokenType::Symbol || currentToken.value != ")") {
            call->arguments.push_back(parseExpression());
            if (currentToken.type == TokenType::Symbol && currentToken.value == ",") {
                advance();  // Skip ','
            }
        }
        advance();  // Skip ')'
        if (currentToken.type == TokenType::Semicolon) {
            advance();
        }
        return call;
    } else {
        std::cerr << "Invalid assignment or function call statement: " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Invalid assignment or function call statement at line " + std::to_string(currentToken.line));
    }
}

std::unique_ptr<ASTNode> Parser::parsePrintStatement() {
    advance();  // Skip 'print'
    auto expression = parseExpression();
    if (currentToken.type == TokenType::Semicolon) {
        advance();
    }
    return std::make_unique<PrintNode>(std::move(expression));
}

std::unique_ptr<ASTNode> Parser::parseFunctionDefinition() {
    advance();  // Skip 'fun'
    if (currentToken.type != TokenType::Identifier) {
        std::cerr << "Expected function name after 'fun', got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected function name after 'fun' at line " + std::to_string(currentToken.line));
    }
    std::string functionName = currentToken.value;
    advance();  // Skip function name

    // Expect a left parenthesis '('
    if (currentToken.type != TokenType::Symbol || currentToken.value != "(") {
        std::cerr << "Expected '(' after function name, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected '(' after function name at line " + std::to_string(currentToken.line));
    }
    advance();  // Skip '('

    // Parse function parameters (identifiers separated by commas)
    std::vector<std::string> parameters;
    while (currentToken.type == TokenType::Identifier) {
        parameters.push_back(currentToken.value);
        advance();  // Skip parameter name
        if (currentToken.type == TokenType::Symbol && currentToken.value == ",") {
            advance();  // Skip comma
        } else {
            break;
        }
    }

    // Expect a right parenthesis ')'
    if (currentToken.type != TokenType::Symbol || currentToken.value != ")") {
        std::cerr << "Expected ')' after function parameters, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected ')' after function parameters at line " + std::to_string(currentToken.line));
    }
    advance();  // Skip ')'

    // Expect a block of statements (enclosed in curly braces '{}')
    if (currentToken.type != TokenType::Symbol || currentToken.value != "{") {
        std::cerr << "Expected '{' to start function body, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected '{' to start function body at line " + std::to_string(currentToken.line));
    }
    advance();  // Skip '{'

    auto body = std::make_unique<BlockNode>();
    while (currentToken.type != TokenType::Symbol || currentToken.value != "}") {
        body->statements.push_back(parseStatement());
        if (currentToken.type == TokenType::Semicolon) {
            advance();
        }
    }

    // Expect a right curly brace '}'
    if (currentToken.type != TokenType::Symbol || currentToken.value != "}") {
        std::cerr << "Expected '}' to end function body, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected '}' to end function body at line " + std::to_string(currentToken.line));
    }
    advance();  // Skip '}'

    return std::make_unique<FunctionDeclarationNode>(functionName, std::move(parameters), std::move(body));
}

std::unique_ptr<ASTNode> Parser::parseReturnStatement() {
    advance();  // Skip 'return'
    auto expression = parseExpression();
    if (currentToken.type == TokenType::Semicolon) {
        advance();
    }
    return std::make_unique<ReturnNode>(std::move(expression));
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    auto lhs = parsePrimary();
    while (currentToken.type == TokenType::Symbol && (currentToken.value == "+" || currentToken.value == "-" || currentToken.value == "*" || currentToken.value == "/")) {
        std::string op = currentToken.value;
        advance();  // Skip operator
        auto rhs = parsePrimary();
        lhs = std::make_unique<BinaryExpressionNode>(std::move(lhs), op, std::move(rhs));
    }
    return lhs;
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    std::cout << "Parsing primary expression starting with token: " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
    if (currentToken.type == TokenType::Identifier) {
        std::string identifier = currentToken.value;
        advance();
        return std::make_unique<IdentifierNode>(identifier);
    } else if (currentToken.type == TokenType::Number) {
        double value = std::stod(currentToken.value);
        advance();
        return std::make_unique<NumberNode>(value);
    } else if (currentToken.type == TokenType::String) {
        std::string value = currentToken.value;
        advance();
        return std::make_unique<StringNode>(value);
    } else {
        std::cerr << "Unknown primary expression type: " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Unknown primary expression type at line " + std::to_string(currentToken.line));
    }
}