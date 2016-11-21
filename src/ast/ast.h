#pragma once

#include "../defines.h"
#include "../vector.h"

typedef enum ast_type
{
	AST_ATOM,
	AST_UNOP,
	AST_BINOP,

	AST_PROGRAM,
	AST_BLOCK,
	AST_RETURN,

	AST_IDENT,
	AST_TYPE,
	AST_TEXP,
	AST_CAST,

	AST_VAR_DECL,
	AST_FUN_DECL,
	AST_FUN_CALL,

	AST_IF,
	AST_WHILE,
	AST_FOR,

	AST_size
} ast_type_t;

typedef struct ast
{
	ast_type_t t;

	void* node;
} ast_t;

typedef ast_t* ast_ptr;
MAKE_VECTOR_H(ast)

ast_ptr ast_new(ast_type_t t, void* node);
void ast_del(ast_ptr val);
int ast_print(ast_ptr val);


