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
  std::cout<<"Creating variable: "<<idName<<" "<<comp<<std::endl;
//  return builder.CreateLoad(nullptr, idName);
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
  std::cout<<"Variable Declaration "<<type.idName<<" "<<id.idName<<" "<<id.comp<<std::endl;
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


llvm::Value* NAssignment::codeGen(CodeGenContext& context) {
  std::cout<<"NAssignment"<<std::endl;
  std::cout<<lhs.idName<<" "<<rhs.idName<<std::endl;
  std::string shr;
  std::string mov;
  std::string cnst = "v,v";
  if(lhs.comp == TXYZW && rhs.comp == TWZYX){
    shr = "v_lshrrev_b32 $0, $1";
    mov = "v_mov_b32_sdwa $0, $1";

  }

  llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx),
          false);
  llvm::InlineAsm *IA = llvm::InlineAsm::get(FT, mov, cnst, true, false);

  llvm::CallInst *Result = builder.CreateCall(IA, context.workItemArgsValue);

  return nullptr;
}

llvm::Value* NFunctionDeclaration::codeGen(CodeGenContext& context) {
  std::cout<<"In function generation"<<std::endl;

  std::vector<llvm::Type*> argTypes(arguments.size(), llvm::Type::getInt32Ty(ctx));

  context.FT = llvm::FunctionType::get(llvm::Type::getInt32Ty(ctx), argTypes, false);

  context.mainFunction = llvm::Function::Create(context.FT, llvm::Function::ExternalLinkage, id.idName, context.module);

  unsigned Idx = 0;
  for(auto &Arg : context.mainFunction->args()) {
    context.locals[arguments[Idx]->id.idName] = &Arg;
    Arg.setName(arguments[Idx++]->id.idName);
    context.workItemArgsValue.push_back(&Arg);
  }

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(ctx, "entry", context.mainFunction);
  builder.SetInsertPoint(BB);


  block.codeGen(context);

  return nullptr;
}


llvm::Value* NReturnStatement::codeGen(CodeGenContext& context)
{
	std::cout << "Generating return code for " << id.idName<< std::endl;
	return nullptr;
}
