#include "mid_end.h"
#include "scoper.h"

error_t me_process(program_node_t* node)
{
	if (scoper_process(NULL, node))
		return ME_SCOPER;

	return SUCCESS;
}
