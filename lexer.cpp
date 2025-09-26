#include "lexer.h"
#include <cctype>
#include <iostream>

Lexer::Lexer(const std::string& source) : source(source), current(0), start(0) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (!isAtEnd()) {
        start = current;
        char c = advance();

        switch (c) {
            case '+': tokens.push_back(makeToken(TokenType::PLUS)); break;
            case '-': tokens.push_back(makeToken(TokenType::MINUS)); break;
            case '*': tokens.push_back(makeToken(TokenType::STAR)); break;
            case '/': tokens.push_back(makeToken(TokenType::SLASH)); break;
            case '(': tokens.push_back(makeToken(TokenType::LPAREN)); break;
            case ')': tokens.push_back(makeToken(TokenType::RPAREN)); break;
            case ',': tokens.push_back(makeToken(TokenType::SEMICOLON)); break;
            default:
                if (std::isdigit(c)) {
                    tokens.push_back(number());
                } else if (std::isalpha(c) || c == '_') {
                    tokens.push_back(identifier());
                } else {
                    std::cerr << "Unexpected character: " << c << std::endl;
                }
                break;
        }
    }

    tokens.push_back(Token(TokenType::EOF_TOKEN, ""));
    return tokens;
}

bool Lexer::isAtEnd() const {
    return current >= source.length();
}

char Lexer::advance() {
    return source[current++];
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::peekNext() const {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() && std::isspace(peek())) {
        advance();
    }
}

Token Lexer::number() {
    while (std::isdigit(peek())) advance();

    if (peek() == '.' && std::isdigit(peekNext())) {
        advance();
        while (std::isdigit(peek())) advance();
    }

    std::string numStr = source.substr(start, current - start);
    return Token(std::stod(numStr));
}

Token Lexer::identifier() {
    while (std::isalnum(peek()) || peek() == '_') advance();

    std::string text = source.substr(start, current - start);

    // Check for keywords
    if (text == "def") return Token(TokenType::DEF, text);
    if (text == "extern") return Token(TokenType::EXTERN, text);

    return Token(TokenType::IDENTIFIER, text);
}

Token Lexer::makeToken(TokenType type) {
    std::string lexeme = source.substr(start, current - start);
    return Token(type, lexeme);
}
