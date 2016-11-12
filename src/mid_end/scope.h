#pragma once

#include "../ast/common.h"
#include "../defines.h"
#include "../vector.h"
#include "context.h"
#include "r_type.h"

/* This indicates whether idents are unique between function and variables */
#define UNIQUE_IDENTS 1

typedef struct scope
{
	context_vector_t* contexts;
} scope_t;

scope_t* scope_new();
void scope_del(scope_t* obj);
void scope_print(scope_t* obj);

void scope_enter(scope_t* obj/*char const* namespace_name*/);
void scope_leave(scope_t* obj);

fun_decl_node_t* scope_get_fun(scope_t* obj, ident_node_t* name);
var_decl_node_t* scope_get_var(scope_t* obj, ident_node_t* name);
context_add_t scope_add_fun(scope_t* obj, fun_decl_node_t* elem);
context_add_t scope_add_var(scope_t* obj, var_decl_node_t* elem);
context_add_t scope_add_vars(scope_t* obj, var_decl_vector_t* nodes);

int scope_transform_type(scope_t* obj, type_node_t* node);
int scope_transform_var(scope_t* obj, var_decl_node_t* node);
int scope_transform_vars(scope_t* obj, var_decl_vector_t* nodes);
r_type_t* scope_resolve_inbuild_str(scope_t* obj, char const* name);
r_type_t* scope_resolve_ur_type(scope_t* obj, ur_type_t* type);
