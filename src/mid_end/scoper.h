#pragma once

#include "scope.h"
#include "../errors.h"

///
/// \param sc Can be null, to indicate that it has its own scope
///
error_t scoper_process(scope_t* sc, program_node_t* ast);
// Generic
error_t scoper_transform_ast(scope_t* sc, ast_ptr* ast);
error_t scoper_transform_atom(scope_t* sc, ast_ptr* ast, atom_node_t* node);
error_t scoper_transform_unop(scope_t* sc, ast_ptr* ast, unop_node_t* node);
error_t scoper_transform_binop(scope_t* sc, ast_ptr* ast, binop_node_t* node);
error_t scoper_transform_assign(scope_t* sc, ast_ptr* ast, program_node_t* node);
error_t scoper_transform_program(scope_t* sc, ast_ptr* ast, program_node_t* node);
error_t scoper_transform_block(scope_t* sc, ast_ptr* ast, block_node_t* node);
error_t scoper_transform_return(scope_t* sc, ast_ptr* ast, return_node_t* node);
error_t scoper_transform_ident(scope_t* sc, ast_ptr* ast, ident_node_t* node);
error_t scoper_transform_type(scope_t* sc, ast_ptr* ast, type_node_t* node);
error_t scoper_transform_texp(scope_t* sc, ast_ptr* ast, texp_node_t* node);
error_t scoper_transform_cast(scope_t* sc, ast_ptr* ast, cast_node_t* node);
error_t scoper_transform_var_decl(scope_t* sc, ast_ptr* ast, var_decl_node_t* node);
error_t scoper_transform_fun_decl(scope_t* sc, ast_ptr* ast, fun_decl_node_t* node);
error_t scoper_transform_fun_call(scope_t* sc, ast_ptr* ast, fun_call_node_t* node);
error_t scoper_transform_if(scope_t* sc, ast_ptr* ast, if_node_t* node);
error_t scoper_transform_while(scope_t* sc, ast_ptr* ast, while_node_t* node);
error_t scoper_transform_for(scope_t* sc, ast_ptr* ast, for_node_t* node);

typedef error_t(*ast_transform_ptr_t)(scope_t*, ast_ptr*, void*);
