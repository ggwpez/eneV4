#include "scope.h"

scope_t* scope_new()
{
	define_ptr(scope_t, ret);
	context_vector_t* contexts = new_ng(context_vector, 0);

	ret->contexts = contexts;

	return ret;
}

void scope_del(scope_t* obj)
{
	assert(obj);

	delete(context_vector, obj->contexts);
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

	context_vector_push_back(obj->contexts, context_new(ast_vector_new_ng(0)));        // add new context
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

int scope_add_fun(scope_t* obj, fun_decl_node_t* elem)
{
	assert(obj);
	assert(elem);
	size_t l = context_vector_size(obj->contexts);
	assert(l);

	context_t* cont	= context_vector_rat(obj->contexts, l -1);

	return context_add_fun(cont, elem);
}

int scope_add_var(scope_t* obj, var_decl_node_t* elem)
{
	assert(obj);
	assert(elem);
	size_t l = context_vector_size(obj->contexts);
	assert(l);

	context_t* cont	= context_vector_rat(obj->contexts, l -1);

	return context_add_var(cont, elem);
}
