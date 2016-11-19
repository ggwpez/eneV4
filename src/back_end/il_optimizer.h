#pragma once

#include "../errors.h"
#include <llvm-c/Core.h>

error_t il_optimize(LLVMModuleRef mod, unsigned level);
