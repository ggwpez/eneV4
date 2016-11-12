#include "fun_decl.h"
#include <stdio.h>

ast_ptr fun_decl_new(type_node_t* type, ident_node_t* name, block_node_t* code, var_decl_vector_t* args)
{
	assert(type);
	assert(name);
	assert(code);
	assert(args);
	define_ptr(fun_decl_node_t, ret);

	ret->type = type;
	ret->name = name;
	ret->code = code;
	ret->args = args;

	return new(ast, AST_FUN_DECL, ret);
}

void fun_decl_del(fun_decl_node_t* node)
{
	assert(node);

	type_del(node->type);
	ident_del(node->name);
	block_del(node->code);
	var_decl_vector_del(node->args);

	free(node);
}

void fun_decl_print(fun_decl_node_t* node)
{
	assert(node);

	printf("<fun_decl("),
		type_print(node->type), putchar(','),
		ident_print(node->name), putchar(','),
		var_decl_vector_print(node->args), putchar(','),
		block_print(node->code),
	printf(")>");
}
