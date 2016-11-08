#pragma once

#include "scope.h"

///
/// \param sc Can be null, to indicate that it has its own scope
///
int scoper_process(scope_t* sc, ast_ptr node);
// Generic
int scoper_visit_ast(scope_t* sc, ast_ptr node);
int scoper_visit_atom(scope_t* sc, atom_node_t* node);
int scoper_visit_unop(scope_t* sc, unop_node_t* node);
int scoper_visit_binop(scope_t* sc, binop_node_t* node);
int scoper_visit_program(scope_t* sc, program_node_t* node);
int scoper_visit_block(scope_t* sc, block_node_t* node);
int scoper_visit_return(scope_t* sc, return_node_t* node);
int scoper_visit_ident(scope_t* sc, ident_node_t* node);
int scoper_visit_type(scope_t* sc, type_node_t* node);
int scoper_visit_var_decl(scope_t* sc, var_decl_node_t* node);
int scoper_visit_fun_decl(scope_t* sc, fun_decl_node_t* node);
int scoper_visit_fun_call(scope_t* sc, fun_call_node_t* node);
int scoper_visit_if(scope_t* sc, if_node_t* node);
int scoper_visit_while(scope_t* sc, while_node_t* node);
int scoper_visit_for(scope_t* sc, for_node_t* node);

typedef int(*ast_visit_ptr)(scope_t*, void*);
extern ast_visit_ptr ast_visits[];
