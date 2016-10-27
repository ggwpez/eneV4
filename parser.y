%{
#include "ast.h"
#include <stdlib.h>

extern ast_vector_t* root;
%}

%union {
	int ival;
	float fval;
	struct ast* node;
}

%token<ival> T_INT
%token<fval> T_FLOAT
%token T_ADD T_SUB T_MUL T_DIV T_DRF T_EQU T_SML T_GRT T_AND T_OR T_LEFT T_RIGHT
%token T_NOT T_POP T_CPY
%token T_WHILE T_FOR T_IF T_NSPACE T_ELSE T_BREAK T_GOON T_RETURN T_STRUCT
%token T_NEWLINE T_QUIT

%type<node> exp program_exp
%type<node> exp_cpy void
%type<node> un_term bin_term
%start program

%%

program:
	| program program_exp { if ($2) ast_vector_push_back(root, $2), ast_print($2); else printf("<NULL>\n"); }
//{ if ($2) ast_print($2), ast_del($2); else printf("<NULL>\n"); }


program_exp: T_NEWLINE	{ $$ = NULL; }
	| void T_NEWLINE		{ $$ = $1; }
	| T_QUIT T_NEWLINE	{ $$ = NULL; printf("bye!\n"); exit(0); }

exp: T_INT					{ $$ = new(atom, ATOM_INT, &$1);	  }
	| T_FLOAT				{ $$ = new(atom, ATOM_FLOAT, &$1);	  }
	| bin_term				{ $$ = $1; }
	| un_term				{ $$ = $1; }

void: exp T_POP				{ $$ = new(unop, UNOP_POP, $1); }
	//| void void				{ $$ =  $$ = new(binop, BINOP_SEQ, $1, $2); }
	| exp void T_POP		{ $$ =  $$ = new(binop, BINOP_SEQ, $1, $2); }

un_term: exp T_NOT			{ $$ = new(unop, UNOP_NOT, $1); }
		//| exp T_POP			{ $$ = new(unop, UNOP_POP, $1); }
		| exp T_CPY			{ $$ = new(unop, UNOP_CPY, $1); }

bin_term: exp exp T_ADD			{ $$ = new(binop, BINOP_ADD, $1, $2); }
		| exp exp T_SUB			{ $$ = new(binop, BINOP_SUB, $1, $2); }
		| exp exp T_MUL			{ $$ = new(binop, BINOP_MUL, $1, $2); }
		| exp exp T_DIV			{ $$ = new(binop, BINOP_DIV, $1, $2); }
		| exp exp T_DRF			{ $$ = new(binop, BINOP_DRF, $1, $2); }
		| exp exp T_EQU			{ $$ = new(binop, BINOP_EQU, $1, $2); }
		| exp exp T_SML			{ $$ = new(binop, BINOP_SML, $1, $2); }
		| exp exp T_GRT			{ $$ = new(binop, BINOP_GRT, $1, $2); }
		| exp exp T_AND			{ $$ = new(binop, BINOP_AND, $1, $2); }
		| exp exp T_OR			{ $$ = new(binop, BINOP_OR , $1, $2); }
