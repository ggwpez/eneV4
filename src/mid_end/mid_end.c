#include "mid_end.h"
#include "scope.h"

me_error_t process(ast_ptr node)
{
	program_node_t* prog = assert_cast(node, program_node_t*, AST_PROGRAM);

	if (! process_scoping(prog))
		return MEE_SCOPE;

	return MEE_NONE;
}

int process_scoping(program_node_t* node)
{
	scope_t* sc = scope_new();
	scope_enter(sc);




	scope_leave(sc);
	scope_del(sc);
}
