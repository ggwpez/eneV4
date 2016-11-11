#include "atom_node.h"
#include <stdio.h>

ast_ptr atom_new(atom_t t, void const* value)
{
	define_ptr(atom_node_t, ret);
	ret->t = t;

	if (t == ATOM_INT)
		ret->v0 = *(int*)value;
	else if (t == ATOM_FLOAT)
		ret->v1 = *(float*)value;

	return new(ast, AST_ATOM, (void*)ret);
}

void atom_del(atom_node_t* val)
{
	assert(val);

	val->t = ATOM_size;

	free(val);
}

void atom_print(atom_node_t* val)
{
	assert(val);

	if (val->t == ATOM_INT)
		printf("<int=%i>", val->v0);
	else if (val->t == ATOM_FLOAT)
		printf("<float=%f>", val->v1);
	else if (val->t == ATOM_REF_TO_RES)
		printf("<&top>");
}
