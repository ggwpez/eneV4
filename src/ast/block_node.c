#include "block_node.h"
#include "../vector.h"

#include <stdio.h>

ast_ptr block_new()
{
	malloc_ptr(block_node_t, ret);

	ret->v = new(ast_vector, AST_VEC_START_SIZE);

	return new(ast, AST_BLOCK, (void*)ret);
}

void block_del(block_node_t* node)
{
	assert(node);
	assert(node->v);

	delete(ast_vector, node->v);
	free(node);
}

void block_print(block_node_t* node)
{
	assert(node);
	assert(node->v);

	printf("<block(");
	ast_vector_print(node->v);
	printf(")>");
}
