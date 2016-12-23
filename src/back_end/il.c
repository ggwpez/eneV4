
#include "il.h"
#include "cast_llvm.h"
#include "../mid_end/cast.h"
#include "../mid_end/trait.h"
#include "il_optimizer.h"
#include "var_scope.h"

#include <llvm-c/Core.h>
#include <llvm-c/Object.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/Types.h>
#include <stdio.h>

MAKE_VTABLE_C(il_creates, il_create_,, il_creator_ptr_t)

static LLVMModuleRef mod = NULL;
static LLVMContextRef con = NULL;
static LLVMBuilderRef build = NULL;
static var_scope_t* sc = NULL;			// scope

error_t il_process(program_node_t* node, unsigned opt_lvl, char const* mod_name, char** output)
{
	assert(node);
	assert(mod_name);
	assert(output);

	mod = LLVMModuleCreateWithName(mod_name);
	con = LLVMGetModuleContext(mod);
	build = LLVMCreateBuilderInContext(con);
	sc = var_scope_new();

	CHECK(mod && con && build, UNDERLYING,);

	LLVMModuleRef prog = il_create_program(node);
	CHECK(prog, ASSERT,);

	il_optimize(mod, opt_lvl);

	char* error = NULL;
	if (LLVMVerifyModule(mod, LLVMPrintMessageAction, &error))
	{
		LLVMDisposeMessage(error);
		return UNDERLYING;
	}
	LLVMDisposeMessage(error);

	*output = LLVMPrintModuleToString(mod);
	CHECK(output, UNDERLYING,);

	var_scope_del(sc);
	LLVMDisposeBuilder(build);
	LLVMDisposeModule(mod);

	return SUCCESS;
}

void* il_create_ast(ast_ptr node)
{
	assert(node);
	assert(node->t < AST_size);

	return il_creates[node->t](node->node);
}

LLVMModuleRef il_create_program(program_node_t* node)
{
	assert(node);

	var_scope_enter(sc);
	size_t s = ast_vec_size(node->v);
	for (size_t i = 0; i < s; ++i)
	{
		ast_ptr inst = ast_vec_at(node->v, i);
		CHECK(il_create_ast(inst), UNKNOWN,);
	}
	var_scope_leave(sc);

	return mod;
}

LLVMBasicBlockRef il_create_block(block_node_t* node)
{
	assert(node);

	var_scope_enter(sc);
	size_t s = ast_vec_size(node->v);
	for (size_t i = 0; i < s; ++i)
	{
		ast_ptr inst = ast_vec_at(node->v, i);
		LLVMValueRef llvm_inst = il_create_ast(inst);
		CHECK(llvm_inst, UNDERLYING,);
	}
	var_scope_leave(sc);

	return LLVMGetInsertBlock(build);
}


LLVMValueRef il_create_atom(atom_node_t* node)
{
	assert(node);

	switch (node->t)
	{
		case ATOM_BOOL:
			return LLVMConstInt(LLVMInt1TypeInContext(con), node->boolean, 0);

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

		case ATOM_REF_TO_RES: default:
			PANIC
	}

	PANIC
}

LLVMValueRef il_create_unop(unop_node_t* node)
{
	assert(node);

	r_type_t* t = trait_typeof(node->node);
	LLVMValueRef ptr = il_create_ast(node->node);

	CHECK(t && ptr, ASSERT,);

	switch (node->t)
	{
		case UNOP_DRF:
		{
			LLVMValueRef null = LLVMConstInt(LLVMInt32TypeInContext(con), 0, 0);
			LLVMValueRef v_ptr = LLVMBuildGEP(build, ptr, &null, 1, "lea");
			//LLVMValueRef v_ptr = LLVMBuildExtractValue(build, ptr, 0, "ptr-lea");

			return LLVMBuildLoad(build, v_ptr, "drf");
		}
		case UNOP_NOT:
		{
			return LLVMBuildNot(build, ptr, "not");
		}
		default:
			PANIC
	}

	PANIC
}

