#pragma once

#include "scoper.h"

error_t scoper_transform_inbuild_fun_call(scope_t* sc, ast_ptr* ast, fun_call_node_t* node);

error_t scoper_inbuild_trait_is_ptr(scope_t* sc, ast_ptr* ast, fun_call_node_t* node);
error_t scoper_inbuild_trait_is_const(scope_t* sc, ast_ptr* ast, fun_call_node_t* node);
error_t scoper_inbuild_typeof(scope_t* sc, ast_ptr* ast, fun_call_node_t* node);
