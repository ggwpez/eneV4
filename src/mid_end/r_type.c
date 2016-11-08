#include "r_type.h"
#include <stdio.h>

r_type_t* r_type_new(r_type_mode_t mod, inbuild_type_t* inbuild, r_type_t* sub)
{
	assert(mod < R_TYPE_MOD_size);
	if (inbuild) assert(! sub);
	if (sub) assert(! inbuild);
	define_ptr(r_type_t, ret);

	if (mod == R_TYPE_MOD_INBUILD)
		ret->inbuild = inbuild, ret->sub = NULL;
	else
		ret->inbuild = NULL, ret->sub = sub;

	return ret;
}

void r_type_del(r_type_t* node)
{
	assert(node);

	if (node->mod == R_TYPE_MOD_INBUILD)
		inbuild_type_del(node->inbuild);
	else
		r_type_del(node->sub);

	free(node);
}

void r_type_print(r_type_t* node)
{
	assert(node);

	printf("<r_type("),
		(node->mod == R_TYPE_MOD_INBUILD) ? inbuild_type_print(node->inbuild) :
									  r_type_print(node->sub),
	printf(")>");
}
