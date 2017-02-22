#include <stack>
#include <typeinfo>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/raw_ostream.h>
#include "node.h"

static llvm::LLVMContext ctx;
static llvm::IRBuilder<> builder(ctx);

class CodeGenBlock {
public:
  llvm::BasicBlock *block;
  llvm::Value *returnValue;
  std::map<std::string, llvm::Value*> locals;
};

class CodeGenContext {
  std::stack<CodeGenBlock*> blocks;
public:
  llvm::Function *mainFunction;

  llvm::Module* module;
  llvm::BasicBlock *entry;
  CodeGenContext(std::string modName)
  {
    module = new llvm::Module(modName.c_str(), ctx);
  }
  void generateCode(NBlock& root);
  llvm::BasicBlock* currentBlock() { return entry; }
};

void createCoreFunctions(CodeGenContext &ctx);
llvm::Function* createPrintfFunction(CodeGenContext& context);
