#include "il.h"
#include "cast_llvm.h"
#include "../mid_end/trait.h"

#include <llvm-c/Core.h>
#include <llvm-c/Object.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/Types.h>
#include <llvm-c/BitWriter.h>
#include <stdio.h>

MAKE_VTABLE_C(il_creates, il_create_, il_creator_ptr_t)

static LLVMModuleRef mod = NULL;
static LLVMContextRef con = NULL;
static LLVMBuilderRef build = NULL;

error_t il_process(program_node_t* node, char const* mod_name, char** output)
{
	assert(node);
	assert(mod_name);
	assert(output);

	mod = LLVMModuleCreateWithName(mod_name);
	con = LLVMGetModuleContext(mod);
	build = LLVMCreateBuilderInContext(con);

	LLVMModuleRef prog = il_create_program(node);
	if (! prog)
		return UNKNOWN;

	char* error = NULL;
	if (LLVMVerifyModule(mod, LLVMPrintMessageAction, &error))
		return UNDERLYING;
	LLVMDisposeMessage(error);

	*output = LLVMPrintModuleToString(mod);
	if (! output)
		return UNDERLYING;

	return SUCCESS;
}

void* il_create_ast(ast_ptr node)
{
	assert(node);
	assert(node->t < AST_size);

	return il_creates[node->t](node->node);
}

LLVMValueRef il_create_atom(atom_node_t* node)
{
	assert(node);

	switch (node->t)
	{
		case ATOM_UINT8:
			return LLVMConstInt(LLVMInt8TypeInContext(con), node->u8, 0);
		case ATOM_UINT16:
			return LLVMConstInt(LLVMInt16TypeInContext(con), node->u16, 0);
		case ATOM_UINT32:
			return LLVMConstInt(LLVMInt32TypeInContext(con), node->u32, 0);
		case ATOM_UINT64:
			return LLVMConstInt(LLVMInt64TypeInContext(con), node->u64, 0);

		case ATOM_SINT8:
			return LLVMConstInt(LLVMInt8TypeInContext(con), node->s8,  1);
		case ATOM_SINT16:
			return LLVMConstInt(LLVMInt16TypeInContext(con), node->s16, 1);
		case ATOM_SINT32:
			return LLVMConstInt(LLVMInt32TypeInContext(con), node->s32, 1);
		case ATOM_SINT64:
			return LLVMConstInt(LLVMInt64TypeInContext(con), node->s64, 1);

		case ATOM_FLOAT:
			return LLVMConstReal(LLVMFloatTypeInContext(con), node->f32);
		case ATOM_DOUBLE:
			return LLVMConstReal(LLVMDoubleTypeInContext(con), node->f64);

		case ATOM_REF_TO_RES:
		default:
			PANIC;
	}

	return NULL;
}

LLVMValueRef il_create_unop(unop_node_t* node)
{
	assert(node);

	return NULL;
}

LLVMValueRef il_create_binop(binop_node_t* node)
{
	assert(node);
	r_type_t* t1 = trait_typeof(node->x),
			* t2 = trait_typeof(node->y);

	assert(trait_is_same(t1, t2) || trait_is_stack_top(t2));	// scoper should have casted both to common type, or top stack ref
	bool is_floating = trait_is_floating(t1);

	LLVMValueRef x = il_create_ast(node->x), y;

	if (trait_is_stack_top(t2))
		y = x;
	else
		y = il_create_ast(node->y);

	if (!x || !y)
		return NULL;

	switch (node->t)
	{
		case BINOP_ADD:
			return is_floating ? LLVMBuildFAdd(build, x, y, "faddtmp") : LLVMBuildAdd(build, x, y, "addtmp");
		case BINOP_SUB:
			return is_floating ? LLVMBuildFSub(build, x, y, "fsubtmp") : LLVMBuildSub(build, x, y, "subtmp");
		case BINOP_MUL:
			return is_floating ? LLVMBuildFMul(build, x, y, "fMultmp") : LLVMBuildMul(build, x, y, "multmp");
		case BINOP_DIV:
			return is_floating ? LLVMBuildFMul(build, x, y, "fMultmp") : LLVMBuildMul(build, x, y, "multmp");
		default:
			return NULL;
	}

	return NULL;
}

