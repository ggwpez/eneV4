#include "mid_end.h"
#include "scoper.h"

me_error_t process(ast_ptr node)
{
	program_node_t* prog = assert_cast(node, program_node_t*, AST_PROGRAM);

	if (scoper_process(NULL, node))
		return MEE_SCOPER;

	return MEE_NONE;
}
