#include <stdio.h>
#include <stdlib.h>

extern "C"
{
	#include "ast.h"

	extern int yylex();
	extern int yyparse();
	extern FILE* yyin;
	void yyerror(const char* s);
}

int main()
{
	yyin = stdin;

	//do
	//{

	yyparse();
	if (!feof(yyin))
		printf("\nParsing stopped!\n");
	//} while();

	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "deine mudda ist ein error: %s\n", s);
	exit(1);
}
