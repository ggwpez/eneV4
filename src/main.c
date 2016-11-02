#include "ast/ast.h"
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(char const* s);

int main()
{
	yyin = stdin;

	if (yyparse())
		fprintf(stderr, "\nParsing failed\n");
	else
		printf("\nParsing succeeded\n");

	return 0;
}

void yyerror(char const* s)
{
	fprintf(stderr, "YYERROR: %s\n", s);
	exit(1);
}
