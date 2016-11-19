#pragma once

#include "../defines.h"
#include "../errors.h"
#include "../ast/common.h"

error_t be_process(program_node_t* node, char const* mod_name, char const* file_name);
