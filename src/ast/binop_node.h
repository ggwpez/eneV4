#pragma once

#include "ast.h"
#include "../enums.h"

#define BINOP_ENUM	(BINOP_ADD)\
					(BINOP_SUB)\
					(BINOP_MUL)\
					(BINOP_DIV)\
					(BINOP_EQU)\
					(BINOP_ASS)\
					(BINOP_ARR)\
					(BINOP_SML)\
					(BINOP_GRT)\
					(BINOP_AND)\
					(BINOP_OR)\
					(BINOP_SEQ)\
					(BINOP_size)
MAKE_ENUM(binop, BINOP_ENUM)

typedef struct binop_node
{
	binop_t t;

	ast_ptr x, y;
} binop_node_t;

ast_ptr binop_new(binop_t t, ast_ptr x, ast_ptr y);
void binop_del(binop_node_t* node);
void binop_print(binop_node_t* node);
