#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

MAKE_ENUM_STRINGS(ast_type,	AST_TYPE_ENUM)

ast_ptr ast_new(ast_type_t t, void* node)
{
	assert(node);
	assert((int)t < AST_size);
	ast_ptr ret = (ast_ptr)malloc(sizeof(ast_t));

	ret->t = t;
	ret->node = node;

	return ret;
}

typedef void(*dtor_ptr)(void*);
MAKE_VTABLE_C(ast_dtors, ,_del, dtor_ptr)
typedef void(*print_ptr)(void*);
MAKE_VTABLE_C(ast_prints, , _print, print_ptr)

void ast_del(ast_ptr val)
{
	assert(val);
	assert((unsigned)val->t < AST_size);

	ast_dtors[(int)val->t](val->node);
	val->t = AST_size;

	free(val);
}

int ast_print(ast_ptr val)
{
	assert(val);
	assert((unsigned)val->t < AST_size);

	ast_prints[(int)val->t](val->node);
	return 0;			// <- just that i can use it in ternary
}
