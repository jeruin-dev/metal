#include <iostream>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"

#define PROJECT_NAME "metal"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <source code>" << std::endl;
        return 1;
    }

    std::string source = argv[1];

    // Lexical analysis
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    std::cout << "Tokens:" << std::endl;
    for (const auto& token : tokens) {
        if (token.type == TokenType::NUMBER) {
            std::cout << "NUMBER: " << token.numberValue << std::endl;
        } else {
            std::cout << token.lexeme << std::endl;
        }
    }

    // Parsing
    Parser parser(tokens);
    auto function = parser.parseFunction();

    if (function) {
        std::cout << "\nParsed function successfully!" << std::endl;

        // Code generation
        CodeGen codegen;
        codegen.generateCode(*function);

        std::cout << "\nGenerated LLVM IR:" << std::endl;
        codegen.printIR();
    } else {
        std::cout << "\nFailed to parse function." << std::endl;
        return 1;
    }

    return 0;
}
