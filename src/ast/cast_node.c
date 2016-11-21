#include "cast_node.h"
#include <stdio.h>

ast_ptr cast_new(ast_ptr node, ur_type_t* t)
{
	define_ptr(cast_node_t, ret);

	ret->node = node;
	ret->t = t;

	return new(ast, AST_CAST, ret);
}

void cast_del(cast_node_t* node)
{
	delete(ast, node->node);
	delete(ur_type, node->t);
	free(node);
}

void cast_print(cast_node_t* node)
{
	printf("<cast("),
		ast_print(node->node), putchar(','),
		ur_type_print(node->t),
	printf(")>");
}
