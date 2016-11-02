#pragma once

#include "ast.h"

typedef enum unop
{
	UNOP_NOT,
	UNOP_POP,		// pop stack
	UNOP_CPY,		// copy stack
	UNOP_DRF,
	UNOP_size
} unop_t;

typedef struct unop_node
{
	unop_t t;

	ast_ptr node;
} unop_node_t;

ast_ptr unop_new(unop_t t, ast_ptr node);
void unop_del(unop_node_t* node);
void unop_print(unop_node_t* node);
