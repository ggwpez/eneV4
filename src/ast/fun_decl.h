#pragma once

#include "ast.h"
#include "type_node.h"
#include "ident_node.h"
#include "block_node.h"
#include "var_decl_node.h"
#include "../vector.h"

typedef struct fun_decl_node
{
	type_node_t* type;
	ident_node_t* name;
	block_node_t* code;
	var_decl_vec_t* args;
} fun_decl_node_t;

typedef fun_decl_node_t* fun_decl_ptr;
MAKE_VECTOR_H(fun_decl)

ast_ptr fun_decl_new(type_node_t* type, ident_node_t* name, block_node_t* code, var_decl_vec_t* args);
void fun_decl_del(fun_decl_node_t* node);
void fun_decl_print(fun_decl_node_t* node);
