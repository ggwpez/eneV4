#pragma once

#include "../defines.h"
#include "../errors.h"
#include "../ast/common.h"

#include <llvm-c/Core.h>

error_t il_process(ast_ptr node, const char* file_name);

error_t il_create_ast(LLVMModuleRef mod, LLVMBuilderRef build, ast_ptr ast);
error_t il_create_atom(LLVMModuleRef mod, LLVMBuilderRef build, atom_node_t* node);
error_t il_create_unop(LLVMModuleRef mod, LLVMBuilderRef build, unop_node_t* node);
error_t il_create_binop(LLVMModuleRef mod, LLVMBuilderRef build, binop_node_t* node);
error_t il_create_program(LLVMModuleRef mod, LLVMBuilderRef build, program_node_t* node);
error_t il_create_block(LLVMModuleRef mod, LLVMBuilderRef build, block_node_t* node);
error_t il_create_return(LLVMModuleRef mod, LLVMBuilderRef build, return_node_t* node);
error_t il_create_ident(LLVMModuleRef mod, LLVMBuilderRef build, ident_node_t* node);
error_t il_create_type(LLVMModuleRef mod, LLVMBuilderRef build, type_node_t* node);
error_t il_create_texp(LLVMModuleRef mod, LLVMBuilderRef build, texp_node_t* node);
error_t il_create_var_decl(LLVMModuleRef mod, LLVMBuilderRef build, var_decl_node_t* node);
error_t il_create_fun_decl(LLVMModuleRef mod, LLVMBuilderRef build, fun_decl_node_t* node);
error_t il_create_fun_call(LLVMModuleRef mod, LLVMBuilderRef build, fun_call_node_t* node);
error_t il_create_if(LLVMModuleRef mod, LLVMBuilderRef build, if_node_t* node);
error_t il_create_while(LLVMModuleRef mod, LLVMBuilderRef build, while_node_t* node);
error_t il_create_for(LLVMModuleRef mod, LLVMBuilderRef build, for_node_t* node);

typedef error_t(*il_creator_ptr_t)(LLVMModuleRef, LLVMBuilderRef, void*);
MAKE_VTABLE_H(il_creates, il_create_, il_creator_ptr_t)
