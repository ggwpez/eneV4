#pragma once

#include "../defines.h"
#include "../errors.h"
#include "../ast/common.h"

error_t me_process(program_node_t* node);
int process_scoping(program_node_t* prog);
