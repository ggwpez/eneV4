#include "var_decl_node.h"
#include <stdio.h>

ast_ptr var_decl_new(type_node_t* type, ident_node_t* var_name)
{
	assert(type);
	assert(var_name);
}

void var_decl_del(var_decl_node_t* node)
{
	assert(node);
}

void var_decl_print(var_decl_node_t* node)
{
	assert(node);
}
