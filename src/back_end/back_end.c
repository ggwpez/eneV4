#include "back_end.h"
#include "il.h"
#include <stdio.h>

error_t be_process(ast_ptr node, const char* file_name)
{
	if (! node || ! file_name)
		return BAD_ARGS;

	return il_process(node, file_name);
}
