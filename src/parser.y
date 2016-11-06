%{
#define YYDEBUG 1

#include "src/ast/common.h"
#include "src/vector.h"
#include "y.tab.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

void crash(char const* msg)
{
	fprintf(stderr, "%s\n", msg);
	exit(0);
}

%}

%union {
	int ival;
	float fval;
	char* str;
	struct arg_node* f_arg;
	struct arg_list_node* f_arg_list;
	struct block_node* block;
	struct ident_node* ident;
	struct ur_type* ur_type;
	struct type_node* type;
	struct ast* node;
	struct ast_vector* stack;
}

%token<ival> T_INT
%token<fval> T_FLOAT
%token<str> T_ID
%token T_ADD T_SUB T_MUL T_DIV T_DRF T_EQU T_SML T_GRT T_AND T_SEQ T_OR T_LEFT T_RIGHT
%token T_BO T_BC T_EBO T_EBC T_CBO T_CBC T_SEMI T_PTR T_CONST
%token T_NOT T_POP T_CPY
%token T_WHILE T_FOR T_IF T_NSPACE T_ELSE T_BREAK T_GOON T_RETURN T_STRUCT
%token T_NEWLINE T_QUIT T_UNKNOWN T_EOF

%right T_SEQ

%type<node>	program program_content program_stmt
%type<block> block block_content
%type<node> block_stmt
%type<node> exp
%type<stack> exp_stack
%type<f_arg> f_arg
%type<f_arg_list> f_arg_list
%type<node> var_decl fun_decl
%type<node> term un_term bin_term
%type<node> if_stmt while_stmt for_stmt
%type<type> type
%type<ur_type> type_part
%type<node> atom
%type<ident> ident
%start program

%%

program: program_content T_EOF { ast_print($$), puts(""); YYACCEPT; }

program_content:  { $$ = new(program,); } | program_content program_stmt
{
	program_node_t* prog = assert_cast($1, program_node_t*, AST_PROGRAM);

	if ($2)
		ast_vector_push_back(prog->v, $2);

	$$ = $1;
}

program_stmt:
			T_NEWLINE { $$ = NULL; }
			| var_decl T_SEMI
			| fun_decl

block: T_SEMI { $$ = new_ng(block,); }
	 | T_CBO block_content T_CBC { $$ = $2; }

block_content: { $$ = NULL; } | block_content block_stmt
{
	if ($1)
	{
		block_node_t* block = $1;

		if ($2)
			ast_vector_push_back(block->v, $2);

		$$ = $1;
	}
	else
	{
		block_node_t* block = new_ng(block,);

		if ($2)
			ast_vector_push_back(block->v, $2);

		$$ = block;
	}
}

block_stmt:
			T_NEWLINE { $$ = NULL; }
			| exp T_SEMI
			| if_stmt
			| while_stmt
			| for_stmt

ident: T_ID						{ $$ = new_ng(ident, $1, true); }
type_part:
		ident					{ $$ = new_ng(ur_type, UR_TYPE_MOD_ID, $1, NULL); }
		| type_part T_PTR		{ $$ = new_ng(ur_type, UR_TYPE_MOD_PTR, NULL, $1); }
		| type_part T_CONST		{ $$ = new_ng(ur_type, UR_TYPE_MOD_CONST, NULL, $1); }
type: type_part					{ $$ = new_ng(type, $1); }

var_decl: type ident			{ $$ = new(var_decl, $1, $2); }

f_arg: type ident { $$ = new_ng(arg, $1, $2); }
f_arg_list: { $$ = new_ng(arg_list, new_ng(arg_vector, 0)); } | T_SEQ f_arg_list
fun_decl: type ident T_BO f_arg_list T_BC block  { $$ = new(fun_decl, $1, $2, ); }

if_stmt:
		T_IF T_BO exp T_BC block T_ELSE block	{ $$ = new(if, $3, $5, $7); }
	   | T_IF T_BO exp T_BC block				{ $$ = new(if, $3, $5, NULL); }

while_stmt: T_WHILE T_BO exp T_BC block			{ $$ = new(while, $3, $5); }

for_stmt: T_FOR T_BO exp T_SEMI exp T_SEMI exp T_BC block { $$ = new(for, $3, $5, $7, $9); }

exp: exp_stack
{
	if (!$1)
		crash("Expression completly wrong!");
	size_t l = ast_vector_size($1);

	if (!l)
		crash("Expression returning void");
	else if (l > 1)
		crash("Expression returning more than one value");

	$$ = ast_vector_pop_back($1);	// delete $1
}

exp_stack: { $$ = NULL; } | exp_stack term
{
	if ($1)
	{
		if ($2)
		{
			ast_ptr op = $2;
			ast_vector_t* stack = $1;
			size_t l = ast_vector_size(stack);

			if (op->t == AST_BINOP)
			{
				binop_node_t* binop = assert_cast(op, binop_node_t*, AST_BINOP);
				if (!l)
					crash("Binary operator cant be applied to [void void] (stack empty)");
				if (l < 2)
					crash("Binary operator cant be applied to [...  void] (stack missing one object)");

				binop->y = ast_vector_pop_back(stack);
				binop->x = ast_vector_pop_back(stack);
				ast_vector_push_back($1, op);
			}
			else if (op->t == AST_UNOP)
			{
				unop_node_t* unop = assert_cast(op, unop_node_t*, AST_UNOP);

				if (!l)
					crash("Unary operator cant be applied to [void] (stack empty)");

				if (unop->t == UNOP_CPY)
				{
					ast_ptr top = ast_vector_rat(stack, 0);
					ast_ptr cpy = NULL;
					if (top->t == AST_ATOM)
					{
						cpy = new(atom, ATOM_REF_TO_RES, NULL);			// dosent matter
						*cpy = *top;									// trivially copyable
					}
					else
						cpy = new(atom, ATOM_REF_TO_RES, NULL);

					ast_vector_push_back(stack, cpy);	// copy if atom
				}
				else if (unop->t == UNOP_POP)
					delete(ast, ast_vector_pop_back(stack));
				else
				{
					unop->node = ast_vector_pop_back(stack);
					ast_vector_push_back($1, op);
				}
			}
			else
				ast_vector_push_back($1, op);
		}

		$$ = $1;
	}
	else
	{
		ast_vector_t* ret = new_ng(ast_vector, 5);

		if ($2)
			ast_vector_push_back(ret, $2);

		$$ = ret;
	}
}

term:
	atom
   | bin_term
   | un_term

atom:
	T_ID					{ $$ = new(ident, $1, true); }
	| T_INT					{ $$ = new(atom, ATOM_INT, &$1); }
	| T_FLOAT				{ $$ = new(atom, ATOM_FLOAT, &$1); }

un_term:
	T_POP					{ $$ = new(unop, UNOP_POP, NULL); }
	| T_CPY					{ $$ = new(unop, UNOP_CPY, NULL); }

bin_term: T_ADD				{ $$ = new(binop, BINOP_ADD, NULL, NULL); }
