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
		CHECK(yyin, FILE_NOT_FOUND, in_file);
	}

	return 0;
}

program_node_t* parse_file(char const* in_file)
{
	CHECK(!open_file(in_file), ASSERT,);
	CHECK(! yyparse(), PARSING_FAILED,);

	if (yyin != stdin)
		fclose(yyin);
	yylex_destroy();

	return prog;
}

int compile_file(compiler_args_t* args)
{
	program_node_t* prog = parse_file(args->in_file);
	CHECK(prog, ASSERT,);

	error_t ret;
	char *module_name;
	CHECK(asprintf(&module_name, "%s-module", args->in_file ? args->in_file : "stdin"), UNDERLYING,);

	ret = me_process(prog);
	if (ret)
		return fprintf(stderr, "\n\n%s\n", "Mid End Rrror"), -1;

	ret = be_process(prog, args->opt_lvl, module_name, args->out_file);
	if (ret)
		return fprintf(stderr, "\n\n%s\n", "Back End Error!"), -1;

	delete(program, prog);
	free(module_name);
	return 0;
}
