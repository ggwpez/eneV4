#pragma once

#include "../ast/common.h"
#include "../defines.h"

typedef enum me_error
{
	ME_ERROR_NONE = 0
} me_error_t;

me_error_t process(ast_ptr node);
