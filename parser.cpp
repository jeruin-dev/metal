#include "parser.h"
#include <iostream>
#include <memory>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

std::unique_ptr<Function> Parser::parseFunction() {
    auto proto = parsePrototype();
    if (!proto) return nullptr;

    auto body = parseExpression();
    if (!body) return nullptr;

    return std::make_unique<Function>(std::move(proto), std::move(body));
}

std::unique_ptr<Prototype> Parser::parsePrototype() {
    if (!match(TokenType::IDENTIFIER)) {
        std::cerr << "Expected function name" << std::endl;
        return nullptr;
    }

    std::string name = previous().lexeme;
    std::vector<std::string> args;

    if (!match(TokenType::LPAREN)) {
        std::cerr << "Expected '(' after function name" << std::endl;
        return nullptr;
    }

    while (!check(TokenType::RPAREN)) {
        if (!match(TokenType::IDENTIFIER)) {
            std::cerr << "Expected parameter name" << std::endl;
            return nullptr;
        }
        args.push_back(previous().lexeme);

        if (!check(TokenType::RPAREN)) {
            if (!match(TokenType::SEMICOLON)) {
                std::cerr << "Expected ',' or ')' in parameter list" << std::endl;
                return nullptr;
            }
        }
    }

    if (!match(TokenType::RPAREN)) {
        std::cerr << "Expected ')' after parameter list" << std::endl;
        return nullptr;
    }

    return std::make_unique<Prototype>(name, args);
}

std::unique_ptr<Expr> Parser::parseExpression() {
    return parseBinaryOp(0);
}

std::unique_ptr<Expr> Parser::parsePrimary() {
    if (match(TokenType::NUMBER)) {
        return std::make_unique<NumberExpr>(previous().numberValue);
    }

    if (match(TokenType::IDENTIFIER)) {
        std::string name = previous().lexeme;

        if (check(TokenType::LPAREN)) {
            // Function call
            advance(); // consume '('
            std::vector<std::unique_ptr<Expr>> args;

            if (!check(TokenType::RPAREN)) {
                do {
                    auto arg = parseExpression();
                    if (!arg) return nullptr;
                    args.push_back(std::move(arg));
                } while (match(TokenType::SEMICOLON));
            }

            if (!match(TokenType::RPAREN)) {
                std::cerr << "Expected ')' after argument list" << std::endl;
                return nullptr;
            }

            return std::make_unique<CallExpr>(name, std::move(args));
        } else {
            // Variable reference
            return std::make_unique<VariableExpr>(name);
        }
    }

    if (match(TokenType::LPAREN)) {
        auto expr = parseExpression();
        if (!match(TokenType::RPAREN)) {
            std::cerr << "Expected ')'" << std::endl;
            return nullptr;
        }
        return expr;
    }

    std::cerr << "Expected expression" << std::endl;
    return nullptr;
}

std::unique_ptr<Expr> Parser::parseBinaryOp(int precedence) {
    auto left = parsePrimary();
    if (!left) return nullptr;

    while (true) {
        char op = peek().lexeme[0];
        int opPrecedence = getPrecedence(op);

        if (opPrecedence < precedence) break;

        advance(); // consume operator

        auto right = parseBinaryOp(opPrecedence + 1);
        if (!right) return nullptr;

        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }

    return left;
}

int Parser::getPrecedence(char op) {
    switch (op) {
        case '+':
        case '-': return 10;
        case '*':
        case '/': return 20;
        default: return -1;
    }
}

Token Parser::peek() const {
    return tokens[current];
}

Token Parser::previous() const {
    return tokens[current - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::EOF_TOKEN;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();

    std::cerr << message << std::endl;
    return Token(TokenType::EOF_TOKEN, "");
}