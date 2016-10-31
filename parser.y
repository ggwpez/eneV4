%{
#define YYDEBUG 1

#include "ast.h"
#include "y.tab.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

extern ast_vector_t* root_stack;

void crash(char const* msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(0);
}

%}

%union {
	int ival;
	float fval;
	struct ast* node;
	struct ast_vector* stack;
}

%token<ival> T_INT
%token<fval> T_FLOAT
%token T_ADD T_SUB T_MUL T_DIV T_DRF T_EQU T_SML T_GRT T_AND T_SEQ T_OR T_LEFT T_RIGHT
%token T_NOT T_POP T_CPY
%token T_WHILE T_FOR T_IF T_NSPACE T_ELSE T_BREAK T_GOON T_RETURN T_STRUCT
%token T_NEWLINE T_QUIT

%right T_SEQ

%type<node> program
%type<node>	exp program_exp
%type<stack> exp_stack term un_term bin_term
%type<stack> bin_op
%type<node> atom
%start program

%%

program: { $$ = NULL; } | program program_exp
	{
		if ($1)
		{
			printf("add\n");
			program_node_t* prog = assert_cast($1, program_node_t*, AST_PROGRAM);

			if ($2)
				ast_vector_push_back(prog->v, $2);

			$$ = $1;
		}
		else
		{
			printf("creat");
			ast_ptr ret = new(program,);
			program_node_t* prog = (program_node_t*)ret->node;		// this cast is assumed safe

			if ($2)
				ast_vector_push_back(prog->v, $2);

			$$ = ret;
		}
	}

program_exp:
	T_NEWLINE				{ $$ = NULL; }
	| exp T_NEWLINE			{ $$ = $1; }

exp: exp_stack
{
	size_t l = ast_vector_size(root_stack);

	if (!l)
		crash("Expression returning void");
	else if (l > 1)
		crash("Expression returning more than one value");

	$$ = ast_vector_pop_back(root_stack);
}

exp_stack:
	| exp_stack term

term:
	atom					{ ast_vector_push_back(root_stack, $1); }
   | bin_term
   | un_term

atom:
	T_INT					{ $$ = new(atom, ATOM_INT, &$1); }
	| T_FLOAT				{ $$ = new(atom, ATOM_FLOAT, &$1); }

un_term:
	T_POP					{ if (!ast_vector_size(root_stack)) crash("POP cant be applied to void (stack empty)"); else ast_vector_pop_back(root_stack); }
	| T_CPY					{ if (!ast_vector_size(root_stack)) crash("CPY cant be applied to void (stack empty)"); else ast_vector_push_back(root_stack, new(atom, ATOM_REF_TO_RES, NULL)); }

bin_term: T_ADD
{
	if (ast_vector_size(root_stack) < 2)
		YYERROR;
	else
	{
		ast_ptr x, y;
		x = ast_vector_pop_back(root_stack);
		y = ast_vector_pop_back(root_stack);

		ast_vector_push_back(root_stack, new(binop, BINOP_ADD, y, x));
	}
}
