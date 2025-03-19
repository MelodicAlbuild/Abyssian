#ifndef AST_H
#define AST_H

#include <memory>
#include <vector>
#include <string>

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual std::unique_ptr<ASTNode> clone() const = 0;
};

class BlockNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    std::unique_ptr<ASTNode> clone() const override {
        auto block = std::make_unique<BlockNode>();
        for (const auto& stmt : statements) {
            block->statements.push_back(stmt->clone());
        }
        return block;
    }
};

class AssignmentNode : public ASTNode {
public:
    std::string identifier;
    std::unique_ptr<ASTNode> expression;

    AssignmentNode(const std::string& id, std::unique_ptr<ASTNode> expr)
        : identifier(id), expression(std::move(expr)) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<AssignmentNode>(identifier, expression->clone());
    }
};

class PrintNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;

    PrintNode(std::unique_ptr<ASTNode> expr)
        : expression(std::move(expr)) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<PrintNode>(expression->clone());
    }
};

class FunctionDeclarationNode : public ASTNode {
public:
    std::string identifier;
    std::vector<std::string> parameters;
    std::unique_ptr<BlockNode> body;

    FunctionDeclarationNode(const std::string& id, std::vector<std::string> params, std::unique_ptr<BlockNode> b)
        : identifier(id), parameters(std::move(params)), body(std::move(b)) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<FunctionDeclarationNode>(identifier, parameters, std::unique_ptr<BlockNode>(static_cast<BlockNode*>(body->clone().release())));
    }
};

class ReturnNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;

    ReturnNode(std::unique_ptr<ASTNode> expr)
        : expression(std::move(expr)) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<ReturnNode>(expression->clone());
    }
};

class BinaryExpressionNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> left;
    std::string op;
    std::unique_ptr<ASTNode> right;

    BinaryExpressionNode(std::unique_ptr<ASTNode> lhs, const std::string& operator_, std::unique_ptr<ASTNode> rhs)
        : left(std::move(lhs)), op(operator_), right(std::move(rhs)) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<BinaryExpressionNode>(left->clone(), op, right->clone());
    }
};

class IdentifierNode : public ASTNode {
public:
    std::string identifier;

    IdentifierNode(const std::string& id)
        : identifier(id) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<IdentifierNode>(identifier);
    }
};

class NumberNode : public ASTNode {
public:
    double value;

    NumberNode(double val)
        : value(val) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<NumberNode>(value);
    }
};

class StringNode : public ASTNode {
public:
    std::string value;

    StringNode(const std::string& val)
        : value(val) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<StringNode>(value);
    }
};

class FunctionCallNode : public ASTNode {
public:
    std::string identifier;
    std::vector<std::unique_ptr<ASTNode>> arguments;

    FunctionCallNode(const std::string& id)
        : identifier(id) {}

    std::unique_ptr<ASTNode> clone() const override {
        auto call = std::make_unique<FunctionCallNode>(identifier);
        for (const auto& arg : arguments) {
            call->arguments.push_back(arg->clone());
        }
        return call;
    }
};

class ForeachLoopNode : public ASTNode {
public:
    std::string identifier;
    std::unique_ptr<ASTNode> collection;
    std::unique_ptr<BlockNode> body;

    ForeachLoopNode(const std::string& id, std::unique_ptr<ASTNode> coll, std::unique_ptr<BlockNode> b)
        : identifier(id), collection(std::move(coll)), body(std::move(b)) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<ForeachLoopNode>(identifier, collection->clone(), std::unique_ptr<BlockNode>(static_cast<BlockNode*>(body->clone().release())));
    }
};

class EventListenerNode : public ASTNode {
public:
    std::string event_name;
    std::unique_ptr<BlockNode> body;

    EventListenerNode(const std::string& event, std::unique_ptr<BlockNode> b)
        : event_name(event), body(std::move(b)) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<EventListenerNode>(event_name, std::unique_ptr<BlockNode>(static_cast<BlockNode*>(body->clone().release())));
    }
};

class NPCActionNode : public ASTNode {
public:
    std::string npc_name;
    std::string action;

    NPCActionNode(const std::string& npc, const std::string& act)
        : npc_name(npc), action(act) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<NPCActionNode>(npc_name, action);
    }
};

class ForLoopNode : public ASTNode {
public:
    std::string identifier;
    std::unique_ptr<ASTNode> lower_bound;
    std::unique_ptr<ASTNode> upper_bound;
    std::unique_ptr<BlockNode> body;

    ForLoopNode(const std::string& id, std::unique_ptr<ASTNode> lower, std::unique_ptr<ASTNode> upper, std::unique_ptr<BlockNode> b)
        : identifier(id), lower_bound(std::move(lower)), upper_bound(std::move(upper)), body(std::move(b)) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<ForLoopNode>(identifier, lower_bound->clone(), upper_bound->clone(), std::unique_ptr<BlockNode>(static_cast<BlockNode*>(body->clone().release())));
    }
};

class WhileLoopNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<BlockNode> body;

    WhileLoopNode(std::unique_ptr<ASTNode> cond, std::unique_ptr<BlockNode> b)
        : condition(std::move(cond)), body(std::move(b)) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<WhileLoopNode>(condition->clone(), std::unique_ptr<BlockNode>(static_cast<BlockNode*>(body->clone().release())));
    }
};

class InputNode : public ASTNode {
public:
    std::string identifier;

    InputNode(const std::string& id)
        : identifier(id) {}

    std::unique_ptr<ASTNode> clone() const override {
        return std::make_unique<InputNode>(identifier);
    }
};

#endif