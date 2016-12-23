#include "ur_type.h"
#include <stdio.h>

ur_type_t* ur_type_new_ng(ur_type_mod_t mod, ident_node_t* id, ur_type_t* sub, size_t elements)
{
	assert(mod < UR_TYPE_MOD_size);
	define_ptr(ur_type_t, ret);

	ret->mod = mod;

	if (mod == UR_TYPE_MOD_ID)
	{
		assert(id);
		ret->id = id;
		ret->sub = NULL;
	}
	else if (mod == UR_TYPE_MOD_CONST || mod == UR_TYPE_MOD_PTR)
	{
		assert(sub);
		ret->id = NULL;
		ret->sub = sub;
	}
	else
	{
		ret->id = NULL;
		ret->sub = sub;
		ret->arr_size = elements;
	}

	return ret;
}

void ur_type_del(ur_type_t* node)
{
	assert(node);
	assert(node->mod < UR_TYPE_MOD_size);

	if (node->sub)
		delete(ur_type, node->sub);
	else
		delete(ident, node->id);

	free(node);
}

void ur_type_print(ur_type_t* node)
{
	assert(node);
	assert(node->mod < UR_TYPE_MOD_size);

	printf("<ur_type(");
	if (node->mod != UR_TYPE_MOD_ID)
	{
		if (node->mod == UR_TYPE_MOD_CONST)
			printf("const ");
		else if (node->mod == UR_TYPE_MOD_PTR)
			printf("ptr ");
		else
			printf("[%zu]", node->arr_size);

		ur_type_print(node->sub);
	}
	else
		ident_print(node->id);

	printf(")>");
}
