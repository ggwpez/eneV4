#include "il_optimizer.h"
#include <llvm-c/Analysis.h>
#include <llvm-c/BitReader.h>
#include <llvm-c/BitWriter.h>
#include <llvm-c/Core.h>
#include <llvm-c/Disassembler.h>
#include <llvm-c/ErrorHandling.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Initialization.h>
#include <llvm-c/IRReader.h>
#include <llvm-c/Linker.h>
#include <llvm-c/LinkTimeOptimizer.h>
#include <llvm-c/lto.h>
#include <llvm-c/Object.h>
#include <llvm-c/OrcBindings.h>
#include <llvm-c/Support.h>
#include <llvm-c/Target.h>
#include <llvm-c/TargetMachine.h>
#include <llvm-c/Transforms/IPO.h>
#include <llvm-c/Transforms/PassManagerBuilder.h>
#include <llvm-c/Transforms/Scalar.h>
#include <llvm-c/Transforms/Vectorize.h>
#include <llvm-c/Types.h>

#include <assert.h>
#include <stdio.h>

error_t il_optimize(LLVMModuleRef mod, unsigned level)
{
	assert(mod);
	level = level > 3 ? 3 : level;
	LLVMPassManagerRef pass = LLVMCreatePassManager();
	LLVMPassManagerBuilderRef build = LLVMPassManagerBuilderCreate();

	if (! pass || ! build)
		return UNDERLYING;

	LLVMPassManagerBuilderSetOptLevel(build, level);
	LLVMPassManagerBuilderSetSizeLevel(build, 0);
//	LLVMPassManagerBuilderUseInlinerWithThreshold(build, 16);
	LLVMPassManagerBuilderPopulateModulePassManager(build, pass);

	LLVMRunPassManager(pass, mod);

	LLVMPassManagerBuilderDispose(build);
	LLVMDisposePassManager(pass);
	return SUCCESS;
}
