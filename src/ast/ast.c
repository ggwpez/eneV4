#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

ast_ptr ast_new(ast_type_t t, void* node)
{
	assert(node);
	assert((int)t < AST_size);
	ast_ptr ret = (ast_ptr)malloc(sizeof(ast_t));

	ret->t = t;
	ret->node = node;

	return ret;
}

typedef void(*dtor_ptr)(void*);

dtor_ptr ast_dtors[] = { (dtor_ptr)atom_del, (dtor_ptr)unop_del, (dtor_ptr)binop_del,
						 (dtor_ptr)program_del, (dtor_ptr)block_del, (dtor_ptr)return_del,
						 (dtor_ptr)ident_del, (dtor_ptr)type_del, (dtor_ptr)texp_del, (dtor_ptr)cast_del,
						 (dtor_ptr)var_decl_del, (dtor_ptr)fun_decl_del, (dtor_ptr)fun_call_del,
						 (dtor_ptr)if_del, (dtor_ptr)while_del, (dtor_ptr)for_del };
static_assert(_countof(ast_dtors) == AST_size, "ast_dtors invalid");

void ast_del(ast_ptr val)
{
	assert(val);
	assert((unsigned)val->t < AST_size);

	ast_dtors[(int)val->t](val->node);
	val->t = AST_size;

	free(val);
}

typedef void(*print_ptr)(void*);
print_ptr ast_prints[] = { (print_ptr)atom_print, (print_ptr)unop_print, (print_ptr)binop_print,
						   (print_ptr)program_print, (print_ptr)block_print, (print_ptr)return_print,
						   (print_ptr)ident_print, (print_ptr)type_print, (print_ptr)texp_print, (print_ptr)cast_print,
						   (print_ptr)var_decl_print, (print_ptr)fun_decl_print, (print_ptr)fun_call_print,
						   (print_ptr)if_print, (print_ptr)while_print, (print_ptr)for_print };
static_assert(_countof(ast_prints) == AST_size, "ast_prints invalid");

int ast_print(ast_ptr val)
{
	assert(val);
	assert((unsigned)val->t < AST_size);

	ast_prints[(int)val->t](val->node);
	return 0;			// <- just that i can use it in ternary
}
