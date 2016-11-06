#pragma once

#include "ast.h"

typedef struct return_node
{
	ast_ptr exp;
} return_node_t;

ast_ptr return_new(ast_ptr exp);
void return_del(return_node_t* node);
void return_print(return_node_t* node);
