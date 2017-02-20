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
	std::string ifstr("ifstr");
	CodeGenContext cgCtx(ifstr);
	cgCtx.generateCode(*programBlock);
	cgCtx.module->print(llvm::errs(), nullptr);
	return 0;
}
