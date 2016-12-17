#include "back_end.h"
#include "il.h"
#include <stdio.h>

error_t write_to_file(char const* str, char const* out_file)
{
	FILE* file = stdout;

	if (out_file)
	{
		file = fopen(out_file, "w");

		if (! file)
			return fprintf(stderr, "File: '%s' could not be writte to\n", out_file), BE_BAD_FILE;
	}

	fprintf(file, "%s", str);

	if (file != stdout)
		fclose(file);
	return SUCCESS;
}

error_t be_process(program_node_t* node, unsigned opt_lvl, char const* mod_name, char const* file_name)
{
	if (! node || ! mod_name)
		return BAD_ARGS;

	char* output;
	CHECK_ERR(il_process(node, opt_lvl, mod_name, &output));
	CHECK_ERR(write_to_file(output, file_name));
	free(output);

	return SUCCESS;
}
