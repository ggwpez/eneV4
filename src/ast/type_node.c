#include "type_node.h"
#include <stdio.h>
#undef mode

ur_type_t* ur_type_new_ng(ur_type_mod_t mod, ident_node_t* id, ur_type_t* sub)
{
	assert(mod < UR_TYPE_MOD_size);
	define_ptr(ur_type_t, ret);

	ret->mod = mod;

	if (mod == UR_TYPE_MOD_ID)
	{
		assert(id);
		ret->id = id;
	}
	else
	{
		assert(sub);
		ret->sub = sub;
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
		else
			printf("ptr ");

		ur_type_print(node->sub);
	}
	else
		ident_print(node->id);

	printf(")>");
}

ast_ptr type_new(ur_type_t* type)
{
	return new(ast, AST_TYPE, type_new_ng(type));
}

type_node_t* type_new_ng(ur_type_t* type)
{
	assert(type);
	define_ptr(type_node_t, ret);

	ret->type = type;

	return ret;
}

void type_del(type_node_t* node)
{
	assert(node);

	delete(ur_type, node->type);
}

void type_print(type_node_t* node)
{
	assert(node);

	printf("<type_node(");
		ur_type_print(node->type);
	printf(")>");
}
