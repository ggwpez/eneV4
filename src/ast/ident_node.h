#pragma once

#include "ast.h"
#include <stdbool.h>

typedef struct ident_node
{
	char* str;
	bool have_to_free;
} ident_node_t;

ast_ptr ident_new(char* str, bool should_copy);
ident_node_t* ident_new_ng(char* str, bool should_copy);				// non generic version
void ident_del(ident_node_t* node);
void ident_print(ident_node_t* node);
int ident_cmp_str(ident_node_t* node, ident_node_t* y);
