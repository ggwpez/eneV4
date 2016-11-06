#pragma once

#include "ast.h"
#include "block_node.h"

typedef struct while_node
{
	ast_ptr cond;
	block_node_t* true_node;
} while_node_t;

ast_ptr while_new(ast_ptr cond, block_node_t* true_node);
void while_del(while_node_t* node);
void while_print(while_node_t* node);
