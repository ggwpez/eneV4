#pragma once

#include "ast.h"
#include "type_node.h"
#include "ident_node.h"
#include "block_node.h"
#include "../vector.h"

typedef struct arg_node		// eg. int a
{
	type_node_t* type;
	ident_node_t* name;
} arg_node_t;
arg_node_t* arg_new_ng(type_node_t* type, ident_node_t* name);
void arg_del(arg_node_t* node);
void arg_print(arg_node_t* node);

typedef arg_node_t* arg_ptr;
MAKE_VECTOR_H(arg)					// create a vector for storing arg_nodes_t s

typedef struct arg_list_node	// eg. (int a, int b)
{
	arg_vector_t* args;
} arg_list_node_t;
arg_list_node_t* arg_list_new_ng(arg_vector_t* args);
void arg_list_del(arg_list_node_t* node);
void arg_list_print(arg_list_node_t* node);

typedef struct fun_decl_node
{
	type_node_t* type;
	ident_node_t* name;
	block_node_t* code;
	arg_list_node_t* args;
} fun_decl_node_t;

ast_ptr fun_decl_new(type_node_t* type, ident_node_t* name, block_node_t* code, arg_list_node_t* args);
void fun_decl_del(fun_decl_node_t* node);
void fun_decl_print(fun_decl_node_t* node);
