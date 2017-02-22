#include <iostream>
#include "codegen.h"

extern int yyparse();
extern NBlock* programBlock;

int main(int argc, char **argv)
{
	yyparse();
	std::cout << programBlock << std::endl;
	llvm::InitializeNativeTarget();
	llvm::InitializeNativeTargetAsmPrinter();
	llvm::InitializeNativeTargetAsmParser();
	std::string opt1("opt1");
	CodeGenContext cgCtx(opt1);
	cgCtx.generateCode(*programBlock);
	cgCtx.module->print(llvm::errs(), nullptr);
	return 0;
}
