#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>
#include <optional>

class Interpreter {
public:
    void interpret(std::unique_ptr<ASTNode> ast);
    std::optional<std::string> execute(); // Changed return type to std::optional<std::string>

private:
    void interpret_node(std::unique_ptr<ASTNode> node, std::optional<std::string>& return_value);
    void interpret_block(std::unique_ptr<BlockNode> block, std::optional<std::string>& return_value);
    void interpret_assignment(std::unique_ptr<AssignmentNode> assignment);
    void interpret_print(std::unique_ptr<PrintNode> print);
    void interpret_input(std::unique_ptr<InputNode> input);
    void interpret_function_declaration(std::unique_ptr<FunctionDeclarationNode> function);
    void interpret_for_loop(std::unique_ptr<ForLoopNode> for_loop, std::optional<std::string>& return_value);
    void interpret_while_loop(std::unique_ptr<WhileLoopNode> while_loop, std::optional<std::string>& return_value);
    void interpret_foreach_loop(std::unique_ptr<ForeachLoopNode> foreach_loop, std::optional<std::string>& return_value);
    void interpret_event_listener(std::unique_ptr<EventListenerNode> event_listener);
    void interpret_npc_action(std::unique_ptr<NPCActionNode> npc_action);
    void interpret_return(std::unique_ptr<ReturnNode> return_node, std::optional<std::string>& return_value);
    std::string interpret_binary_expression(std::unique_ptr<BinaryExpressionNode> binary_expression);

    std::string evaluate_expression(std::unique_ptr<ASTNode> node);
    bool is_number(const std::string& s);

    std::unordered_map<std::string, std::string> variables;
    std::unordered_map<std::string, std::unique_ptr<ASTNode>> functions;
    std::unordered_map<std::string, std::vector<std::unique_ptr<ASTNode>>> events;
    std::unique_ptr<BlockNode> ast; // Store the AST for execution
};

#endif // INTERPRETER_H