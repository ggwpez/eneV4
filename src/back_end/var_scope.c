#include "var_scope.h"

void var_scope_enter(var_scope_t* sc)
{
	assert(sc);

	var_scope_entry_t* entry = var_scope_entry_new();
	var_scope_entry_vector_push_back(sc->scopes, entry);
}

void var_scope_leave(var_scope_t* sc)
{
	assert(sc);
	assert(var_scope_entry_vector_size(sc->scopes));

	var_scope_entry_vector_pop_back(sc->scopes);
}

void var_scope_add(var_scope_t* sc, var_t* var)
{
	assert(sc);
	assert(var);
	assert(! var_scope_get(sc, var->name));

	var_scope_entry_t* entry = var_scope_entry_vector_rat(sc->scopes, 0);
	var_scope_entry_add(entry, var);
}

var_t* var_scope_get(var_scope_t* sc, char const* name)
{
	assert(sc);
	assert(name);

	size_t s = var_scope_entry_vector_size(sc->scopes);

	for (size_t i = 0; i < s; ++i)
	{
		var_scope_entry_t* entry = var_scope_entry_vector_at(sc->scopes, i);
		var_t* var = var_scope_entry_get(entry, name);

		if (var)
			return var;
	}

	return NULL;
}
