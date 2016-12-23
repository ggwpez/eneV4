#include "binop_node.h"
#include <stdio.h>

MAKE_ENUM_STRINGS(binop, BINOP_ENUM)

ast_ptr binop_new(binop_t t, ast_ptr x, ast_ptr y)
{
	define_ptr(binop_node_t, ret);

	ret->t = t;
	ret->x = x;
	ret->y = y;

	return new(ast, AST_BINOP, (ast_ptr)ret);
}

void binop_del(binop_node_t* node)
{
	assert(node);

	delete(ast, node->x);
	delete(ast, node->y);
	node->t = BINOP_size;

	free(node);
}

void binop_print(binop_node_t* node)
{
	printf("<binop("), ast_print(node->x), putchar(','),
			node->x == node->y ? printf("<&top>") : ast_print(node->y),
	printf(",%s)>", binop_strings[(int)node->t]);
}
