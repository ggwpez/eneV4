#ifndef AST_H
#define AST_H

#include "defines.h"

struct ast_vector;

typedef enum ast_type
{
	AST_ATOM,
	AST_UNOP,
	AST_BINOP,
	AST_PROGRAM,
	AST_size
} ast_type_t;

typedef struct ast
{
	ast_type_t t;

	void* node;
} ast_t;
typedef ast_t* ast_ptr;

ast_t* ast_new(ast_type_t t, ast_ptr node);
void ast_del(ast_ptr val);
int ast_print(ast_ptr val);

// START ATOM
typedef enum atom
{
	ATOM_INT,
	ATOM_FLOAT,
	ATOM_REF_TO_RES,			// a reference to the value on below in the stack
	ATOM_size
} atom_t;

typedef struct atom_node
{
	atom_t t;

	union
	{
		int v0;
		float v1;
		ast_ptr v2;
	};
} atom_node_t;

ast_t* atom_new(atom_t t, void const* value);
void atom_del(atom_node_t* val);
void atom_print(atom_node_t* val);
// END ATOM

// START UNARY
typedef enum unop
{
	UNOP_NOT,
	UNOP_POP,		// pop stack
	UNOP_CPY,		// copy stack
	UNOP_DRF,
	UNOP_size
} unop_t;

typedef struct unop_node
{
	unop_t t;

	ast_ptr node;
} unop_node_t;

ast_t* unop_new(unop_t t, ast_ptr node);
void unop_del(unop_node_t* node);
void unop_print(unop_node_t* node);
// END UNARY

// START BINARY
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

typedef struct binop_node
{
	binop_t t;

	ast_ptr x, y;
} binop_node_t;

ast_t* binop_new(binop_t t, ast_ptr x, ast_ptr y);
void binop_del(binop_node_t* node);
void binop_print(binop_node_t* node);
// END BINARY

// START PROGRAM
typedef struct program_node
{
	struct ast_vector* v;
} program_node_t;

ast_t* program_new();
void program_del(program_node_t* node);
void program_print(program_node_t* node);
// END PROGRAM

// START VECTOR
#include <stddef.h>

typedef struct ast_vector
{
	ast_ptr* data;
	ast_ptr* ptr;
	size_t capacity;
} ast_vector_t;

ast_vector_t* ast_vector_new(size_t capacity);
void ast_vector_del(ast_vector_t* v);
ast_vector_t* ast_vector_resize(ast_vector_t* v, size_t new_capacity);
size_t ast_vector_size(ast_vector_t *);
ast_ptr ast_vector_at(ast_vector_t* v, size_t i);
ast_ptr ast_vector_rat(ast_vector_t* v, size_t ri);		// reverse at()
ast_ptr ast_vector_push_back(ast_vector_t* v, ast_ptr e);
ast_ptr ast_vector_pop_back(ast_vector_t* v);
void ast_vector_print(ast_vector_t* v);
// END VECTOR
#endif // AST_H
