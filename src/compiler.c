#include "compiler.h"

#include "ast/program_node.h"
#include "mid_end/mid_end.h"
#include "back_end/back_end.h"

#include <stdio.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(char const* s);

program_node_t* prog = NULL;

int open_file(char const* in_file)
{
	yyin = stdin;

	if (in_file)
	{
		yyin = fopen(in_file, "r");
		if (! yyin)
			return fprintf(stderr, "File: '%s' not found\n", in_file), -1;
	}

	return 0;
}

program_node_t* parse_file(char const* in_file)
{
	if (open_file(in_file))
		return NULL;

	if (yyparse())
		return fprintf(stderr, "\nParsing failed\n"), NULL;

	if (yyin != stdin)
		fclose(yyin);

	return prog;
}

int compile_file(char const* in_file, char const* out_file)
{
	program_node_t* prog = parse_file(in_file);

	if (! prog)
		return -1;

	error_t ret;
	char *module_name;
	asprintf(&module_name, "%s-module", in_file ? in_file : "stdin");

	ret = me_process(prog);
	if (ret)
		return fprintf(stderr, "\n\n%s\n", "Mid End Rrror"), -1;

	ret = be_process(prog, module_name, out_file);
	if (ret)
		return fprintf(stderr, "\n\n%s\n", "Back End Error!"), -1;

	program_del(prog);
	free(module_name);
	return 0;
}
