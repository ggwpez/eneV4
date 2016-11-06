#include "fun_decl.h"
#include <stdio.h>

arg_node_t* arg_new_ng(type_node_t* type, ident_node_t* name)
{
    assert(type);
    assert(name);
    define_ptr(arg_node_t, ret);

    ret->type = type;
    ret->name = name;

    return ret;
}

void arg_del(arg_node_t* node)
{
    assert(node);

    delete(ident, node->name);
    delete(type, node->type);

    free(node);
}

void arg_print(arg_node_t* node)
{
    assert(node);

    printf("<arg("),
        type_print(node->type), putchar(','),
        ident_print(node->name),
    printf(")>");
}

arg_list_node_t* arg_list_new_ng(arg_vector_t* args)
{
    assert(args);
    define_ptr(arg_list_node_t, ret);

    ret->args = args;

    return ret;
}

void arg_list_del(arg_list_node_t* node)
{
    assert(node);

    delete(arg_vector, node->args);

    free(node);
}

void arg_list_print(arg_list_node_t* node)
{
    assert(node);

    printf("<arg_list("),
        arg_vector_print(node->args),
    printf(")>");
}

ast_ptr fun_decl_new(type_node_t* type, ident_node_t* name, block_node_t* code, arg_list_node_t* args)
{
    assert(type);
    assert(name);
    assert(code);
    assert(args);
    define_ptr(fun_decl_node_t, ret);

    ret->type = type;
    ret->name = name;
    ret->code = code;
    ret->args = args;

    return new(ast, AST_FUN_DECL, ret);
}

void fun_decl_del(fun_decl_node_t* node)
{
    assert(node);

    type_del(node->type);
    ident_del(node->name);
    block_del(node->code);
    arg_list_del(node->args);

    free(node);
}

void fun_decl_print(fun_decl_node_t* node)
{
    assert(node);

    printf("<fun_decl("),
        type_print(node->type), putchar(','),
        ident_print(node->name), putchar(','),
        arg_list_print(node->args), putchar(','),
        block_print(node->code),
    printf(")>");
}

MAKE_VECTOR_C(arg)
