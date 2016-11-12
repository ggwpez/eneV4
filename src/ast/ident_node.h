#pragma once

#include "ast.h"
#include <stdbool.h>

typedef struct ident_node
{
	char* str;
	bool should_free;
} ident_node_t;

ast_ptr ident_new(char* str, bool should_free);
ident_node_t* ident_new_ng(char* str, bool should_free);				// non generic version
void ident_del(ident_node_t* node);
void ident_print(ident_node_t* node);
int ident_cmp(ident_node_t* node, ident_node_t* y);
int ident_cmp_str(ident_node_t* node, char const* str);
int ident_cmp_str_str(char const* str1, char const* str2);
