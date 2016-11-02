#pragma once

#include "ast.h"

typedef struct block_node
{
	struct ast_vector* v;
} block_node_t;

ast_ptr block_new();
void block_del(block_node_t* node);
void block_print(block_node_t* node);
