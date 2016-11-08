#include "type_node.h"
#include <stdio.h>

ast_ptr type_new(ur_type_t* type)
{
	return new(ast, AST_TYPE, type_new_ng(type));
}

type_node_t* type_new_ng(ur_type_t* type)
{
	assert(type);
	define_ptr(type_node_t, ret);

	ret->ur_type = type;
	ret->r_type = NULL;

	return ret;
}

void type_del(type_node_t* node)
{
	assert(node);

	ur_type_del(node->ur_type);
	if (node->r_type)
		r_type_del(node->r_type);
}

void type_print(type_node_t* node)
{
	assert(node);

	printf("<type_node(");
		(node->r_type) ?
			r_type_print(node->r_type):
			ur_type_print(node->ur_type),
	printf(")>");
}
