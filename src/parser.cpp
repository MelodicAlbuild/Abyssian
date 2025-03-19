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

    if (currentToken.type == TokenType::Keyword) {
        if (currentToken.value == "print") {
            return parsePrintStatement();
        } else if (currentToken.value == "fun") {
            return parseFunctionDefinition();
        } else if (currentToken.value == "return") {
            return parseReturnStatement();
        } else if (currentToken.value == "foreach") {
            return parseForeachLoop();
        } else if (currentToken.value == "event") {
            return parseEventListener();
        } else if (currentToken.value == "npc") {
            return parseNPCAction();
        } else if (currentToken.value == "for") {
            return parseForLoop();
        } else if (currentToken.value == "while") {
            return parseWhileLoop();
        } else if (currentToken.value == "input") {
            return parseInputStatement();
        } else {
            std::cerr << "Unknown keyword: " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
            throw std::runtime_error("Unknown keyword at line " + std::to_string(currentToken.line));
        }
    } else if (currentToken.type == TokenType::Identifier) {
        return parseAssignmentOrFunctionCall();
    } else if (currentToken.type == TokenType::Symbol && currentToken.value == "(") {
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
        return parseFunctionCall(identifier);
    } else {
        std::cerr << "Invalid assignment or function call statement: " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Invalid assignment or function call statement at line " + std::to_string(currentToken.line));
    }
}

std::unique_ptr<ASTNode> Parser::parseFunctionCall(const std::string& identifier) {
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

std::unique_ptr<ASTNode> Parser::parseForeachLoop() {
    advance();  // Skip 'foreach'
    if (currentToken.type != TokenType::Identifier) {
        std::cerr << "Expected identifier after 'foreach', got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected identifier after 'foreach' at line " + std::to_string(currentToken.line));
    }
    std::string identifier = currentToken.value;
    advance();  // Skip identifier

    if (currentToken.type != TokenType::Keyword || currentToken.value != "in") {
        std::cerr << "Expected 'in' after identifier, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected 'in' after identifier at line " + std::to_string(currentToken.line));
    }
    advance();  // Skip 'in'

    auto collection = parseExpression();

    if (currentToken.type != TokenType::Symbol || currentToken.value != "{") {
        std::cerr << "Expected '{' to start loop body, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected '{' to start loop body at line " + std::to_string(currentToken.line));
    }
    advance();  // Skip '{'

    auto body = std::make_unique<BlockNode>();
    while (currentToken.type != TokenType::Symbol || currentToken.value != "}") {
        body->statements.push_back(parseStatement());
        if (currentToken.type == TokenType::Semicolon) {
            advance();
        }
    }

    if (currentToken.type != TokenType::Symbol || currentToken.value != "}") {
        std::cerr << "Expected '}' to end loop body, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected '}' to end loop body at line " + std::to_string(currentToken.line));
    }
    advance();  // Skip '}'

    return std::make_unique<ForeachLoopNode>(identifier, std::move(collection), std::move(body));
}

std::unique_ptr<ASTNode> Parser::parseEventListener() {
    advance();  // Skip 'event'
    if (currentToken.type != TokenType::Identifier) {
        std::cerr << "Expected event name after 'event', got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected event name after 'event' at line " + std::to_string(currentToken.line));
    }
    std::string event_name = currentToken.value;
    advance();  // Skip event name

    if (currentToken.type != TokenType::Symbol || currentToken.value != "{") {
        std::cerr << "Expected '{' to start event listener body, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected '{' to start event listener body at line " + std::to_string(currentToken.line));
    }
    advance();  // Skip '{'

    auto body = std::make_unique<BlockNode>();
    while (currentToken.type != TokenType::Symbol || currentToken.value != "}") {
        body->statements.push_back(parseStatement());
        if (currentToken.type == TokenType::Semicolon) {
            advance();
        }
    }

    if (currentToken.type != TokenType::Symbol || currentToken.value != "}") {
        std::cerr << "Expected '}' to end event listener body, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected '}' to end event listener body at line " + std::to_string(currentToken.line));
    }
    advance();  // Skip '}'

    return std::make_unique<EventListenerNode>(event_name, std::move(body));
}

std::unique_ptr<ASTNode> Parser::parseNPCAction() {
    advance();  // Skip 'npc'
    if (currentToken.type != TokenType::Identifier) {
        std::cerr << "Expected NPC name after 'npc', got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected NPC name after 'npc' at line " + std::to_string(currentToken.line));
    }
    std::string npc_name = currentToken.value;
    advance();  // Skip NPC name

    if (currentToken.type != TokenType::Identifier) {
        std::cerr << "Expected action after NPC name, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected action after NPC name at line " + std::to_string(currentToken.line));
    }
    std::string action = currentToken.value;
    advance();  // Skip action

    if (currentToken.type == TokenType::Semicolon) {
        advance();
    }

    return std::make_unique<NPCActionNode>(npc_name, action);
}

