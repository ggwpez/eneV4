#pragma once

#include "ast.h"
#include "../mid_end/r_type.h"

typedef struct block_node
{
	struct ast_vector* v;
} block_node_t;

ast_ptr block_new();
block_node_t* block_new_ng();
void block_del(block_node_t* node);
void block_print(block_node_t* node);
