#pragma once

#include "ast.h"
#include "ident_node.h"
#include "../mid_end/ur_type.h"
#include "../mid_end/r_type.h"
#include <stdbool.h>

typedef struct type_node
{
	ur_type_t* ur_type;
	r_type_t* r_type;
} type_node_t;

ast_ptr type_new(ur_type_t* type);
type_node_t* type_new_ng(ur_type_t* type);
void type_del(type_node_t* node);
void type_print(type_node_t* node);
