#include "program_node.h"
#include "../vector.h"

#include <stdio.h>

ast_ptr program_new()
{
	define_ptr(program_node_t, ret);

	ret->v = new_ng(ast_vector, AST_VEC_START_SIZE);

	return new(ast, AST_PROGRAM, (void*)ret);
}

void program_del(program_node_t* node)
{
	assert(node);
	assert(node->v);

	delete(ast_vector, node->v);
	free(node);
}

void program_print(program_node_t* node)
{
	assert(node);
	assert(node->v);

	printf("<program(");
	ast_vector_print(node->v);
	printf(")>");
}