LLVMValueRef il_create_binop(binop_node_t* node)
{
	assert(node);
	r_type_t* t1 = trait_typeof(node->x),
			* t2 = trait_typeof(node->y);
	bool is_floating = trait_is_floating(t1),
		 is_signed = trait_is_signed(t1);
	LLVMValueRef x = il_create_ast(node->x), y;

	if (trait_is_stack_top(t2))
		y = x;
	else
		y = il_create_ast(node->y);

	CHECK(t1 && t2 && x && y, ASSERT,);

	switch (node->t)
	{
		case BINOP_ASS:
		{
			LLVMValueRef null = LLVMConstInt(LLVMInt32TypeInContext(con), 0, 0);
			LLVMValueRef v_ptr = LLVMBuildGEP(build, x, &null, 1, "lea");
			//LLVMValueRef v_ptr = LLVMBuildExtractValue(build, x, 0, "ptr-lea");

			return LLVMBuildStore(build, y, v_ptr);
		}
		case BINOP_ARR:
		{
			LLVMValueRef v_ptr = LLVMBuildGEP(build, x, &y, 1, "arr-lea");

			return LLVMBuildLoad(build, v_ptr, "arr-drf");
		}
		case BINOP_ADD:
			return is_floating ? LLVMBuildFAdd(build, x, y, "faddtmp") : LLVMBuildAdd(build, x, y, "add");
		case BINOP_SUB:
			return is_floating ? LLVMBuildFSub(build, x, y, "fsubtmp") : LLVMBuildSub(build, x, y, "sub");
		case BINOP_MUL:
			return is_floating ? LLVMBuildFMul(build, x, y, "fMultmp") : LLVMBuildMul(build, x, y, "mul");
		case BINOP_DIV:
			return is_floating ? LLVMBuildFDiv(build, x, y, "fDivtmp") : (is_signed ? LLVMBuildSDiv(build, x, y, "sdiv") : LLVMBuildUDiv(build, x, y, "udiv"));
		case BINOP_SML:
		{
			if (is_floating)
				return LLVMBuildFCmp(build, LLVMRealULT, x, y, "f_less");
			else
			{
				if (is_signed)
					return LLVMBuildICmp(build, LLVMIntSLT, x, y, "i_less");
				else
					return LLVMBuildICmp(build, LLVMIntULT, x, y, "u_less");
			}
		}
		case BINOP_GRT:
		{
			if (is_floating)
				return LLVMBuildFCmp(build, LLVMRealUGT, x, y, "f_greater");
			else
			{
				if (is_signed)
					return LLVMBuildICmp(build, LLVMIntSGT, x, y, "i_greater");
				else
					return LLVMBuildICmp(build, LLVMIntUGT, x, y, "u_greater");
			}
		}
		case BINOP_EQU:
		{
			if (is_floating)
				return LLVMBuildFCmp(build, LLVMRealUEQ, x, y, "f_eq");
			else
				return LLVMBuildICmp(build, LLVMIntEQ, x, y, "i_eq");
		}
		default:
			PANIC
	}

	PANIC
}

LLVMValueRef il_create_return(return_node_t* node)
{
	assert(node);

	LLVMValueRef ret = il_create_ast(node->exp);
	CHECK(ret, ASSERT,);

	return LLVMBuildRet(build, ret);
}

LLVMValueRef il_create_ident(ident_node_t* node)
{
	assert(node);

	var_t* var = var_scope_get(sc, node->str);
	assert(var);											// cant happen, ensured by the scoper
	LLVMValueRef val = var->val;
	assert(val);											// "

	return val;
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
	else if (node->cast_type == CAST_TO_BOOL)
		return LLVMBuildIsNotNull(build, val, "texp_cast");
	else if (node->cast_type == CAST_BOOL_INT)
		return LLVMBuildCast(build, LLVMZExt, val, type, "texp_cast");
	else
		return LLVMBuildCast(build, (LLVMOpcode)node->cast_type, val, type, "texp_cast");
}

