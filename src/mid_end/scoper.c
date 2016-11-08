#include "scoper.h"

ast_visit_ptr ast_visits[] = { (ast_visit_ptr)scoper_visit_atom, (ast_visit_ptr)scoper_visit_unop, (ast_visit_ptr)scoper_visit_binop,
						 (ast_visit_ptr)scoper_visit_program, (ast_visit_ptr)scoper_visit_block, (ast_visit_ptr)scoper_visit_return,
						 (ast_visit_ptr)scoper_visit_ident, (ast_visit_ptr)scoper_visit_type,
						 (ast_visit_ptr)scoper_visit_var_decl, (ast_visit_ptr)scoper_visit_fun_decl, (ast_visit_ptr)scoper_visit_fun_call,
						 (ast_visit_ptr)scoper_visit_if, (ast_visit_ptr)scoper_visit_while, (ast_visit_ptr)scoper_visit_for };

int scoper_process(scope_t* sc, ast_ptr node)
{
	assert(node);
	bool got_sc = ! sc;

	if (! got_sc)
		sc = scope_new();
	scope_enter(sc);

	int ret = scoper_visit_ast(sc, node);
	if (ret)
		return -1;		// TODO dont leak scope

	scope_leave(sc);
	if (! got_sc)
		scope_del(sc);
	return 0;
}

int scoper_visit_ast(scope_t* sc, ast_ptr node)
{
	assert(sc);
	assert(node);
	assert(node->t < AST_size);

	return ast_visits[(int) node->t](sc, node->node);
}

int scoper_visit_atom(scope_t* sc, atom_node_t* node)
{

}

int scoper_visit_unop(scope_t* sc, unop_node_t* node)
{

}

int scoper_visit_binop(scope_t* sc, binop_node_t* node)
{

}

int scoper_visit_program(scope_t* sc, program_node_t* node)
{

}

int scoper_visit_block(scope_t* sc, block_node_t* node)
{

}

int scoper_visit_return(scope_t* sc, return_node_t* node)
{

}

int scoper_visit_ident(scope_t* sc, ident_node_t* node)
{

}

int scoper_visit_type(scope_t* sc, type_node_t* node)
{

}

int scoper_visit_var_decl(scope_t* sc, var_decl_node_t* node)
{

}

int scoper_visit_fun_decl(scope_t* sc, fun_decl_node_t* node)
{

}

int scoper_visit_fun_call(scope_t* sc, fun_call_node_t* node)
{

}

int scoper_visit_if(scope_t* sc, if_node_t* node)
{

}

int scoper_visit_while(scope_t* sc, while_node_t* node)
{

}

int scoper_visit_for(scope_t* sc, for_node_t* node)
{

}

