#include "interpreter.h"
#include "parser.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <regex>
#include <iomanip>

void Interpreter::interpret(std::unique_ptr<ASTNode> ast) {
    this->ast = std::unique_ptr<BlockNode>(dynamic_cast<BlockNode*>(ast.release()));
}

std::optional<std::string> Interpreter::execute() {
    std::optional<std::string> return_value;
    if (ast) {
        interpret_block(std::move(ast), return_value);
    } else {
        throw std::runtime_error("No code to execute.");
    }
    return return_value;
}

void Interpreter::interpret_node(std::unique_ptr<ASTNode> node, std::optional<std::string>& return_value) {
    if (auto block = dynamic_cast<BlockNode*>(node.get())) {
        interpret_block(std::unique_ptr<BlockNode>(static_cast<BlockNode*>(node.release())), return_value);
    } else if (auto assignment = dynamic_cast<AssignmentNode*>(node.get())) {
        interpret_assignment(std::unique_ptr<AssignmentNode>(static_cast<AssignmentNode*>(node.release())));
    } else if (auto print = dynamic_cast<PrintNode*>(node.get())) {
        interpret_print(std::unique_ptr<PrintNode>(static_cast<PrintNode*>(node.release())));
    } else if (auto input = dynamic_cast<InputNode*>(node.get())) {
        interpret_input(std::unique_ptr<InputNode>(static_cast<InputNode*>(node.release())));
    } else if (auto function = dynamic_cast<FunctionDeclarationNode*>(node.get())) {
        interpret_function_declaration(std::unique_ptr<FunctionDeclarationNode>(static_cast<FunctionDeclarationNode*>(node.release())));
    } else if (auto for_loop = dynamic_cast<ForLoopNode*>(node.get())) {
        interpret_for_loop(std::unique_ptr<ForLoopNode>(static_cast<ForLoopNode*>(node.release())), return_value);
    } else if (auto while_loop = dynamic_cast<WhileLoopNode*>(node.get())) {
        interpret_while_loop(std::unique_ptr<WhileLoopNode>(static_cast<WhileLoopNode*>(node.release())), return_value);
    } else if (auto foreach_loop = dynamic_cast<ForeachLoopNode*>(node.get())) {
        interpret_foreach_loop(std::unique_ptr<ForeachLoopNode>(static_cast<ForeachLoopNode*>(node.release())), return_value);
    } else if (auto event_listener = dynamic_cast<EventListenerNode*>(node.get())) {
        interpret_event_listener(std::unique_ptr<EventListenerNode>(static_cast<EventListenerNode*>(node.release())));
    } else if (auto npc_action = dynamic_cast<NPCActionNode*>(node.get())) {
        interpret_npc_action(std::unique_ptr<NPCActionNode>(static_cast<NPCActionNode*>(node.release())));
    } else if (auto return_node = dynamic_cast<ReturnNode*>(node.get())) {
        interpret_return(std::unique_ptr<ReturnNode>(static_cast<ReturnNode*>(node.release())), return_value);
    } else if (auto binary_expression = dynamic_cast<BinaryExpressionNode*>(node.get())) {
        return_value = interpret_binary_expression(std::unique_ptr<BinaryExpressionNode>(static_cast<BinaryExpressionNode*>(node.release())));
    } else {
        throw std::runtime_error("Unknown AST node");
    }
}

void Interpreter::interpret_block(std::unique_ptr<BlockNode> block, std::optional<std::string>& return_value) {
    for (const auto& statement : block->statements) {
        interpret_node(statement->clone(), return_value);
        if (return_value.has_value()) {
            break;
        }
    }
}

void Interpreter::interpret_assignment(std::unique_ptr<AssignmentNode> assignment) {
    auto value = evaluate_expression(assignment->expression->clone());
    variables[assignment->identifier] = value;
}

void Interpreter::interpret_print(std::unique_ptr<PrintNode> print) {
    auto value = evaluate_expression(print->expression->clone());

    // Format the output to include decimals only if necessary
    std::stringstream ss(value);
    double number;
    ss >> number;

    if (!ss.fail() && ss.eof()) {
        // Value is a number
        std::stringstream formatted_value;
        formatted_value << std::fixed << std::setprecision(6) << number;
        std::string output = formatted_value.str();
        // Remove trailing zeros and the decimal point if not needed
        output.erase(output.find_last_not_of('0') + 1, std::string::npos);
        if (output.back() == '.') {
            output.pop_back();
        }
        std::cout << output << std::endl;
    } else {
        // Value is not a number
        std::cout << value << std::endl;
    }
}

