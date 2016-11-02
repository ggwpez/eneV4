#pragma once

#include "../defines.h"

struct ast_vector;

typedef enum ast_type
{
	AST_ATOM,
	AST_UNOP,
	AST_BINOP,
	AST_PROGRAM,
	AST_BLOCK,
	AST_IF,
	AST_WHILE,
	AST_size
} ast_type_t;

typedef struct ast
{
	ast_type_t t;

	void* node;
} ast_t;
typedef ast_t* ast_ptr;

ast_ptr ast_new(ast_type_t t, void* node);
void ast_del(ast_ptr val);
int ast_print(ast_ptr val);

// START VECTOR

// END VECTOR
