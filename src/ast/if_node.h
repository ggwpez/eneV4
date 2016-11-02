#pragma once

#include "ast.h"

typedef struct if_node
{
	ast_ptr cond;
	ast_ptr true_node;
	ast_ptr else_node;
} if_node_t;

ast_ptr if_new(ast_ptr cond, ast_ptr true_node, ast_ptr else_node);
void if_del(if_node_t* node);
void if_print(if_node_t* node);
