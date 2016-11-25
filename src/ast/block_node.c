#include "block_node.h"
#include "../vector.h"

#include <stdio.h>

ast_ptr block_new()
{
	return new(ast, AST_BLOCK, block_new_ng());
}

block_node_t* block_new_ng()
{
	define_ptr(block_node_t, ret);

	ret->v = new_ng(ast_vec, VEC_START_SIZE);

	return ret;
}

void block_del(block_node_t* node)
{
	assert(node);
	assert(node->v);

	delete(ast_vec, node->v);
	free(node);
}

void block_print(block_node_t* node)
{
	assert(node);
	assert(node->v);

	printf("<block("),
		ast_vec_print(node->v),
	printf(")>");
}
