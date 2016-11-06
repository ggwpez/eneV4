#include "var_decl_node.h"
#include "../defines.h"

#include <stdio.h>

ast_ptr var_decl_new(type_node_t* type, ident_node_t* name)
{
	assert(type);
	assert(name);

	define_ptr(var_decl_node_t, ret);

	ret->type = type;
	ret->name = name;

	return new(ast, AST_VAR_DECL, ret);
}

void var_decl_del(var_decl_node_t* node)
{
	assert(node);

	type_del(node->type);		// TODO look here, if not deleted in scope
	ident_del(node->name);
}

void var_decl_print(var_decl_node_t* node)
{
	assert(node);

	printf("<var_decl(");
		type_print(node->type), putchar(','), ident_print(node->name),
	printf(")>");
}
