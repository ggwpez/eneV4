#include "mid_end.h"
#include "scoper.h"

error_t me_process(ast_ptr node)
{
	if (scoper_process(NULL, &node))
		return ME_SCOPER;

	return SUCCESS;
}
