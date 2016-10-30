#include <stdio.h>
#include <stdlib.h>

extern "C"
{
	#include "ast.h"

	extern int yylex();
	extern int yyparse();
	extern FILE* yyin;
	void yyerror(const char* s);
	ast_vector_t* root = NULL;
}

int main()
{
	yyin = stdin;



	do {
		root = new(ast_vector, 5);
		yyparse();
		ast_vector_del(root);
	} while(!feof(yyin));

	return 0;
}

void yyerror(const char* s) {
	fprintf(stderr, "deine mudda ist ein error: %s\n", s);
	exit(1);
}
