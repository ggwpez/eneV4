#include "context.h"

context_t* context_new(/*, char const* namespace_name*/)
{
	define_ptr(context_t, ret);
	fun_decl_vector_t* funs = fun_decl_vector_new_ng(0);
	var_decl_vector_t* vars = var_decl_vector_new_ng(0);

	ret->funs = funs;
	ret->vars = vars;

	return ret;
}

void context_del(context_t* obj)
{
	assert(obj);

	fun_decl_vector_free(obj->funs);	// only free, the nodes are in the tree, so DONT DELETE
	var_decl_vector_free(obj->vars);
	free(obj);
}

void context_print(context_t* obj)
{
	assert(obj);

	printf("<context("),
		fun_decl_vector_print(obj->funs), putchar(','),
		var_decl_vector_print(obj->vars),
	printf(")>");
}

fun_decl_node_t* context_get_fun(context_t* obj, ident_node_t* name)
{
	assert(obj);
	assert(name);

	size_t s = fun_decl_vector_size(obj->funs),
		   i;

	for (i = 0; i < s; ++i)					// TODO make this a rat, assuming that latest defined is first used
	{
		fun_decl_node_t* fun = fun_decl_vector_at(obj->funs, i);

		if (! ident_cmp(fun->name, name))
			return fun;
	}

	return NULL;
}

var_decl_node_t* context_get_var(context_t* obj, ident_node_t* name)
{
	assert(obj);
	assert(name);

	size_t s = var_decl_vector_size(obj->vars),
		   i;

	for (i = 0; i < s; ++i)					// TODO make this a rat, assuming that latest defined is first used
	{
		var_decl_node_t* var = var_decl_vector_at(obj->vars, i);

		if (! ident_cmp(var->name, name))
			return var;
	}

	return NULL;
}

error_t context_add_fun(context_t* obj, fun_decl_node_t* elem)
{
	assert(obj);
	assert(elem);
	assert(elem->type->r_type);

	if (! fun_decl_vector_push_back(obj->funs, elem))
		return CON_UNDERLYING;

	return SUCCESS;
}

error_t context_add_var(context_t* obj, var_decl_node_t* elem)
{
	assert(obj);
	assert(elem);
	assert(elem->type->r_type);

	if (! var_decl_vector_push_back(obj->vars, elem))
		return CON_UNDERLYING;

	return SUCCESS;
}
