#pragma once

#include "ast.h"
#include "ident_node.h"
#include "type_node.h"

typedef struct var_decl_node
{
	type_node_t* type;
	ident_node_t* name;
} var_decl_node_t;

typedef var_decl_node_t* var_decl_ptr;
MAKE_VECTOR_H(var_decl)

ast_ptr var_decl_new(type_node_t* type, ident_node_t* name);
var_decl_node_t* var_decl_new_ng(type_node_t* type, ident_node_t* name);
void var_decl_del(var_decl_node_t* node);
void var_decl_print(var_decl_node_t* node);
