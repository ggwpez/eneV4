#pragma once

#include "../defines.h"
#include "../vector.h"

struct ast_vector;

typedef enum ast_type
{
    AST_ATOM,
    AST_UNOP,
    AST_BINOP,

<<<<<<< HEAD
    AST_PROGRAM,
    AST_BLOCK,
=======
	AST_PROGRAM,
	AST_BLOCK,
	AST_RETURN,
>>>>>>> 5cd23b9126ebbbd28e26fdf6ed9f4a696dbab97e

    AST_IDENT,
    AST_TYPE,

<<<<<<< HEAD
    AST_VAR_DECL,
    AST_FUN_DECL,
    AST_FUN_CALL,
=======
	AST_VAR_DECL,
	AST_FUN_DECL,
	AST_FUN_CALL,
>>>>>>> 5cd23b9126ebbbd28e26fdf6ed9f4a696dbab97e

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

ast_ptr ast_new(ast_type_t t, void* node);
void ast_del(ast_ptr val);
int ast_print(ast_ptr val);

// START VECTOR
MAKE_VECTOR_H(ast)
// END VECTOR