void Interpreter::interpret_input(std::unique_ptr<InputNode> input) {
    std::string value;
    std::getline(std::cin, value);
    variables[input->identifier] = value;
}

void Interpreter::interpret_function_declaration(std::unique_ptr<FunctionDeclarationNode> function) {
    functions[function->identifier] = std::move(function);
}

void Interpreter::interpret_for_loop(std::unique_ptr<ForLoopNode> for_loop, std::optional<std::string>& return_value) {
    try {
        std::string lower_bound_str = evaluate_expression(for_loop->lower_bound->clone());
        std::string upper_bound_str = evaluate_expression(for_loop->upper_bound->clone());
        std::cout << "For loop bounds: lower=" << lower_bound_str << " upper=" << upper_bound_str << std::endl;
        if (!is_number(lower_bound_str) || !is_number(upper_bound_str)) {
            throw std::invalid_argument("Bounds are not valid numbers");
        }
        int lower_bound = std::stoi(lower_bound_str);
        int upper_bound = std::stoi(upper_bound_str);
        for (int i = lower_bound; i <= upper_bound; ++i) {
            variables[for_loop->identifier] = std::to_string(i);
            interpret_node(for_loop->body->clone(), return_value);
            if (return_value.has_value()) {
                break;
            }
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument in for loop bounds: " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Out of range error in for loop bounds: " << e.what() << std::endl;
    }
}

void Interpreter::interpret_while_loop(std::unique_ptr<WhileLoopNode> while_loop, std::optional<std::string>& return_value) {
    while (evaluate_expression(while_loop->condition->clone()) == "true") {
        interpret_node(while_loop->body->clone(), return_value);
        if (return_value.has_value()) {
            break;
        }
    }
}

void Interpreter::interpret_foreach_loop(std::unique_ptr<ForeachLoopNode> foreach_loop, std::optional<std::string>& return_value) {
    auto collection = evaluate_expression(foreach_loop->collection->clone());
    std::istringstream ss(collection);
    std::string item;
    while (std::getline(ss, item, ',')) {
        variables[foreach_loop->identifier] = item;
        interpret_node(foreach_loop->body->clone(), return_value);
        if (return_value.has_value()) {
            break;
        }
    }
}

void Interpreter::interpret_event_listener(std::unique_ptr<EventListenerNode> event_listener) {
    events[event_listener->event_name].push_back(std::move(event_listener));
}

void Interpreter::interpret_npc_action(std::unique_ptr<NPCActionNode> npc_action) {
    // Execute the NPC action (implementation depends on the game engine)
    std::cout << "Executing NPC action for: " << npc_action->npc_name << std::endl;
}

void Interpreter::interpret_return(std::unique_ptr<ReturnNode> return_node, std::optional<std::string>& return_value) {
    return_value = evaluate_expression(return_node->expression->clone());
}

std::string Interpreter::interpret_binary_expression(std::unique_ptr<BinaryExpressionNode> binary_expression) {
    auto left = evaluate_expression(binary_expression->left->clone());
    auto right = evaluate_expression(binary_expression->right->clone());

    if (is_number(left) && is_number(right)) {
        double left_num = std::stod(left);
        double right_num = std::stod(right);
        if (binary_expression->op == "+") {
            return std::to_string(left_num + right_num);
        } else if (binary_expression->op == "-") {
            return std::to_string(left_num - right_num);
        } else if (binary_expression->op == "*") {
            return std::to_string(left_num * right_num);
        } else if (binary_expression->op == "/") {
            if (right_num == 0) {
                throw std::runtime_error("Division by zero");
            }
            return std::to_string(left_num / right_num);
        } else {
            throw std::runtime_error("Unknown binary operator: " + binary_expression->op);
        }
    } else {
        throw std::runtime_error("Invalid operands for binary operator: " + binary_expression->op);
    }
}

std::string Interpreter::evaluate_expression(std::unique_ptr<ASTNode> node) {
    if (auto identifier = dynamic_cast<IdentifierNode*>(node.get())) {
        return variables[identifier->identifier];
    } else if (auto number = dynamic_cast<NumberNode*>(node.get())) {
        return std::to_string(number->value);
    } else if (auto str = dynamic_cast<StringNode*>(node.get())) {
        return str->value;
    } else if (auto binary_expression = dynamic_cast<BinaryExpressionNode*>(node.get())) {
        return interpret_binary_expression(std::unique_ptr<BinaryExpressionNode>(static_cast<BinaryExpressionNode*>(node.release())));
    }
    throw std::runtime_error("Unknown expression node");
}

bool Interpreter::is_number(const std::string& s) {
    std::regex number_regex("^-?\\d+(\\.\\d+)?$");
    return std::regex_match(s, number_regex);
}