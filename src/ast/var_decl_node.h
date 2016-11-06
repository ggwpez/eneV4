#pragma once

#include "ast.h"
#include "ident_node.h"
#include "type_node.h"

typedef struct var_decl_node
{
	type_node_t* type;
	ident_node_t* name;
} var_decl_node_t;

ast_ptr var_decl_new(type_node_t* type, ident_node_t* name);
void var_decl_del(var_decl_node_t* node);
void var_decl_print(var_decl_node_t* node);
