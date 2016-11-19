#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"

int main(int argc, char** argv)
{
	if (argc == 3)
		compile_file(argv[1], argv[2]);
	else if (argc == 2)
		compile_file(argv[1], NULL);
	else if (argc == 1)
		compile_file(NULL, NULL);
	else
		fprintf(stderr, "%s", "Usage:\nene input.ene output.llvm");

	return 0;
}

void yyerror(char const* s)
{
	fprintf(stderr, "YYERROR: %s\n", s);
	exit(1);
}
