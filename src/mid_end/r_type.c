#include "r_type.h"
#include <stdio.h>

r_type_t* r_type_new(r_type_mod_t mod, inbuild_type_t* inbuild, r_type_t* sub)
{
	assert(mod < R_TYPE_MOD_size);
	if (inbuild) assert(! sub);
	if (sub) assert(! inbuild);
	define_ptr(r_type_t, ret);

	ret->mod = mod;
	if (mod == R_TYPE_MOD_INBUILD)
		ret->inbuild = inbuild, ret->sub = NULL;
	else
		ret->inbuild = NULL, ret->sub = sub;

	return ret;
}

r_type_t* r_type_cpy(r_type_t* obj)
{
	assert(obj);
	assert(obj->mod < R_TYPE_MOD_size);
	define_ptr(r_type_t, ret);
	ret->mod = obj->mod;

	if (ret->mod == R_TYPE_MOD_INBUILD)
	{
		ret->inbuild = obj->inbuild;
		ret->sub = NULL;
	}
	else
	{
		ret->inbuild = NULL;
		ret->sub = r_type_cpy(obj->sub);
	}

	return ret;
}

void r_type_del(r_type_t* node)
{
	assert(node);
	assert(node->mod < R_TYPE_MOD_size);

	if (node->mod == R_TYPE_MOD_INBUILD)
		inbuild_type_del(node->inbuild);
	else
		r_type_del(node->sub);

	free(node);
}

void r_type_print(r_type_t* node)
{
	assert(node);
	assert(node->mod < R_TYPE_MOD_size);

	printf("<r_type(");
	if (node->mod != R_TYPE_MOD_INBUILD)
	{
		if (node->mod == R_TYPE_MOD_CONST)
			printf("const ");
		else
			printf("ptr ");

		r_type_print(node->sub);
	}
	else
		inbuild_type_print(node->inbuild);
	printf(")>");
}

void r_type_pprint(r_type_t* node)
{
	assert(node);
	assert(node->mod < R_TYPE_MOD_size);

	if (node->mod != R_TYPE_MOD_INBUILD)
	{
		if (node->mod == R_TYPE_MOD_CONST)
		{
			r_type_pprint(node->sub);
			printf("const ");
		}
		else
		{
			r_type_pprint(node->sub);
			putchar('\'');
		}
	}
	else
		inbuild_type_pprint(node->inbuild);
}
