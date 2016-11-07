#include "mid_end.h"

me_error_t process(ast_ptr node)
{
    program_node_t* prog = assert_cast(node, program_node_t*, AST_PROGRAM);

(void)prog;

    return ME_ERROR_NONE;
}
