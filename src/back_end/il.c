#include "il.h"
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
#include <stdio.h>

MAKE_VTABLE_C(il_creates, il_create_, il_creator_ptr_t)

error_t il_process(ast_ptr node, char const* file_name)
{
	assert(node);
	assert(file_name);

	LLVMModuleRef mod = LLVMModuleCreateWithName("dein_modul");

	LLVMTypeRef param_types[] = { LLVMInt32Type(), LLVMInt32Type() };
	LLVMTypeRef ret_type = LLVMFunctionType(LLVMInt32Type(), param_types, 2, 0);
	LLVMValueRef sum = LLVMAddFunction(mod, "sum", ret_type);

	LLVMBasicBlockRef entry = LLVMAppendBasicBlock(sum, "entry");
	LLVMBuilderRef builder = LLVMCreateBuilder();
	LLVMPositionBuilderAtEnd(builder, entry);

	LLVMValueRef tmp = LLVMBuildAdd(builder, LLVMGetParam(sum, 0), LLVMGetParam(sum, 1), "tmp");
	LLVMBuildRet(builder, tmp);

	char* error = NULL;
	if (LLVMVerifyModule(mod, LLVMPrintMessageAction, &error))
		return UNDERLYING;
	LLVMDisposeMessage(error);

	if (LLVMPrintModuleToFile(mod, file_name, &error))
	{
		if (*error)
		{
			fprintf(stderr, "%s\n", error);
			return 1;
		}
		LLVMDisposeMessage(error);
	}

	return SUCCESS;
}

error_t il_create_ast(LLVMModuleRef mod, LLVMBuilderRef build, ast_ptr node)
{
	assert(node);
	assert(node->t < AST_size);

	return il_creates[node->t](mod, build, node->node);
}

error_t il_create_atom(LLVMModuleRef mod, LLVMBuilderRef build, atom_node_t* node)
{
	assert(node);

	return SUCCESS;
}

error_t il_create_unop(LLVMModuleRef mod, LLVMBuilderRef build, unop_node_t* node)
{
	assert(node);

	return SUCCESS;
}

error_t il_create_binop(LLVMModuleRef mod, LLVMBuilderRef build, binop_node_t* node)
{
	assert(node);

	return SUCCESS;
}

error_t il_create_program(LLVMModuleRef mod, LLVMBuilderRef build, program_node_t* node)
{
	assert(node);

	return SUCCESS;
}

error_t il_create_block(LLVMModuleRef mod, LLVMBuilderRef build, block_node_t* node)
{
	assert(node);

	return SUCCESS;
}

error_t il_create_return(LLVMModuleRef mod, LLVMBuilderRef build, return_node_t* node)
{
	assert(node);

	return SUCCESS;
}

error_t il_create_ident(LLVMModuleRef mod, LLVMBuilderRef build, ident_node_t* node)
{
	assert(node);

	return SUCCESS;
}

error_t il_create_type(LLVMModuleRef mod, LLVMBuilderRef build, type_node_t* node)
{
	assert(node);

	return SUCCESS;
}

error_t il_create_texp(LLVMModuleRef mod, LLVMBuilderRef build, texp_node_t* node)
{
	assert(node);

	return SUCCESS;
}

error_t il_create_var_decl(LLVMModuleRef mod, LLVMBuilderRef build, var_decl_node_t* node)
{
	assert(node);

	return SUCCESS;
}

error_t il_create_fun_decl(LLVMModuleRef mod, LLVMBuilderRef build, fun_decl_node_t* node)
{
	assert(node);

	return SUCCESS;
}

error_t il_create_fun_call(LLVMModuleRef mod, LLVMBuilderRef build, fun_call_node_t* node)
{
	assert(node);

	return SUCCESS;
}

error_t il_create_if(LLVMModuleRef mod, LLVMBuilderRef build, if_node_t* node)
{
	assert(node);

	return SUCCESS;
}

error_t il_create_while(LLVMModuleRef mod, LLVMBuilderRef build, while_node_t* node)
{
	assert(node);

	return SUCCESS;
}

error_t il_create_for(LLVMModuleRef mod, LLVMBuilderRef build, for_node_t* node)
{
	assert(node);

	return SUCCESS;
}
