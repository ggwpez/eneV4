#pragma once

#include "../ast/ast.h"
#include <llvm-c/Core.h>

typedef struct
{
	char* name;
	LLVMValueRef val;
} var_t;

typedef var_t* var_ptr;
MAKE_VECTOR_H(var)

var_t* var_new(const char* name, LLVMValueRef val);
void var_del(var_t* node);
void var_print(var_t* node);
