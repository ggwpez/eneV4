#include "fun_call_node.h"
#include <stdio.h>

ast_ptr fun_call_new(ident_node_t* name, ast_vector_t* args)
{
<<<<<<< HEAD
    return new(fun_call, name, args);
=======
	assert(name);
	assert(args);
	return new(ast, AST_FUN_CALL, new_ng(fun_call, name, args));
>>>>>>> 5cd23b9126ebbbd28e26fdf6ed9f4a696dbab97e
}

fun_call_node_t* fun_call_new_ng(ident_node_t* name, ast_vector_t* args)
{
<<<<<<< HEAD
    assert(name);
    assert(args);
    define_ptr(fun_call_node_t, ret);

    ret->name = name;
    ret->args = args;

    return ret;
=======
	assert(name);
	assert(args);
	define_ptr(fun_call_node_t, ret);

	ret->name = name;
	ret->args = args;

	return ret;
>>>>>>> 5cd23b9126ebbbd28e26fdf6ed9f4a696dbab97e
}

void fun_call_del(fun_call_node_t* node)
{
<<<<<<< HEAD
    assert(node);

    delete(ident, node->name);
    delete(ast_vector, node->args);
    free(node);
=======
	assert(node);

	delete(ident, node->name);
	delete(ast_vector, node->args);
	free(node);
>>>>>>> 5cd23b9126ebbbd28e26fdf6ed9f4a696dbab97e
}

void fun_call_print(fun_call_node_t* node)
{
<<<<<<< HEAD
    assert(node);

    printf("<fun_call("),
        ident_print(node->name), putchar(','),
        ast_vector_print(node->args),
    printf(")>");
=======
	assert(node);

	printf("<fun_call("),
		ident_print(node->name), putchar(','),
		ast_vector_print(node->args),
	printf(")>");
>>>>>>> 5cd23b9126ebbbd28e26fdf6ed9f4a696dbab97e
}
