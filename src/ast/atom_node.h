#pragma once

#include "ast.h"

typedef enum atom
{
	ATOM_INT,
	ATOM_FLOAT,
	ATOM_REF_TO_RES,			// a reference to the value on below in the stack
	ATOM_size
} atom_t;

typedef struct atom_node
{
	atom_t t;

	union
	{
		int v0;
		float v1;
		ast_ptr v2;
	};
} atom_node_t;

ast_ptr atom_new(atom_t t, void const* value);
void atom_del(atom_node_t* val);
void atom_print(atom_node_t* val);
