#include "scope.h"

scope_t* scope_new()
{
	define_ptr(scope_t, ret);
	context_vector_t* contexts = context_vector_new_ng(0);

	ret->contexts = contexts;

	return ret;
}

void scope_del(scope_t* obj)
{
	assert(obj);
	assert(! context_vector_size(obj->contexts));		// check whether all contexts are left

	context_vector_del(obj->contexts);
	free(obj);
}

void scope_print(scope_t* obj)
{
	assert(obj);

	printf("<scope("),
		context_vector_print(obj->contexts),
	printf(")>");
}

void scope_enter(scope_t* obj/*char const* namespace_name*/)
{
	assert(obj);

	context_vector_push_back(obj->contexts, context_new());        // add new context
}

void scope_leave(scope_t* obj)
{
	assert(obj);
}

fun_decl_node_t* scope_get_fun(scope_t* obj, ident_node_t* name)
{
	assert(obj);
	assert(name);

	size_t l = context_vector_size(obj->contexts);
	size_t i = 0;

	while (l)
	{
		context_t* cont = context_vector_rat(obj->contexts, i++);
		fun_decl_node_t* ret = context_get_fun(cont, name);

		if (ret)
			return ret;

		l--;
	}

	return NULL;
}

var_decl_node_t* scope_get_var(scope_t* obj, ident_node_t* name)
{
	assert(obj);
	assert(name);

	size_t l = context_vector_size(obj->contexts);
	size_t i = 0;

	while (l)
	{
		context_t* cont = context_vector_rat(obj->contexts, i++);
		var_decl_node_t* ret = context_get_var(cont, name);

		if (ret)
			return ret;

		l--;
	}

	return NULL;
}

static_assert((int)UR_TYPE_MOD_size == (int)R_TYPE_MOD_size, "(int)UR_TYPE_MOD_size == (int)R_TYPE_MOD_size");
r_type_t* scope_resolve_ur_type(scope_t* obj, ur_type_t* type)
{
	assert(obj);
	assert(type);
	assert(type->mod < UR_TYPE_MOD_size);

	if (type->mod == UR_TYPE_MOD_PTR|| type->mod == UR_TYPE_MOD_CONST)
	{
		r_type_t* new_sub = scope_resolve_ur_type(obj, type->sub);

		if (! new_sub)
			return NULL;

		return r_type_new((r_type_mod_t)type->mod, NULL, new_sub);		// this cast is assumed safe
	}
	else //if (type->mod == UR_TYPE_MOD_ID)
	{
		inbuild_type_t* new_sub = inbuild_type_resolve(type->id);

		if (! new_sub)
			return NULL;

		return r_type_new(R_TYPE_MOD_INBUILD, new_sub, NULL);
	}
	// Add here for struct types
}

r_type_t* scope_resolve_inbuild_str(scope_t* obj, char const* name)
{
	assert(obj);
	assert(name);

	inbuild_type_t* it = inbuild_type_resolve_str(name);

	if (! it) return NULL;
	return new(r_type, R_TYPE_MOD_INBUILD, it, NULL);
}

int scope_transform_type(scope_t* obj, type_node_t* node)
{
	assert(node);
	assert(node->ur_type);
	assert(! node->r_type);

	r_type_t* new_res = scope_resolve_ur_type(obj, node->ur_type);

	if (! new_res)
		return -1;

	node->r_type = new_res;
	return 0;
}

context_add_t scope_add_fun(scope_t* obj, fun_decl_node_t* elem)
{
	assert(obj);
	assert(elem);
	size_t l = context_vector_size(obj->contexts);
	assert(l);

	if (scope_get_fun(obj, elem->name))
		return CA_DOUBLE;
	else if (UNIQUE_IDENTS && scope_get_var(obj, elem->name))
		return CA_FUN_REG_AS_VAR;

	scope_transform_type(obj, elem->type);
	context_t* cont	= context_vector_rat(obj->contexts, l -1);

	return context_add_fun(cont, elem);
}

context_add_t scope_add_var(scope_t* obj, var_decl_node_t* elem)
{
	assert(obj);
	assert(elem);
	size_t l = context_vector_size(obj->contexts);
	assert(l);

	if (scope_get_var(obj, elem->name))
		return CA_DOUBLE;
	else if (UNIQUE_IDENTS && scope_get_fun(obj, elem->name))
		return CA_VAR_REG_AS_FUN;

	scope_transform_type(obj, elem->type);
	context_t* cont	= context_vector_rat(obj->contexts, l -1);

	return context_add_var(cont, elem);
}
