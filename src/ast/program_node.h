#pragma once

#include "ast.h"

typedef struct program_node
{
	ast_vector_t* v;
} program_node_t;

ast_ptr program_new();
void program_del(program_node_t* node);
void program_print(program_node_t* node);
