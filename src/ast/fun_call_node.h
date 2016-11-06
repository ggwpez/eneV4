#pragma once

#include "ast.h"
#include "ident_node.h"

typedef struct fun_call_node
{
    ident_node_t* name;
    ast_vector_t* args;
} fun_call_node_t;

ast_ptr fun_call_new(ident_node_t* name, ast_vector_t* args);
fun_call_node_t* fun_call_new_ng(ident_node_t* name, ast_vector_t* args);
void fun_call_del(fun_call_node_t* node);
void fun_call_print(fun_call_node_t* node);
