#pragma once

#include "../mid_end/r_type.h"
#include <llvm-c/Core.h>

LLVMTypeRef cast_llvm_from_inbuild(LLVMContextRef cont, inbuild_type_t* t);
LLVMTypeRef cast_llvm_from_resolved(LLVMContextRef cont, r_type_t* t);
