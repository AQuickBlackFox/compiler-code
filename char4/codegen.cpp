#include "codegen.h"
#include <iostream>
#include "parser.hpp"

void CodeGenContext::generateCode(NBlock& root) {
  /*
  std::vector<llvm::Type*> args;
  args.push_back(llvm::Type::getInt8PtrTy(ctx));

  llvm::FunctionType* funcType =
    llvm::FunctionType::get(
      llvm::Type::getInt32Ty(ctx), args, false
    );

  llvm::Function* func =
    llvm::Function::Create(
      funcType,
      llvm::Function::ExternalLinkage,
      "func",
      module
    );

  func->setCallingConv(llvm::CallingConv::C);

  entry = llvm::BasicBlock::Create(ctx, "", func);
  mainFunction = func;
  */
  root.codeGen(*this);
}

llvm::Value* NInteger::codeGen(CodeGenContext& context) {
  std::cout<< "Creating Integer: "<<value<<std::endl;
  return llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), value, true);
}

llvm::Value* NDouble::codeGen(CodeGenContext& context) {
  std::cout<<"Creating Double: "<<value<<std::endl;
  return llvm::ConstantFP::get(llvm::Type::getDoubleTy(ctx), value);
}

llvm::Value* NIdentifier::codeGen(CodeGenContext& context) {
  std::cout<<"Creating variable: "<<idName<<std::endl;
  return builder.CreateLoad(nullptr, idName);
}

llvm::Value* NIfBlock::codeGen(CodeGenContext& context) {
  std::cout<<"Creating If Block"<<std::endl;
  llvm::Value *eq = expression->codeGen(context);
  llvm::BasicBlock *thenbb = llvm::BasicBlock::Create(ctx, "then", context.mainFunction);
  builder.CreateCondBr(eq, thenbb, nullptr);
  return eq;
}

llvm::Value* NExpressionStatement::codeGen(CodeGenContext& context) {
  std::cout<<"Generating code " <<typeid(expression).name() << std::endl;
  return expression.codeGen(context);
}

llvm::Value* NVariableDeclaration::codeGen(CodeGenContext& context) {
  std::cout<<"Variable Declaration "<<type.idName<<" "<<id.idName<<std::endl;
  return nullptr;
}

llvm::Value* NBlock::codeGen(CodeGenContext& context) {
  StatementList::const_iterator it;
  llvm::Value* last = NULL;
  for(it = statements.begin(); it != statements.end(); it++) {
    std::cout<<"Generating code for "<<typeid(**it).name() << std::endl;
    last = (**it).codeGen(context);
  }
  return last;
}

llvm::Value* NBinaryOperator::codeGen(CodeGenContext& context) {
  std::cout<<"Creating binary operator "<<op<<" "<<TCEQ<<std::endl;
  switch(op) {
    case TCEQ :   return builder.CreateICmpEQ(//context.currentBlock(),
                                lhs.codeGen(context),
                                rhs.codeGen(context),
                                "tmp"
                              );
  }
  return NULL;
}

llvm::Value* NFunctionDeclaration::codeGen(CodeGenContext& context) {
  std::cout<<"In function generation"<<std::endl;

  std::vector<llvm::Type*> argTypes(arguments.size(), llvm::Type::getInt32Ty(ctx));

  llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getInt32Ty(ctx), argTypes, false);

  llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, id.idName, context.module);

  unsigned Idx = 0;
  for(auto &Arg : F->args()) {
    Arg.setName(arguments[Idx++]->id.idName);
  }

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(ctx, "entry", F);
  builder.SetInsertPoint(BB);

  return nullptr;
}
