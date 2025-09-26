#include "codegen.h"
#include <llvm/IR/Verifier.h>
#include <iostream>

CodeGen::CodeGen()
    : context(std::make_unique<llvm::LLVMContext>()),
      module(std::make_unique<llvm::Module>("metal", *context)),
      builder(std::make_unique<llvm::IRBuilder<>>(*context)) {}

void CodeGen::generateCode(const Function& func) {
    // Generate function prototype
    llvm::Function* function = codegen(*func.proto);
    if (!function) return;

    // Create basic block
    llvm::BasicBlock* BB = llvm::BasicBlock::Create(*context, "entry", function);
    builder->SetInsertPoint(BB);

    // Clear named values
    namedValues.clear();

    // Add function arguments to namedValues
    for (auto& arg : function->args()) {
        namedValues[arg.getName().str()] = &arg;
    }

    // Generate code for function body
    llvm::Value* retVal = codegen(*func.body);
    if (retVal) {
        builder->CreateRet(retVal);

        // Verify the function
        llvm::verifyFunction(*function);

        std::cout << "Generated function:" << std::endl;
        function->print(llvm::outs());
        std::cout << std::endl;
    } else {
        function->eraseFromParent();
    }
}

void CodeGen::printIR() {
    module->print(llvm::outs(), nullptr);
}

llvm::Value* CodeGen::codegen(const Expr& expr) {
    if (auto* numExpr = dynamic_cast<const NumberExpr*>(&expr)) {
        return codegen(*numExpr);
    } else if (auto* varExpr = dynamic_cast<const VariableExpr*>(&expr)) {
        return codegen(*varExpr);
    } else if (auto* binExpr = dynamic_cast<const BinaryExpr*>(&expr)) {
        return codegen(*binExpr);
    } else if (auto* callExpr = dynamic_cast<const CallExpr*>(&expr)) {
        return codegen(*callExpr);
    }
    return nullptr;
}

llvm::Value* CodeGen::codegen(const NumberExpr& expr) {
    return llvm::ConstantFP::get(*context, llvm::APFloat(expr.value));
}

llvm::Value* CodeGen::codegen(const VariableExpr& expr) {
    llvm::Value* V = namedValues[expr.name];
    if (!V) {
        std::cerr << "Unknown variable name: " << expr.name << std::endl;
        return nullptr;
    }
    return V;
}

llvm::Value* CodeGen::codegen(const BinaryExpr& expr) {
    llvm::Value* L = codegen(*expr.left);
    llvm::Value* R = codegen(*expr.right);
    if (!L || !R) return nullptr;

    switch (expr.op) {
        case '+': return builder->CreateFAdd(L, R, "addtmp");
        case '-': return builder->CreateFSub(L, R, "subtmp");
        case '*': return builder->CreateFMul(L, R, "multmp");
        case '/': return builder->CreateFDiv(L, R, "divtmp");
        default:
            std::cerr << "Unknown binary operator: " << expr.op << std::endl;
            return nullptr;
    }
}

llvm::Value* CodeGen::codegen(const CallExpr& expr) {
    llvm::Function* CalleeF = module->getFunction(expr.callee);
    if (!CalleeF) {
        std::cerr << "Unknown function referenced: " << expr.callee << std::endl;
        return nullptr;
    }

    if (CalleeF->arg_size() != expr.args.size()) {
        std::cerr << "Incorrect number of arguments passed" << std::endl;
        return nullptr;
    }

    std::vector<llvm::Value*> ArgsV;
    for (auto& arg : expr.args) {
        ArgsV.push_back(codegen(*arg));
        if (!ArgsV.back()) return nullptr;
    }

    return builder->CreateCall(CalleeF, ArgsV, "calltmp");
}

llvm::Function* CodeGen::codegen(const Prototype& proto) {
    std::vector<llvm::Type*> Doubles(proto.args.size(), llvm::Type::getDoubleTy(*context));
    llvm::FunctionType* FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(*context), Doubles, false);
    llvm::Function* F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, proto.name, module.get());

    // Set names for all arguments
    unsigned Idx = 0;
    for (auto& Arg : F->args()) {
        Arg.setName(proto.args[Idx++]);
    }

    return F;
}