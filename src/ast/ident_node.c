#include "ident_node.h"

#include <stdio.h>
#include <string.h>

extern char* strdup(const char *s);		// this is POSIX, not standard C
ast_ptr ident_new(char* str, bool should_copy)
{
	return new(ast, AST_IDENT, ident_new_ng(str, should_copy));
}

ident_node_t* ident_new_ng(char* str, bool should_copy)
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

	return ret;
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
