#pragma once

#include "scope.h"

///
/// \param sc Can be null, to indicate that it has its own scope
///
int scoper_process(scope_t* sc, ast_ptr* ast);
// Generic
int scoper_transform_ast(scope_t* sc, ast_ptr* ast);
int scoper_transform_atom(scope_t* sc, ast_ptr* ast);
int scoper_transform_unop(scope_t* sc, ast_ptr* ast);
int scoper_transform_binop(scope_t* sc, ast_ptr* ast);
int scoper_transform_program(scope_t* sc, ast_ptr* ast);
int scoper_transform_block(scope_t* sc, ast_ptr* ast);
int scoper_transform_return(scope_t* sc, ast_ptr* ast);
int scoper_transform_ident(scope_t* sc, ast_ptr* ast);
int scoper_transform_type(scope_t* sc, ast_ptr* ast);
int scoper_transform_texp(scope_t* sc, ast_ptr* ast);
int scoper_transform_var_decl(scope_t* sc, ast_ptr* ast);
int scoper_transform_fun_decl(scope_t* sc, ast_ptr* ast);
int scoper_transform_fun_call(scope_t* sc, ast_ptr* ast);
int scoper_transform_if(scope_t* sc, ast_ptr* ast);
int scoper_transform_while(scope_t* sc, ast_ptr* ast);
int scoper_transform_for(scope_t* sc, ast_ptr* ast);

typedef int(*ast_transform_ptr_t)(scope_t*, ast_ptr*);
extern ast_transform_ptr_t ast_visits[];
