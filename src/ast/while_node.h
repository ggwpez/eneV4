#pragma once

#include "ast.h"

typedef struct while_node
{
	ast_ptr cond;
	ast_ptr true_node;
} while_node_t;

ast_ptr while_new(ast_ptr cond, ast_ptr true_node);
void while_del(while_node_t* node);
void while_print(while_node_t* node);
