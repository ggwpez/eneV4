#pragma once

#include "ast.h"

typedef struct program_node
{
	struct ast_vector* v;
} program_node_t;

ast_ptr program_new();
void program_del(program_node_t* node);
void program_print(program_node_t* node);
