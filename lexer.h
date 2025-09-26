#pragma once

#include "tokens.h"
#include <string>
#include <vector>

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t current;
    size_t start;

    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    void skipWhitespace();
    Token number();
    Token identifier();
    Token makeToken(TokenType type);
};