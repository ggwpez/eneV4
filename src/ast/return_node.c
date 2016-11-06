#include "return_node.h"
#include <stdio.h>

ast_ptr return_new(ast_ptr exp)
{
	assert(exp);
	define_ptr(return_node_t, ret);

	ret->exp = exp;

	return new(ast, AST_RETURN, ret);
}

void return_del(return_node_t* node)
{
	assert(node);

	delete(ast, node->exp);
	free(node);
}

void return_print(return_node_t* node)
{
	assert(node);

	printf("<return("),
		ast_print(node->exp),
	printf(")>");
}
