%option noyywrap

%{
#include <stdio.h>

#define YY_DECL int yylex()

#include "y.tab.h"

%}

%%

[ \t]	; // ignore all whitespace
[0-9]+\.[0-9]+		{ yylval.fval = atof(yytext); return T_FLOAT; }
[0-9]+				{ yylval.ival = atoi(yytext); return T_INT; }
\n					{ return T_NEWLINE; }

%x // BINOP
"$^"	{ return T_ATLEAST_S1; }
"^$"	{ return T_ATLEAST_S2; }
"+"		{ return T_ADD; }
"-"		{ return T_SUB; }
"*"		{ return T_MUL; }
"/"		{ return T_DIV; }
"~"		{ return T_DRF; }
"?"		{ return T_EQU; }
"<"		{ return T_SML; }
">"		{ return T_GRT; }
"&"		{ return T_AND; }
","		{ return T_SEQ; }
"|"		{ return T_OR; }
%x // UNOP
"!"		{ return T_NOT; }
"^"		{ return T_POP; }
"$"		{ return T_CPY; }
"~"		{ return T_DRF; }
%x // KEYWORD
"while"		{ return T_WHILE; }
"for"		{ return T_FOR; }
"if"		{ return T_IF; }
"namespace"	{ return T_NSPACE; }
"else"		{ return T_ELSE; }
"break"		{ return T_BREAK; }
"goon"		{ return T_GOON; }
"return"	{ return T_RETURN; }
"struct"	{ return T_STRUCT; }
"exit"	{ return T_QUIT; }
"q"		{ return T_QUIT; }
