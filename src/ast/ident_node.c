#include "ident_node.h"
#include "../defines.h"

#include <stdio.h>
#include <string.h>

ast_ptr ident_new(char* str, bool should_free)
{
	return new(ast, AST_IDENT, ident_new_ng(str, should_free));
}

ident_node_t* ident_new_ng(char* str, bool should_free)
{
	assert(str);
	define_ptr(ident_node_t, ret);

	ret->str = str;
	ret->should_free = should_free;

	return ret;
}

void ident_del(ident_node_t* node)
{
	assert(node);
	assert(node->str);

	if (node->should_free)
		free(node->str);

	free(node);
}

void ident_print(ident_node_t* node)
{
	assert(node);
	assert(node->str);

	printf("<ident(%s)>", node->str);
}

int ident_cmp(ident_node_t* node, ident_node_t* y)
{
	assert(y);

	return ident_cmp_str(node, y->str);
}

int ident_cmp_str(ident_node_t* node, char const* str)
{
	assert(node);

	return ident_cmp_str_str(node->str, str);
}

int ident_cmp_str_str(char const* str1, char const* str2)
{
	assert(str1);
	assert(str2);

	return strcmp(str1, str2);
}
