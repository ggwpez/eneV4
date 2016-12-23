#pragma once

#include "../defines.h"
#include "../vector.h"
#include "../enums.h"

#define AST_TYPE_ENUM	(AST_ATOM)\
						(AST_UNOP)\
						(AST_BINOP)\
						(AST_PROGRAM)\
						(AST_BLOCK)\
						(AST_RETURN)\
						(AST_IDENT)\
						(AST_TYPE)\
						(AST_TEXP)\
						(AST_CAST)\
						(AST_VAR_DECL)\
						(AST_FUN_DECL)\
						(AST_FUN_CALL)\
						(AST_IF)\
						(AST_WHILE)\
						(AST_FOR)\
						(AST_size)
MAKE_ENUM(ast_type,	AST_TYPE_ENUM)

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


