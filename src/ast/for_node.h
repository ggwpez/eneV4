#pragma once

#include "ast.h"

typedef struct for_node
{
    ast_ptr init;
    ast_ptr cond;
    ast_ptr inc;
    ast_ptr block;
} for_node_t;

ast_ptr for_new(ast_ptr init, ast_ptr cond, ast_ptr inc, ast_ptr block);
void for_del(for_node_t* node);
void for_print(for_node_t* node);
