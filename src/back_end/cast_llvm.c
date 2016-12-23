#include "cast_llvm.h"
#include "../mid_end/trait.h"

LLVMTypeRef cast_llvm_from_inbuild(LLVMContextRef cont, inbuild_type_t* t)
{
	assert(t);

	switch (t->type)
	{
		case IT_VOID:
			return LLVMVoidTypeInContext(cont);
		case IT_BOOL:
			return LLVMIntTypeInContext(cont, 1);
		case IT_U8: case IT_I8:
			return LLVMInt8TypeInContext(cont);
		case IT_U16: case IT_I16:
			return LLVMInt16TypeInContext(cont);
		case IT_U32: case IT_I32:
			return LLVMInt32TypeInContext(cont);
		case IT_U64: case IT_I64:
			return LLVMInt64TypeInContext(cont);

		case IT_F32:
			return LLVMFloatTypeInContext(cont);
		case IT_F64:
			return LLVMDoubleTypeInContext(cont);

		default:
			PANIC
	}
}

LLVMTypeRef cast_llvm_drop_ptr(LLVMTypeRef const t)
{
	assert(0);
}

LLVMTypeRef cast_llvm_from_resolved(LLVMContextRef cont, r_type_t* t)
{
	assert(t);

	if (t->mod == R_TYPE_MOD_PTR)
		return LLVMPointerType(cast_llvm_from_resolved(cont, t->sub), 0);
	else if (t->mod == R_TYPE_MOD_CONST)
		return cast_llvm_from_resolved(cont, t->sub);						// ignore const, SSA is fine with it - AFAIK
	else if (t->mod == R_TYPE_MOD_INBUILD)
		return cast_llvm_from_inbuild(cont, t->inbuild);
	else if (t->mod == R_TYPE_MOD_ARRAY)
		return LLVMArrayType(cast_llvm_from_resolved(cont, t->sub), t->arr_size);
	else
		PANIC
}
