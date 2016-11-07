#pragma once

#include "../ast/common.h"
#include "../defines.h"
#include "../vector.h"

/* This indicates whether idents are unique between function and variables */
#define UNIQUE_IDENTS 1

typedef fun_decl_node_t* fun_decl_ptr;
typedef var_decl_node_t* var_decl_ptr;

MAKE_VECTOR_H(fun_decl)
MAKE_VECTOR_H(var_decl)

typedef struct context
{
	fun_decl_vector_t* funs;
	var_decl_vector_t* vars;
} context_t;

context_t* context_new(/*char const* namespace_name*/);
void context_del(context_t* obj);
void context_print(context_t* obj);

typedef enum context_add
{
	CA_SUCC = 0,
	CA_DOUBLE,
	CA_FUN_REG_AS_VAR,		// Function name already used as variable name
	CA_VAR_REG_AS_FUN,		// vice versa
	CA_UNDERLYING			// underlying error
} context_add_t;

fun_decl_node_t* context_get_fun(context_t* obj, ident_node_t* name);
var_decl_node_t* context_get_var(context_t* obj, ident_node_t* name);
context_add_t context_add_fun(context_t* obj, fun_decl_node_t* elem);
context_add_t context_add_var(context_t* obj, var_decl_node_t* elem);

typedef context_t* context_ptr;
MAKE_VECTOR_H(context)