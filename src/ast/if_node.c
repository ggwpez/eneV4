#include "if_node.h"
#include <stdio.h>

ast_ptr if_new(ast_ptr cond, block_node_t* true_node, block_node_t* else_node)
{
	assert(cond);
	define_ptr(if_node_t, ret);

	ret->cond = cond;
	ret->true_node = true_node;
	ret->else_node = else_node;

	return new(ast, AST_IF, (void*)ret);
}

void if_del(if_node_t* node)
{
	assert(node);

	delete(ast, node->cond);
	if (node->true_node)
		block_del(node->true_node);
	if (node->else_node)
		block_del(node->else_node);

	free(node);
}

void if_print(if_node_t* node)
{
	assert(node);

	printf("<if("), ast_print(node->cond),
		putchar(','), block_print(node->true_node),
		putchar(','), block_print(node->else_node),
	printf(")>");
}
