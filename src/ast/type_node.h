#pragma once

#include "ast.h"
#include "ident_node.h"
#include <stdbool.h>

typedef enum ur_type_mod
{
	UR_TYPE_MOD_ID,		// ident, eg. "i8", "float"…
	UR_TYPE_MOD_PTR,
	UR_TYPE_MOD_CONST,
	UR_TYPE_MOD_size
} ur_type_mod_t;

typedef struct ur_type
{
	ur_type_mod_t mod;

	union
	{
		ast_ptr id;
		struct ur_type* sub;
	};

} ur_type_t;
ur_type_t* ur_type_new(ur_type_mod_t mod, ast_ptr id, ur_type_t* sub);
void ur_type_del(ur_type_t* node);
void ur_type_print(ur_type_t* node);

typedef struct type_node
{
	ur_type_t* type;
} type_node_t;

ast_ptr type_new(ur_type_t* type);
void type_del(type_node_t* node);
void type_print(type_node_t* node);
