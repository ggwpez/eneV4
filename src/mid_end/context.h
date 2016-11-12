#pragma once

#include "../ast/common.h"
#include "../defines.h"
#include "../vector.h"

typedef struct context
{
	fun_decl_vector_t* funs;
	var_decl_vector_t* vars;
	// struct_decl_vector_t* structs;
} context_t;

typedef context_t* context_ptr;
MAKE_VECTOR_H(context)

context_t* context_new(/*char const* namespace_name*/);
void context_del(context_t* obj);
void context_print(context_t* obj);

typedef enum context_add
{
	CA_SUCC = 0,
	CA_DOUBLE,
	CA_FUN_REG_AS_VAR,		// Function name already used as variable name
	CA_VAR_REG_AS_FUN,		// vice versa
	CA_TYPE_NOT_FOUND,
	CA_UNDERLYING			// underlying error
} context_add_t;

fun_decl_node_t* context_get_fun(context_t* obj, ident_node_t* name);
var_decl_node_t* context_get_var(context_t* obj, ident_node_t* name);
//struct_decl_node_t* context_get_struct(context_t* obj, ident_node_t* name);
context_add_t context_add_fun(context_t* obj, fun_decl_node_t* elem);
context_add_t context_add_var(context_t* obj, var_decl_node_t* elem);
//context_add_t context_add_struct(context_t* obj, struct_decl_node_t* elem);
