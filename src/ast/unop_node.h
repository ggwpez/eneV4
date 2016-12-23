#pragma once

#include "ast.h"
#include "../enums.h"

#define UNOP_ENUM	(UNOP_NOT)\
					(UNOP_POP)\
					(UNOP_CPY)\
					(UNOP_DRF)\
					(UNOP_size)
MAKE_ENUM(unop, UNOP_ENUM)

typedef struct unop_node
{
	unop_t t;

	ast_ptr node;
} unop_node_t;

ast_ptr unop_new(unop_t t, ast_ptr node);
void unop_del(unop_node_t* node);
void unop_print(unop_node_t* node);
