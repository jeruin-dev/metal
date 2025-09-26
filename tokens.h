#pragma once

#include <string>

enum class TokenType {
    // Keywords
    DEF,
    EXTERN,

    // Primary
    IDENTIFIER,
    NUMBER,

    // Operators
    PLUS,
    MINUS,
    STAR,
    SLASH,

    // Delimiters
    LPAREN,
    RPAREN,
    SEMICOLON,

    // End of file
    EOF_TOKEN
};

struct Token {
    TokenType type;
    std::string lexeme;
    double numberValue; // For NUMBER tokens

    Token(TokenType t, std::string l = "") : type(t), lexeme(l), numberValue(0.0) {}
    Token(double val) : type(TokenType::NUMBER), lexeme(""), numberValue(val) {}
};