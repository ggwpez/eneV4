#pragma once

#include "../defines.h"
#include "../errors.h"
#include "../ast/common.h"

error_t me_process(ast_ptr node);
int process_scoping(program_node_t* prog);
