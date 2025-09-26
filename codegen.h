#pragma once

#include "ast.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <map>

class CodeGen {
public:
    CodeGen();
    void generateCode(const Function& func);
    void printIR();

private:
    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::Module> module;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    std::map<std::string, llvm::Value*> namedValues;

    llvm::Value* codegen(const Expr& expr);
    llvm::Value* codegen(const NumberExpr& expr);
    llvm::Value* codegen(const VariableExpr& expr);
    llvm::Value* codegen(const BinaryExpr& expr);
    llvm::Value* codegen(const CallExpr& expr);
    llvm::Function* codegen(const Prototype& proto);
};