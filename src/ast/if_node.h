#pragma once

#include "ast.h"
#include "block_node.h"

typedef struct if_node
{
	ast_ptr cond;
	block_node_t* true_node;
	block_node_t* else_node;
} if_node_t;

ast_ptr if_new(ast_ptr cond, block_node_t* true_node, block_node_t* else_node);
void if_del(if_node_t* node);
void if_print(if_node_t* node);