LLVMValueRef il_create_cast(cast_node_t* node)
{
	assert(node);

	return il_create_ast(node->node);
}

LLVMValueRef il_create_var_decl(var_decl_node_t* node)
{
	assert(node);

	LLVMTypeRef type = cast_llvm_from_resolved(con, cast_drop_ptr(node->type->r_type, false));
	LLVMValueRef alloc;

	if (trait_is_arr(node->type->r_type))
		alloc = LLVMBuildArrayAlloca(build, type, LLVMGetUndef(LLVMGetElementType(type)), "array");
	else
		alloc = LLVMBuildAlloca(build, type, node->name->str);

	var_scope_add(sc, var_new(node->name->str, alloc));
	return alloc;
}

LLVMValueRef il_create_fun_decl(fun_decl_node_t* node)
{
	assert(node);
	LLVMContextRef con = LLVMGetModuleContext(mod);

	// HEAD
	size_t args_s = var_decl_vec_size(node->args);
	LLVMTypeRef* args_t = malloc(sizeof(LLVMTypeRef) *args_s);
	LLVMTypeRef ret_t;

	for (size_t	i = 0; i < args_s; ++i)
	{
		var_decl_node_t* var_decl = var_decl_vec_at(node->args, i);
		CHECK(args_t[i] = il_create_type(var_decl->type), ASSERT,);
	}
	ret_t = il_create_type(node->type);

	LLVMTypeRef fun_t = LLVMFunctionType(ret_t, args_t, args_s, 0);
	LLVMValueRef fun = LLVMAddFunction(mod, node->name->str, fun_t);

	if (node->has_implementation)
	{// BODY
		var_scope_enter(sc);
		LLVMBasicBlockRef block = LLVMAppendBasicBlockInContext(con, fun, "entry");
		LLVMPositionBuilderAtEnd(build, block);

		for	(size_t i = 0; i < args_s; ++i)
		{
			LLVMValueRef arg = LLVMGetParam(fun, i);
			var_decl_node_t* arg_decl = var_decl_vec_at(node->args, i);
			LLVMTypeRef type = cast_llvm_from_resolved(con, arg_decl->type->r_type);
			LLVMValueRef alloc = LLVMBuildAlloca(build, type, "arg");

			LLVMBuildStore(build, arg, alloc);
			var_scope_add(sc, var_new(arg_decl->name->str, alloc));
		}

		LLVMBasicBlockRef body = il_create_block(node->code);
		if (! body)
		{
			LLVMDeleteFunction(fun);
			return NULL;
		}

		free(args_t);
		var_scope_leave(sc);
	}
	else
		LLVMAddFunctionAttr(fun, LLVMExternalLinkage);

	return fun;
}

LLVMValueRef il_create_fun_call(fun_call_node_t* node)
{
	assert(node);

	size_t s = ast_vec_size(node->args);
	LLVMValueRef fun = LLVMGetNamedFunction(mod, node->name->str);
	LLVMValueRef* args = (LLVMValueRef*)malloc(sizeof(LLVMValueRef) *s);
	assert(fun && args);

	for (size_t i = 0; i < s; ++i)
		assert(args[i] = il_create_ast(ast_vec_at(node->args, i)));

	LLVMValueRef ret = LLVMBuildCall(build, fun, args, s, "call");
	free(args);
	return ret;
}

