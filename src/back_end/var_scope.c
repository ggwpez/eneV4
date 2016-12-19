#include "var_scope.h"

var_scope_t* var_scope_new()
{
	define_ptr(var_scope_t, ret);

	ret->scopes = var_scope_entry_vec_new_ng(0);

	return ret;
}

void var_scope_del(var_scope_t* sc)
{
	assert(sc);
	assert(! var_scope_entry_vec_size(sc->scopes));	// You forgot to leave

	var_scope_entry_vec_del(sc->scopes);
	free(sc);
}

void var_scope_enter(var_scope_t* sc)
{
	assert(sc);

	var_scope_entry_t* entry = var_scope_entry_new();
	var_scope_entry_vec_push_back(sc->scopes, entry);
}

void var_scope_leave(var_scope_t* sc)
{
	assert(sc);
	assert(var_scope_entry_vec_size(sc->scopes));	// You forgot to enter

	var_scope_entry_del(var_scope_entry_vec_pop_back(sc->scopes));
}

void var_scope_add(var_scope_t* sc, var_t* var)
{
	assert(sc);
	assert(var);
	assert(var_scope_entry_vec_size(sc->scopes));	// You forgot to enter
	assert(! var_scope_get(sc, var->name));

	var_scope_entry_t* entry = var_scope_entry_vec_rat(sc->scopes, 0);
	var_scope_entry_add(entry, var);
}

var_t* var_scope_get(var_scope_t* sc, char const* name)
{
	assert(sc);
	assert(name);

	size_t s = var_scope_entry_vec_size(sc->scopes);

	for (size_t i = 0; i < s; ++i)
	{
		var_scope_entry_t* entry = var_scope_entry_vec_at(sc->scopes, i);
		var_t* var = var_scope_entry_get(entry, name);

		if (var)
			return var;
	}

	return NULL;
}
