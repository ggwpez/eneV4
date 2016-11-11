#include "mid_end.h"
#include "scoper.h"

me_error_t me_process(ast_ptr node)
{
    if (scoper_process(NULL, &node))
        return MEE_SCOPER;

    return MEE_NONE;
}
