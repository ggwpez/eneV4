#pragma once

#include "ast.h"
#include <stdbool.h>

typedef struct ident_node
{
	char const* str;
	bool have_to_free;
} ident_node_t;

ast_ptr ident_new(char const* str, bool should_copy);
void ident_del(ident_node_t* node);
void ident_print(ident_node_t* node);
