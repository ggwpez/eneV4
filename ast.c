#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

ast_t* ast_new(ast_type_t t, ast_ptr node)
{
	ast_t* ret = malloc(sizeof(ast_t));

	ret->t = t;
	ret->node = node;

	return ret;
}

typedef void(*dtor_ptr)(void*);

dtor_ptr ast_dtors[] = { (dtor_ptr)atom_del, (dtor_ptr)unop_del, (dtor_ptr)binop_del, (dtor_ptr)program_del };
static_assert(_countof(ast_dtors) == AST_size, "ast_dtors invalid");

void ast_del(ast_ptr val)
{
	assert(val);
	assert((unsigned)val->t < AST_size);

	ast_dtors[(int)val->t](val->node);
	val->t = AST_size;
	free(val);
}

typedef void(*print_ptr)(void*);
print_ptr ast_prints[] = { (print_ptr)atom_print, (print_ptr)unop_print, (print_ptr)binop_print, (print_ptr)program_print };
static_assert(_countof(ast_prints) == AST_size, "ast_prints invalid");

int ast_print(ast_ptr val)
{
	assert(val);
	assert((unsigned)val->t < AST_size);

	ast_prints[(int)val->t](val->node);
	return 0;			// <- just that i can use it in ternary
}

ast_t* atom_new(atom_t t, void const* value)
{
	malloc_ptr(atom_node_t, ret);

	ret->t = t;
	if (t == ATOM_INT)
		ret->v0 = *(int*)value;
	else if (t == ATOM_FLOAT)
		ret->v1 = *(float*)value;

	return new(ast, AST_ATOM, (ast_ptr)ret);
}

void atom_del(atom_node_t* val)
{
	val->t = ATOM_size;
}

void atom_print(atom_node_t* val)
{
	if (val->t == ATOM_INT)
		printf("<int=%i>", val->v0);
	else if (val->t == ATOM_FLOAT)
		printf("<float=%f>", val->v1);
	else if (val->t == ATOM_REF_TO_RES)
		printf("<&top>");
}

ast_t* unop_new(unop_t t, ast_ptr node)
{
	malloc_ptr(unop_node_t, ret);

	ret->t = t;
	ret->node = node;

	return new(ast, AST_UNOP, (ast_ptr)ret);
}

void unop_del(unop_node_t* node)
{
	ast_del(node->node);
	node->t = UNOP_size;
}

char const* unop_strings[] = { "!", "^", "$", "~" };
static_assert(_countof(unop_strings) == UNOP_size, "unnop_strings invalid");
void unop_print(unop_node_t* node)
{
	printf("<unop("), ast_print(node->node), printf(",%s)>", unop_strings[(int)node->t]);
}

ast_t* binop_new(binop_t t, ast_ptr x, ast_ptr y)
{
	malloc_ptr(binop_node_t, ret);

	ret->t = t;
	ret->x = x;
	ret->y = y;

	return new(ast, AST_BINOP, (ast_ptr)ret);
}

void binop_del(binop_node_t* node)
{
	ast_del(node->x);
	ast_del(node->y);
	node->t = BINOP_size;
}

char const* binop_strings[] = { "+", "-", "*", "/", "~", "?", "<", ">", "&", ",", "|" };
static_assert(_countof(binop_strings) == BINOP_size, "binop_strings invalid");
void binop_print(binop_node_t* node)
{
	printf("<binop("), ast_print(node->x), putchar(','),
			node->x == node->y ? printf("<&top>") : ast_print(node->y),
	printf(",%s)>", binop_strings[(int)node->t]);
}

ast_t* program_new()
{
	malloc_ptr(program_node_t, ret);

	ret->v = malloct(ast_vector_t);

	return new(ast, AST_PROGRAM, (ast_ptr)ret);
}

void program_del(program_node_t* node)
{
	assert(node->v);

	free(node->v);
}

void program_print(program_node_t* node)
{
	printf("<program(");
	ast_vector_print(node->v);
	printf(")>");
}
