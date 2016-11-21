#pragma once

#include "var_scope_entry.h"

typedef struct
{
	var_scope_entry_vector_t* scopes;
} var_scope_t;

void var_scope_enter(var_scope_t* sc);
void var_scope_leave(var_scope_t* sc);
void var_scope_add(var_scope_t* sc, var_t* var);
var_t* var_scope_get(var_scope_t* sc, char const* name);
