#include "ident_node.h"

#include <stdio.h>
#include <string.h>

ast_ptr ident_new(char const* str, bool should_copy)
{
	assert(str);
	define_ptr(ident_node_t, ret);

	if (should_copy)
	{
		ret->str = strdup(str);
		if (!str)
			return NULL;
		ret->have_to_free = true;
	}
	else
	{
		ret->str = str;
		ret->have_to_free = false;
	}

	return new(ast, AST_IDENT, ret);
}

void ident_del(ident_node_t* node)
{
	assert(node);
	assert(node->str);

	if (node->have_to_free)
		free(node->str);

	free(node);
}

void ident_print(ident_node_t* node)
{
	assert(node);
	assert(node->str);

	printf("<ident(%s)>", node->str);
}