std::unique_ptr<ASTNode> Parser::parseForLoop() {
    advance();  // Skip 'for'
    if (currentToken.type != TokenType::Identifier) {
        std::cerr << "Expected identifier after 'for', got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected identifier after 'for' at line " + std::to_string(currentToken.line));
    }
    std::string identifier = currentToken.value;
    advance();  // Skip identifier

    if (currentToken.type != TokenType::Operator || currentToken.value != "=") {
        std::cerr << "Expected '=' after identifier, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected '=' after identifier at line " + std::to_string(currentToken.line));
    }
    advance();  // Skip '='

    auto lower_bound = parseExpression();

    if (currentToken.type != TokenType::Keyword || currentToken.value != "to") {
        std::cerr << "Expected 'to' after lower bound, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected 'to' after lower bound at line " + std::to_string(currentToken.line));
    }
    advance();  // Skip 'to'

    auto upper_bound = parseExpression();

    if (currentToken.type != TokenType::Symbol || currentToken.value != "{") {
        std::cerr << "Expected '{' to start loop body, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected '{' to start loop body at line " + std::to_string(currentToken.line));
    }
    advance();  // Skip '{'

    auto body = std::make_unique<BlockNode>();
    while (currentToken.type != TokenType::Symbol || currentToken.value != "}") {
        body->statements.push_back(parseStatement());
        if (currentToken.type == TokenType::Semicolon) {
            advance();
        }
    }

    if (currentToken.type != TokenType::Symbol || currentToken.value != "}") {
        std::cerr << "Expected '}' to end loop body, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected '}' to end loop body at line " + std::to_string(currentToken.line));
    }
    advance();  // Skip '}'

    return std::make_unique<ForLoopNode>(identifier, std::move(lower_bound), std::move(upper_bound), std::move(body));
}

std::unique_ptr<ASTNode> Parser::parseWhileLoop() {
    advance();  // Skip 'while'
    auto condition = parseExpression();  // Parse the condition

    if (currentToken.type != TokenType::Symbol || currentToken.value != "{") {
        std::cerr << "Expected '{' to start loop body, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected '{' to start loop body at line " + std::to_string(currentToken.line));
    }
    advance();  // Skip '{'

    auto body = std::make_unique<BlockNode>();
    while (currentToken.type != TokenType::Symbol || currentToken.value != "}") {
        body->statements.push_back(parseStatement());
        if (currentToken.type == TokenType::Semicolon) {
            advance();
        }
    }

    if (currentToken.type != TokenType::Symbol || currentToken.value != "}") {
        std::cerr << "Expected '}' to end loop body, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected '}' to end loop body at line " + std::to_string(currentToken.line));
    }
    advance();  // Skip '}'

    return std::make_unique<WhileLoopNode>(std::move(condition), std::move(body));
}

std::unique_ptr<ASTNode> Parser::parseInputStatement() {
    advance();  // Skip 'input'
    if (currentToken.type != TokenType::Identifier) {
        std::cerr << "Expected identifier after 'input', got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Expected identifier after 'input' at line " + std::to_string(currentToken.line));
    }
    std::string identifier = currentToken.value;
    advance();  // Skip identifier

    if (currentToken.type == TokenType::Semicolon) {
        advance();
    }

    return std::make_unique<InputNode>(identifier);
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    auto lhs = parseTerm();

    while (currentToken.type == TokenType::Symbol &&
           (currentToken.value == "+" || currentToken.value == "-" ||
            currentToken.value == "<" || currentToken.value == ">" ||
            currentToken.value == "<=" || currentToken.value == ">=" ||
            currentToken.value == "==" || currentToken.value == "!=")) {
        std::string op = currentToken.value;
        advance();  // Skip operator
        auto rhs = parseTerm();
        lhs = std::make_unique<BinaryExpressionNode>(std::move(lhs), op, std::move(rhs));
    }

    return lhs;
}

std::unique_ptr<ASTNode> Parser::parseTerm() {
    auto lhs = parseFactor();

    while (currentToken.type == TokenType::Symbol && (currentToken.value == "*" || currentToken.value == "/")) {
        std::string op = currentToken.value;
        advance();  // Skip operator
        auto rhs = parseFactor();
        lhs = std::make_unique<BinaryExpressionNode>(std::move(lhs), op, std::move(rhs));
    }

    return lhs;
}

std::unique_ptr<ASTNode> Parser::parseFactor() {
    if (currentToken.type == TokenType::Number) {
        auto number = std::make_unique<NumberNode>(std::stod(currentToken.value));
        advance();  // Skip number
        return number;
    } else if (currentToken.type == TokenType::Identifier) {
        std::string identifier = currentToken.value;
        advance();  // Skip identifier
        return std::make_unique<IdentifierNode>(identifier);
    } else if (currentToken.type == TokenType::Symbol && currentToken.value == "(") {
        advance();  // Skip '('
        auto expression = parseExpression();
        if (currentToken.type != TokenType::Symbol || currentToken.value != ")") {
            std::cerr << "Expected ')' after expression, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
            throw std::runtime_error("Expected ')' after expression at line " + std::to_string(currentToken.line));
        }
        advance();  // Skip ')'
        return expression;
    } else {
        std::cerr << "Unexpected token: " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Unexpected token at line " + std::to_string(currentToken.line));
    }
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    std::cout << "Parsing primary expression starting with token: " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
    if (currentToken.type == TokenType::Identifier) {
        std::string identifier = currentToken.value;
        advance();
        if (currentToken.type == TokenType::Symbol && currentToken.value == "(") {
            return parseFunctionCall(identifier);
        }
        return std::make_unique<IdentifierNode>(identifier);
    } else if (currentToken.type == TokenType::Number) {
        double value = std::stod(currentToken.value);
        advance();
        return std::make_unique<NumberNode>(value);
    } else if (currentToken.type == TokenType::String) {
        std::string value = currentToken.value;
        advance();
        return std::make_unique<StringNode>(value);
    } else if (currentToken.type == TokenType::Symbol && currentToken.value == "(") {
        advance();  // Skip '('
        auto expression = parseExpression();
        if (currentToken.type != TokenType::Symbol || currentToken.value != ")") {
            std::cerr << "Expected ')' after expression, got " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
            throw std::runtime_error("Expected ')' after expression at line " + std::to_string(currentToken.line));
        }
        advance();  // Skip ')'
        return expression;
    } else {
        std::cerr << "Unknown primary expression type: " << currentToken.value << " (line " << currentToken.line << ")" << std::endl;
        throw std::runtime_error("Unknown primary expression type at line " + std::to_string(currentToken.line));
    }
}