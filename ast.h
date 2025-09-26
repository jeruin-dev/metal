#pragma once

#include <memory>
#include <string>
#include <vector>

// Forward declarations
class Expr;
class NumberExpr;
class VariableExpr;
class BinaryExpr;
class CallExpr;
class Prototype;
class Function;

// Base expression class
class Expr {
public:
    virtual ~Expr() = default;
};

// Number literal
class NumberExpr : public Expr {
public:
    double value;
    NumberExpr(double val) : value(val) {}
};

// Variable reference
class VariableExpr : public Expr {
public:
    std::string name;
    VariableExpr(const std::string& n) : name(n) {}
};

// Binary operation
class BinaryExpr : public Expr {
public:
    char op;
    std::unique_ptr<Expr> left, right;
    BinaryExpr(char o, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
};

// Function call
class CallExpr : public Expr {
public:
    std::string callee;
    std::vector<std::unique_ptr<Expr>> args;
    CallExpr(const std::string& c, std::vector<std::unique_ptr<Expr>> a)
        : callee(c), args(std::move(a)) {}
};

// Function prototype
class Prototype {
public:
    std::string name;
    std::vector<std::string> args;
    Prototype(const std::string& n, std::vector<std::string> a)
        : name(n), args(std::move(a)) {}
};

// Function definition
class Function {
public:
    std::unique_ptr<Prototype> proto;
    std::unique_ptr<Expr> body;
    Function(std::unique_ptr<Prototype> p, std::unique_ptr<Expr> b)
        : proto(std::move(p)), body(std::move(b)) {}
};