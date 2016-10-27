%{
#include "ast.h"
#include "y.tab.h"
#include <stdlib.h>

extern ast_vector_t* root;

binop_t tok_to_binop(int t)
{
	return (binop_t)(t -(int)T_ADD);
}

%}

%union {
	int ival;
	float fval;
	struct ast* node;
}

%token<ival> T_INT
%token<fval> T_FLOAT
%token T_ADD T_SUB T_MUL T_DIV T_DRF T_EQU T_SML T_GRT T_AND T_SEQ T_OR T_LEFT T_RIGHT
%token T_NOT T_POP T_CPY
%token T_ATLEAST_S1 T_ATLEAST_S2
%token T_WHILE T_FOR T_IF T_NSPACE T_ELSE T_BREAK T_GOON T_RETURN T_STRUCT
%token T_NEWLINE T_QUIT

%right T_SEQ

%type<node> exp program_exp
%type<node> exp_cpy void
%type<node> un_term bin_term
%type<ival> bin_op
%start program

%%

program:
	| program program_exp { if ($2) ast_vector_push_back(root, $2), ast_print($2); else printf("<NULL>\n"); }
//{ if ($2) ast_print($2), ast_del($2); else printf("<NULL>\n"); }


program_exp:
	T_NEWLINE				{ $$ = NULL; }
	| exp T_NEWLINE			{ $$ = $1; }
	| T_QUIT T_NEWLINE		{ $$ = NULL; printf("bye!\n"); exit(0); }

exp:
	T_INT					{ $$ = new(atom, ATOM_INT, &$1);	  }
	| T_FLOAT				{ $$ = new(atom, ATOM_FLOAT, &$1);	  }
	| bin_term				{ $$ = $1; }
	| un_term				{ $$ = $1; }

exp_cpy:
		exp T_CPY			{ $$ = new(unop, UNOP_CPY, $1); }

un_term:
		exp T_NOT			{ $$ = new(unop, UNOP_NOT, $1); }
		//| exp T_DRF			{ $$ = new(unop, UNOP_DRF, $1); }
//		| exp T_CPY			{ $$ = new(unop, UNOP_CPY, $1); }

mb_atleast1: T_ATLEAST_S1 |
mb_atleast2: T_ATLEAST_S2 |
bin_term: //1 1 ^$+							// why you do this?
	 //exp exp T_POP { $$ = new(binop, BINOP_SEQ, $2, $1); }
		//| exp exp T_POP			{ $$ = new(binop, BINOP_SEQ, $2, $1); }		// evaluate both, take first as result the same as: '$2, $1'
		exp T_ATLEAST_S1		{ $$ = $1; }
		//| exp exp T_ATLEAST_S2  { $$ = new(binop, BINOP_SEQ, $2, $1); }
		| exp_cpy mb_atleast2 T_ADD			{ $$ = new(binop, BINOP_ADD, $1, $1); }			// he gets that it is a reference to the stack.top() because the pointers are equal
		| exp exp mb_atleast2 T_ADD			{ $$ = new(binop, BINOP_ADD, $1, $2); }
		| exp exp T_SUB			{ $$ = new(binop, BINOP_SUB, $1, $2); }
		| exp exp T_MUL			{ $$ = new(binop, BINOP_MUL, $1, $2); }
		| exp exp T_DIV			{ $$ = new(binop, BINOP_DIV, $1, $2); }
		| exp exp T_DRF			{ $$ = new(binop, BINOP_DRF, $1, $2); }
		| exp exp T_EQU			{ $$ = new(binop, BINOP_EQU, $1, $2); }
		| exp exp T_SML			{ $$ = new(binop, BINOP_SML, $1, $2); }
		| exp exp T_GRT			{ $$ = new(binop, BINOP_GRT, $1, $2); }
		| exp exp T_AND			{ $$ = new(binop, BINOP_AND, $1, $2); }
		| exp exp T_SEQ			{ $$ = new(binop, BINOP_SEQ, $1, $2); }
		| exp exp T_OR			{ $$ = new(binop, BINOP_OR , $1, $2); }
