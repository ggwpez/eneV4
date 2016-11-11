#include "ident_node.h"
#include "../defines.h"

#include <stdio.h>
#include <string.h>

extern char* strdup(char const*);
ast_ptr ident_new(char* str, bool should_copy)
{
    return new(ast, AST_IDENT, ident_new_ng(str, should_copy));
}

ident_node_t* ident_new_ng(char* str, bool should_copy)
{
    assert(str);
    define_ptr(ident_node_t, ret);

    if (should_copy)
    {
        ret->str = strdup(str);
        if (!str)
            return NULL;
        ret->have_to_free = true;
    }
    else
    {
        ret->str = str;
        ret->have_to_free = false;
    }

    return ret;
}

void ident_del(ident_node_t* node)
{
    assert(node);
    assert(node->str);

    if (node->have_to_free)
        free(node->str);

    free(node);
}

void ident_print(ident_node_t* node)
{
    assert(node);
    assert(node->str);

    printf("<ident(%s)>", node->str);
}

int ident_cmp(ident_node_t* node, ident_node_t* y)
{
    assert(y);

    return ident_cmp_str(node, y->str);
}

int ident_cmp_str(ident_node_t* node, char const* str)
{
    assert(node);

    return ident_cmp_str_str(node->str, str);
}

int ident_cmp_str_str(char const* str1, char const* str2)
{
    assert(str1);
    assert(str2);

    return strcmp(str1, str2);
}
