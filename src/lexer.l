%option noyywrap

%{
#include <stdio.h>

#define YY_DECL int yylex()

#include "y.tab.h"
#define YY_USER_ACTION \
yylloc.first_line = yylloc.last_line; \
yylloc.first_column = yylloc.last_column; \
for(int i = 0; yytext[i]; i++)\
{ \
	if(yytext[i] == '\n')\
	{ \
		yylloc.last_line++; \
		yylloc.last_column = 0; \
	} \
	else\
		yylloc.last_column++;\
}

void yyerror(char const* s)
{
	fprintf(stderr, "Lexing halted at %d:%d with msg: %s\n", yylloc.first_line, yylloc.first_column, s);
	exit(1);
}

%}

%option noyywrap
/* to avoid warning: ‘yyunput’ defined but not used */
%option nounput
/* to avoid warning: ‘input’ defined but not used */
%option noinput
%option yylineno

%%

[ \t\n]	; // ignore all whitespace
[0-9]+\.[0-9]+"d"	{ yylval.dval = atof(yytext); return T_DOUBLE; }
[0-9]+\.[0-9]+		{ yylval.fval = atof(yytext); return T_FLOAT; }
"-"[0-9]+			{ yylval.ival = atoi(yytext); return T_SINT; }
[0-9]+				{ yylval.uval = atoi(yytext); return T_UINT; }
\"(\\.|[^"])*\"		{ yylval.str = strdup(yytext); return T_STR; }
	/* BINOP */
"?"		{ return T_EQU; }
"+"		{ return T_ADD; }
"-"		{ return T_SUB; }
"*"		{ return T_MUL; }
"/"		{ return T_DIV; }
"<"		{ return T_SML; }
">"		{ return T_GRT; }
"&"		{ return T_AND; }
"|"		{ return T_OR; }
"#"		{ return T_HASH; }
	/* UNOP */
"!"		{ return T_NOT; }
"^"		{ return T_POP; }
"$"		{ return T_CPY; }
"~"		{ return T_DRF; }
	/* ASSIGN */
"="		{ return T_ASS; }
":"		{ return T_DDOT; }
	/* TOKS */
"("		{ return T_BO; }
")"		{ return T_BC; }
"["		{ return T_EBO; }
"]"		{ return T_EBC; }
"{"		{ return T_CBO; }
"}"		{ return T_CBC; }
";"		{ return T_SEMI; }
","     { return T_SEQ; }
	/* KEYWORD */
"while"		{ return T_WHILE; }
"for"		{ return T_FOR; }
"if"		{ return T_IF; }
"namespace"	{ return T_NSPACE; }
"else"		{ return T_ELSE; }
"break"		{ return T_BREAK; }
"goon"		{ return T_GOON; }
"return"	{ return T_RETURN; }
"struct"	{ return T_STRUCT; }
"exit"      { return T_QUIT; }
"q"         { return T_QUIT; }
"true"		{ return T_TRUE; }
"false"		{ return T_FALSE; }
	/* TYPE MODS */
"'"         { return T_PTR; }
"const"     { return T_CONST; }
"\\"	{ return T_BS; }

[_a-zA-Z][_a-zA-Z0-9]* { yylval.str = strdup(yytext); return T_ID; }

<<EOF>>		{ return T_EOF; }
.			{ return T_UNKNOWN; }
