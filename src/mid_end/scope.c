#include "scope.h"

scope_t* scope_new()
{
	define_ptr(scope_t, ret);
	context_vec_t* contexts = context_vec_new_ng(0);

	ret->contexts = contexts;

	return ret;
}

void scope_del(scope_t* obj)
{
	assert(obj);
	assert(! context_vec_size(obj->contexts));		// check whether all contexts are left

	context_vec_del(obj->contexts);
	free(obj);
}

void scope_print(scope_t* obj)
{
	assert(obj);

	printf("<scope("),
		context_vec_print(obj->contexts),
	printf(")>");
}

void scope_enter(scope_t* obj/*char const* namespace_name*/)
{
	assert(obj);
	assert(context_vec_push_back(obj->contexts, context_new()));        // add new context TODO this is lazy
}

void scope_leave(scope_t* obj)
{
	assert(obj);
	context_del(context_vec_pop_back(obj->contexts));
}

fun_decl_node_t* scope_get_fun(scope_t* obj, ident_node_t* name)
{
	assert(obj);
	assert(name);

	size_t l = context_vec_size(obj->contexts);
	size_t i = 0;

	while (l)
	{
		context_t* cont = context_vec_rat(obj->contexts, i++);
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

	size_t l = context_vec_size(obj->contexts);

	while (l)
	{
		context_t* cont = context_vec_at(obj->contexts, --l);
		var_decl_node_t* ret = context_get_var(cont, name);

		if (ret)
			return ret;
	}

	return NULL;
}

static_assert((int)UR_TYPE_MOD_size == (int)R_TYPE_MOD_size, "(int)UR_TYPE_MOD_size == (int)R_TYPE_MOD_size");
r_type_t* scope_resolve_ur_type(scope_t* obj, ur_type_t* type)
{
	assert(obj);
	assert(type);
	assert(type->mod < UR_TYPE_MOD_size);

	if (type->mod == UR_TYPE_MOD_PTR || type->mod == UR_TYPE_MOD_CONST)
	{
		r_type_t* new_sub = scope_resolve_ur_type(obj, type->sub);

		if (! new_sub)
			return NULL;

		return r_type_new((r_type_mod_t)type->mod, NULL, new_sub, -1);		// this cast is assumed safe
	}
	else if (type->mod == UR_TYPE_MOD_ID)
	{
		inbuild_type_t* new_sub = inbuild_type_resolve(type->id);

		if (! new_sub)
			return NULL;

		return r_type_new(R_TYPE_MOD_INBUILD, new_sub, NULL, -1);
	}
	else if (type->mod == UR_TYPE_MOD_ARRAY)
	{
		r_type_t* new_sub = scope_resolve_ur_type(obj, type->sub);

		if (! new_sub)
			return NULL;

		return r_type_new((r_type_mod_t)type->mod, NULL, new_sub, type->arr_size);		// this cast is assumed safe
	}
	else
		PANIC;
	// Add here for struct types
}

r_type_t* scope_resolve_inbuild_str(scope_t* obj, char const* name)
{
	assert(obj);
	assert(name);

	inbuild_type_t* it = inbuild_type_resolve_str(name);

	if (! it) return NULL;
	return new(r_type, R_TYPE_MOD_INBUILD, it, NULL, -1);
}

error_t scope_transform_type(scope_t* obj, type_node_t* node)
{
	assert(node);
	assert(node->ur_type);
	assert(! node->r_type);

	r_type_t* new_res = scope_resolve_ur_type(obj, node->ur_type);

	if (! new_res)
		return SC_TRANSFORM_TYPE;

	node->r_type = new_res;
	return SUCCESS;
}

error_t scope_transform_var(scope_t* obj, var_decl_node_t* node)
{
	assert(obj);
	assert(node);

	CHECK_ERR(scope_transform_type(obj, node->type));
	return SUCCESS;
}

error_t scope_transform_vars(scope_t* obj, var_decl_vec_t* nodes)
{
	assert(nodes);

	size_t s = var_decl_vec_size(nodes);

	for (size_t i = 0; i < s; ++i)
	{
		var_decl_node_t* node = var_decl_vec_at(nodes, i);
		CHECK_ERR(scope_transform_var(obj, node));
	}

	return SUCCESS;
}

error_t scope_add_vars(scope_t* obj, var_decl_vec_t* nodes)
{
	assert(obj);
	assert(nodes);

	size_t s = var_decl_vec_size(nodes);

	for (size_t i = 0; i < s; ++i)
	{
		var_decl_node_t* var = var_decl_vec_at(nodes, i);
		CHECK_ERR(scope_add_var(obj, var));
	}

	return SUCCESS;
}

error_t scope_add_fun(scope_t* obj, fun_decl_node_t* elem)
{
	assert(obj);
	assert(elem);
	assert(context_vec_size(obj->contexts));

	if (scope_get_fun(obj, elem->name))
		return CON_DOUBLE;
	else if (UNIQUE_IDENTS && scope_get_var(obj, elem->name))
		return CON_FUN_REG_AS_VAR;

	context_t* cont	= context_vec_rat(obj->contexts, 0);

	return context_add_fun(cont, elem);
}

error_t scope_add_var(scope_t* obj, var_decl_node_t* elem)
{
	assert(obj);
	assert(elem);
	assert(context_vec_size(obj->contexts));

	if (scope_get_var(obj, elem->name))
		return CON_DOUBLE;
	else if (UNIQUE_IDENTS && scope_get_fun(obj, elem->name))
		return CON_VAR_REG_AS_FUN;

	context_t* cont	= context_vec_rat(obj->contexts, 0);

	return context_add_var(cont, elem);
}
