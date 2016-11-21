#include "var_scope_entry.h"

var_scope_entry_t* var_scope_entry_new()
{
	define_ptr(var_scope_entry_t, ret);

	ret->vars = var_vector_new_ng(0);

	return ret;
}

void var_scope_entry_add(var_scope_entry_t* sc, var_t* var)
{
	assert(var);
	var_vector_push_back(sc->vars, var);
}

var_t* var_scope_entry_get(var_scope_entry_t* sc, char const* name)
{
	assert(sc);
	assert(name);

	size_t s = var_vector_size(sc->vars);

	for (size_t i = 0; i < s; ++i)
	{
		var_t* var = var_vector_at(sc->vars, i);
		if (! strcmp(name, var->name))
			return var;
	}

	return NULL;
}

void var_scope_entry_del(var_scope_entry_t* sc)
{
	assert(sc);

	var_vector_del(sc->vars);
	free(sc);
}

void var_scope_entry_print(var_scope_entry_t* sc)
{
	assert(sc);

	printf("<var_scope_entry("),
		var_vector_print(sc->vars),
	printf(")>");
}
