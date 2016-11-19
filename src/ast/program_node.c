#include "program_node.h"
#include "../vector.h"

#include <stdio.h>

ast_ptr program_new()
{
	return new(ast, AST_PROGRAM, program_new_ng());
}

program_node_t* program_new_ng()
{
	define_ptr(program_node_t, ret);

	ret->v = new_ng(ast_vector, VEC_START_SIZE);

	return ret;
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