LLVMModuleRef il_create_program(program_node_t* node)
{
	assert(node);

	size_t s = ast_vector_size(node->v);
	for (size_t i = 0; i < s; ++i)
	{
		ast_ptr inst = ast_vector_at(node->v, i);
		if (! il_create_ast(inst))
			return NULL;
	}

	return mod;
}

LLVMBasicBlockRef il_create_block(block_node_t* node)
{
	assert(node);

	size_t s = ast_vector_size(node->v);
	for (size_t i = 0; i < s; ++i)
	{
		ast_ptr inst = ast_vector_at(node->v, i);
		if (! il_create_ast(inst))
			return NULL;
	}

	return LLVMGetInsertBlock(build);
}

LLVMValueRef il_create_return(return_node_t* node)
{
	assert(node);

	LLVMValueRef ret = il_create_ast(node->exp);
	if (! ret)
		return NULL;
	return LLVMBuildRet(build, ret);
}

LLVMValueRef il_create_ident(ident_node_t* node)
{
	assert(node);

	return NULL;
}

LLVMTypeRef il_create_type(type_node_t* node)
{
	assert(node);

	return cast_llvm_from_resolved(con, node->r_type);
}

LLVMValueRef il_create_texp(texp_node_t* node)
{
	assert(node);

	LLVMValueRef val = il_create_ast(node->exp);
	LLVMTypeRef type = cast_llvm_from_resolved(con, node->type);

	if (node->cast_type == CAST_size)
		return val;
	else
	{
		return LLVMBuildCast(build, (LLVMOpcode)node->cast_type, val, type, "texp_cast");
	}
}

LLVMValueRef il_create_var_decl(var_decl_node_t* node)
{
	assert(node);

	return NULL;
}

LLVMValueRef il_create_fun_decl(fun_decl_node_t* node)
{
	assert(node);
	LLVMContextRef con = LLVMGetModuleContext(mod);

	// HEAD
	size_t args_s = var_decl_vector_size(node->args);
	LLVMTypeRef* args_t = malloc(sizeof(LLVMTypeRef) *args_s);
	LLVMTypeRef ret_t;

	for (size_t	i = 0; i < args_s; ++i)
	{
		var_decl_node_t* var_decl = var_decl_vector_at(node->args, i);
		assert(args_t[i] = il_create_type(var_decl->type));
	}
	ret_t = il_create_type(node->type);

	LLVMTypeRef fun_t = LLVMFunctionType(ret_t, args_t, args_s, 0);
	LLVMValueRef fun = LLVMAddFunction(mod, node->name->str, fun_t);
	free(args_t);

	for	(size_t i = 0; i < args_s; ++i)
	{
		LLVMValueRef arg = LLVMGetParam(fun, i);
		var_decl_node_t* arg_decl = var_decl_vector_at(node->args, i);
		char const* arg_name = arg_decl->name->str;

		LLVMSetValueName(arg, arg_name);
	}

	// BODY
	LLVMBasicBlockRef block = LLVMAppendBasicBlockInContext(con, fun, "entry");
	LLVMPositionBuilderAtEnd(build, block);

	LLVMBasicBlockRef body = il_create_block(node->code);
	if (! body)
	{
		LLVMDeleteFunction(fun);
		return NULL;
	}

	if (LLVMVerifyFunction(fun, LLVMPrintMessageAction))
	{
		LLVMDeleteFunction(fun);
		return NULL;
	}

	return fun;
}

LLVMValueRef il_create_fun_call(fun_call_node_t* node)
{
	assert(node);

	return NULL;
}

LLVMValueRef il_create_if(if_node_t* node)
{
	assert(node);

	return NULL;
}

LLVMValueRef il_create_while(while_node_t* node)
{
	assert(node);

	return NULL;
}

LLVMValueRef il_create_for(for_node_t* node)
{
	assert(node);

	return NULL;
}
