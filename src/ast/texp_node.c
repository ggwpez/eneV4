#include "texp_node.h"
#include <stdio.h>

MAKE_ENUM_STRINGS(texp_cast, CAST_ENUM)

ast_ptr texp_new(ast_ptr exp, r_type_t* type)
{
	return new(ast, AST_TEXP, texp_new_ng(exp, type));
}

texp_node_t* texp_new_ng(ast_ptr exp, r_type_t* type)
{
	assert(exp);
	assert(type);
	define_ptr(texp_node_t, ret);

	ret->exp = exp;
	ret->type = type;
	ret->cast_type = CAST_size;

	return ret;
}

void texp_del(texp_node_t* node)
{
	assert(node);

	delete(ast, node->exp);
	r_type_del(node->type);
	free(node);
}

void texp_print(texp_node_t* node)
{
	assert(node);

	printf("<texp("),
		ast_print(node->exp), putchar(','),
		printf("%s", texp_cast_strings[(int)node->cast_type]),
		r_type_print(node->type),
	printf(")>");
}
