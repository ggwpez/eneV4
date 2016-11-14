#pragma once

#include "../defines.h"
#include "../errors.h"
#include "../ast/common.h"

error_t be_process(ast_ptr node, char const* file_name);
