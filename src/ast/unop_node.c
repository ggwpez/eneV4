#include "unop_node.h"
#include <stdio.h>

ast_ptr unop_new(unop_t t, ast_ptr node)
{
	define_ptr(unop_node_t, ret);

	ret->t = t;
	ret->node = node;

	return new(ast, AST_UNOP, (void*)ret);
}

void unop_del(unop_node_t* node)
{
	assert(node);

	delete(ast, node->node);
	free(node);
}

char const* unop_strings[] = { "!", "^", "$", "~" };
static_assert(_countof(unop_strings) == UNOP_size, "unnop_strings invalid");
void unop_print(unop_node_t* node)
{
	printf("<unop("), node->node == NULL ? printf("<&top>") : ast_print(node->node), printf(",%s)>", unop_strings[(int)node->t]);
}
