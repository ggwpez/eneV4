#include "for_node.h"

#include <stdio.h>

ast_ptr for_new(ast_ptr init, ast_ptr cond, ast_ptr inc, block_node_t* block)
{
	assert(block);
	define_ptr(for_node_t, ret);

	ret->init = init;
	ret->cond = cond;
	ret->inc = inc;
	ret->block = block;

	return new(ast, AST_FOR, ret);
}

void for_del(for_node_t* node)
{
	assert(node);

	delete(ast, node->init);
	delete(ast, node->cond);
	delete(ast, node->inc);
	block_del(node->block);

	free(node);
}

void for_print(for_node_t* node)
{
	assert(node);

	printf("<for("),
		node->init ? ast_print(node->init) : printf("<null>"),
		putchar(','), node->cond ? ast_print(node->cond) : printf("<null>"),
		putchar(','), node->inc ? ast_print(node->inc) : printf("<null>"),
		block_print(node->block),
	printf(")>");
}
