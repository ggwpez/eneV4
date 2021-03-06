#pragma once

#include "ast.h"

typedef struct program_node
{
	ast_vec_t* v;
} program_node_t;

ast_ptr program_new();
program_node_t* program_new_ng();
void program_del(program_node_t* node);
void program_print(program_node_t* node);
