#pragma once

#include "../ast/common.h"
#include "../defines.h"

typedef enum me_error
{
	MEE_NONE = 0,
	MEE_SCOPER
} me_error_t;

me_error_t process(ast_ptr node);
int process_scoping(program_node_t* prog);
