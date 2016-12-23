#include "r_type.h"
#include <stdio.h>

r_type_t* r_type_new(r_type_mod_t mod, inbuild_type_t* type, r_type_t* sub, size_t elements)
{
	assert(mod < R_TYPE_MOD_size);
	if (type) assert(! sub);
	if (sub) assert(! type);
	define_ptr(r_type_t, ret);

	ret->mod = mod;
	if (mod == R_TYPE_MOD_INBUILD)
		ret->inbuild = type, ret->sub = NULL, ret->arr_size = -1;
	else if (mod == R_TYPE_MOD_CONST || mod == R_TYPE_MOD_PTR)
		ret->inbuild = NULL, ret->sub = sub, ret->arr_size = -1;
	else if (mod == R_TYPE_MOD_ARRAY)
		ret->inbuild = NULL, ret->sub = sub, ret->arr_size = elements;
	else
		PANIC

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
	else if (ret->mod == R_TYPE_MOD_CONST || ret->mod == R_TYPE_MOD_PTR)
	{
		ret->inbuild = NULL;
		ret->sub = r_type_cpy(obj->sub);
	}
	else if (ret->mod == R_TYPE_MOD_ARRAY)
	{
		ret->inbuild = NULL;
		ret->arr_size = obj->arr_size;
		ret->sub = r_type_cpy(obj->sub);
	}
	else
		PANIC

	return ret;
}

void r_type_del(r_type_t* node)
{
	assert(node);
	assert(node->mod < R_TYPE_MOD_size);

	if (node->mod == R_TYPE_MOD_INBUILD)
		inbuild_type_del(node->inbuild);
	else if (node->mod == R_TYPE_MOD_CONST || node->mod == R_TYPE_MOD_PTR)
		r_type_del(node->sub);
	else if (node->mod == R_TYPE_MOD_ARRAY)
		r_type_del(node->sub);
	else
		PANIC

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
		else if (node->mod == R_TYPE_MOD_PTR)
			printf("ptr ");
		else if (node->mod == R_TYPE_MOD_ARRAY)
			printf("array ");
		else
			PANIC

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
		else if (node->mod == R_TYPE_MOD_PTR)
		{
			r_type_pprint(node->sub);
			putchar('\'');
		}
		else if (node->mod == R_TYPE_MOD_ARRAY)
		{
			r_type_pprint(node->sub);
			printf("[%zu]", node->arr_size);
		}
		else
			PANIC
	}
	else
		inbuild_type_pprint(node->inbuild);
}
