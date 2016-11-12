#pragma once

#include "ast.h"

typedef enum binop
{
	BINOP_ADD,
	BINOP_SUB,
	BINOP_MUL,
	BINOP_DIV,
	BINOP_DRF,
	BINOP_EQU,
	BINOP_SML,
	BINOP_GRT,
	BINOP_AND,
	BINOP_SEQ,		// c comma operator, result is the last expression, right assoc.
	BINOP_OR,
	BINOP_size
} binop_t;
extern char const* binop_strings[];

typedef struct binop_node
{
	binop_t t;

	ast_ptr x, y;
} binop_node_t;

ast_ptr binop_new(binop_t t, ast_ptr x, ast_ptr y);
void binop_del(binop_node_t* node);
void binop_print(binop_node_t* node);
