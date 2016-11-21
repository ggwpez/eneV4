#pragma once

#include "../defines.h"
#include "../errors.h"
#include "../ast/common.h"

#include <llvm-c/Core.h>

error_t il_process(program_node_t* node, const char* mod_name, char** output);

void* il_create_ast(ast_ptr ast);
LLVMValueRef il_create_atom(atom_node_t* node);
LLVMValueRef il_create_unop(unop_node_t* node);
LLVMValueRef il_create_binop(binop_node_t* node);
LLVMModuleRef il_create_program(program_node_t* node);
LLVMBasicBlockRef il_create_block(block_node_t* node);
LLVMValueRef il_create_return(return_node_t* node);
LLVMValueRef il_create_ident(ident_node_t* node);
LLVMTypeRef il_create_type(type_node_t* node);
LLVMValueRef il_create_texp(texp_node_t* node);
LLVMValueRef il_create_cast(cast_node_t* node);
LLVMValueRef il_create_var_decl(var_decl_node_t* node);
LLVMValueRef il_create_fun_decl(fun_decl_node_t* node);
LLVMValueRef il_create_fun_call(fun_call_node_t* node);
LLVMValueRef il_create_if(if_node_t* node);
LLVMValueRef il_create_while(while_node_t* node);
LLVMValueRef il_create_for(for_node_t* node);

typedef void*(*il_creator_ptr_t)(void*);
MAKE_VTABLE_H(il_creates, il_create_, il_creator_ptr_t)