LLVMValueRef il_create_if(if_node_t* node)
{
	assert(node);

	LLVMValueRef cond = il_create_ast(node->cond);
	assert(cond);

	LLVMBasicBlockRef true_block, false_block, end_block;
	{
		LLVMValueRef function = LLVMGetBasicBlockParent(LLVMGetInsertBlock(build));
		true_block = LLVMAppendBasicBlockInContext(con, function, "if_true");
		false_block = LLVMAppendBasicBlockInContext(con, function, "if_false");
		end_block = LLVMAppendBasicBlockInContext(con, function, "if_resume");
	}

	LLVMValueRef cmp = LLVMBuildIsNotNull(build, cond, "if");
	LLVMValueRef ret = LLVMBuildCondBr(build, cmp, true_block, false_block);
	assert(true_block && false_block && end_block && cmp && ret);

	{
		LLVMPositionBuilderAtEnd(build, true_block);
		true_block = il_create_block(node->true_node);					// required
		LLVMBuildBr(build, end_block);
	}{
		LLVMPositionBuilderAtEnd(build, false_block);
		false_block = il_create_block(node->else_node);					// required
		LLVMBuildBr(build, end_block);
	}

	LLVMPositionBuilderAtEnd(build, end_block);
	return ret;
}

LLVMValueRef il_create_while(while_node_t* node)
{
	assert(node);

	LLVMBasicBlockRef cond_block, true_block, end_block;
	{
		LLVMValueRef fun = LLVMGetBasicBlockParent(LLVMGetInsertBlock(build));
		cond_block	= LLVMAppendBasicBlockInContext(con, fun, "while_cond");
		true_block	= LLVMAppendBasicBlockInContext(con, fun, "while_true");
		end_block	= LLVMAppendBasicBlockInContext(con, fun, "while_resume");
	}

	LLVMBuildBr(build, cond_block);
	{
		LLVMPositionBuilderAtEnd(build, cond_block);

		LLVMValueRef cond = il_create_ast(node->cond);
		LLVMValueRef cmp = LLVMBuildIsNotNull(build, cond, "while");
		LLVMValueRef ret = LLVMBuildCondBr(build, cmp, true_block, end_block);
		assert(true_block && end_block && cmp && ret);
	}{
		LLVMPositionBuilderAtEnd(build, true_block);

		true_block = il_create_block(node->true_node);
		LLVMBuildBr(build, cond_block);
	}

	LLVMPositionBuilderAtEnd(build, end_block);
	return (LLVMValueRef)1;
}

LLVMValueRef il_create_for(for_node_t* node)
{
	assert(node);

	LLVMBasicBlockRef init_block, cond_block, inc_block, true_block, end_block;
	{
		LLVMValueRef fun = LLVMGetBasicBlockParent(LLVMGetInsertBlock(build));
		init_block	= LLVMAppendBasicBlockInContext(con, fun, "for_init");
		cond_block	= LLVMAppendBasicBlockInContext(con, fun, "for_cond");
		inc_block	= LLVMAppendBasicBlockInContext(con, fun, "for_inc");
		true_block	= LLVMAppendBasicBlockInContext(con, fun, "for_true");
		end_block	= LLVMAppendBasicBlockInContext(con, fun, "for_resume");
	}

	LLVMBuildBr(build, init_block);
	{
		LLVMPositionBuilderAtEnd(build, init_block);

		CHECK(il_create_ast(node->init), ASSERT,);
		LLVMBuildBr(build, cond_block);
	}{
		LLVMPositionBuilderAtEnd(build, cond_block);

		LLVMValueRef cond = il_create_ast(node->cond);
		LLVMValueRef cmp = LLVMBuildIsNotNull(build, cond, "for");
		LLVMBuildCondBr(build, cmp, true_block, end_block);
	}{
		LLVMPositionBuilderAtEnd(build, true_block);

		CHECK(true_block = il_create_block(node->block), ASSERT,);
		LLVMBuildBr(build, inc_block);
	}{
		LLVMPositionBuilderAtEnd(build, inc_block);

		CHECK(il_create_ast(node->inc), ASSERT,);
		LLVMBuildBr(build, cond_block);
	}

	LLVMPositionBuilderAtEnd(build, end_block);
	return (LLVMValueRef)1;
}
