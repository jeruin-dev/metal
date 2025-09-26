#pragma once

#include "ast.h"
#include "tokens.h"
#include <vector>
#include <memory>

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::unique_ptr<Function> parseFunction();
    std::unique_ptr<Prototype> parsePrototype();
    std::unique_ptr<Expr> parseExpression();

private:
    std::vector<Token> tokens;
    size_t current;

    Token peek() const;
    Token previous() const;
    Token advance();
    bool isAtEnd() const;
    bool check(TokenType type) const;
    bool match(TokenType type);
    Token consume(TokenType type, const std::string& message);

    std::unique_ptr<Expr> parsePrimary();
    std::unique_ptr<Expr> parseBinaryOp(int precedence);
    int getPrecedence(char op);
};