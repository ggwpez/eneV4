#pragma once

#include "ast.h"
#include "block_node.h"

typedef struct for_node
{
	ast_ptr init;
	ast_ptr cond;
	ast_ptr inc;
	block_node_t* block;
} for_node_t;

ast_ptr for_new(ast_ptr init, ast_ptr cond, ast_ptr inc, block_node_t* block);
void for_del(for_node_t* node);
void for_print(for_node_t* node);
