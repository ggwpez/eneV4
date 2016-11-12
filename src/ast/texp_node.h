#pragma once

#include "ast.h"
#include "../mid_end/r_type.h"

typedef struct texp_node
{
	ast_ptr exp;
	r_type_t* type;
} texp_node_t;

ast_ptr texp_new(ast_ptr exp, r_type_t* type);
texp_node_t* texp_new_ng(ast_ptr exp, r_type_t* type);
void texp_del(texp_node_t* node);
void texp_print(texp_node_t* node);
