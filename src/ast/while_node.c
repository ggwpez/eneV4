#include "while_node.h"
#include <stdio.h>

ast_ptr while_new(ast_ptr cond, ast_ptr true_node)
{
	assert(cond);
	malloc_ptr(while_node_t, ret);

	ret->cond = cond;
	ret->true_node = true_node;

	return new(ast, AST_WHILE, ret);
}

void while_del(while_node_t* node)
{
	assert(node);
	assert(node->cond);

	delete(ast, node->cond);
	if (node->true_node)
		delete(ast, node->true_node);

	free(node);
}

void while_print(while_node_t* node)
{
	assert(node);

	printf("<while("), ast_print(node->cond);
	putchar(','), (node->true_node) ? ast_print(node->true_node) : printf("<null>");
	printf(")>");
}
