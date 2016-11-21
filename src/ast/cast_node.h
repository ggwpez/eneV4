#pragma once

#include "ast.h"
#include "../mid_end/ur_type.h"

typedef struct cast_node
{
	ast_ptr node;

	ur_type_t* t;
} cast_node_t;

ast_ptr cast_new(ast_ptr node, ur_type_t* t);
void cast_del(cast_node_t* node);
void cast_print(cast_node_t* node);
