#include "scoper.h"

ast_transform_ptr_t ast_visits[] = { (ast_transform_ptr_t)scoper_transform_atom, (ast_transform_ptr_t)scoper_transform_unop, (ast_transform_ptr_t)scoper_transform_binop,
						 (ast_transform_ptr_t)scoper_transform_program, (ast_transform_ptr_t)scoper_transform_block, (ast_transform_ptr_t)scoper_transform_return,
						 (ast_transform_ptr_t)scoper_transform_ident, (ast_transform_ptr_t)scoper_transform_type,
						 (ast_transform_ptr_t)scoper_transform_var_decl, (ast_transform_ptr_t)scoper_transform_fun_decl, (ast_transform_ptr_t)scoper_transform_fun_call,
						 (ast_transform_ptr_t)scoper_transform_if, (ast_transform_ptr_t)scoper_transform_while, (ast_transform_ptr_t)scoper_transform_for };

int scoper_process(scope_t* sc, ast_ptr* ast)
{
	assert(ast);
	bool got_sc = sc;

	if (! got_sc)
		sc = scope_new();
	scope_enter(sc);

	int ret = scoper_transform_ast(sc, ast);
	if (ret)
		return -1;		// TODO dont leak scope

	scope_leave(sc);
	if (! got_sc)
		scope_del(sc);
	return 0;
}

int scoper_transform_ast(scope_t* sc, ast_ptr* ast)
{
	assert(ast && *ast);
	assert((*ast)->t < AST_size);

	if ()

	if (ast_visits[(int)(*ast)->t](sc, ast))
		return -1;

	return 0;
}

int scoper_transform_atom(scope_t* sc, ast_ptr* ast)
{
	assert(ast && *ast);

	atom_node_t* node = (*ast)->node;
	r_type_t* nt = NULL;
	texp_node_t* ne = NULL;

	if (node->t == ATOM_FLOAT)
		nt = scope_resolve_inbuild_str(sc, "f32");
	else if (node->t == ATOM_INT)
		nt = scope_resolve_inbuild_str(sc, "i32");
	else
		return -1;

	ne = new_ng(texp, *ast, nt);
	*ast = new(ast, AST_TEXP, ne);

	return 0;
}

int scoper_transform_unop(scope_t* sc, ast_ptr* ast)
{
	assert(ast && *ast);
	unop_node_t* node = (*ast)->node;

	int ret = scoper_transform_ast(sc, &node->node);
	CHECK_RET(ret);

	// for the first this always returns i32
	r_type_t* nt = scope_resolve_inbuild_str(sc, "i32");
	texp_node_t* ne = new_ng(texp, *ast, nt);

	*ast = new(ast, AST_TEXP, ne);
	return 0;
}

int scoper_transform_binop(scope_t* sc, ast_ptr* ast)
{
	assert(ast && *ast);

	// for the first this always returns i32
	r_type_t* nt = scope_resolve_inbuild_str(sc, "i32");
	texp_node_t* ne = new_ng(texp, *ast, nt);

	*ast = new(ast, AST_TEXP, ne);
	return 0;
}

int scoper_transform_program(scope_t* sc, ast_ptr* ast)
{
	assert(ast && *ast);

	scope_enter(sc);
	program_node_t* node = (*ast)->node;
	size_t s = ast_vector_size(node->v);
	ast_vector_lock(node->v);

	for (size_t i = 0; i < s; ++i)
	{
		ast_ptr* obj = ast_vector_aat(node->v, i);
		int ret = scoper_transform_ast(sc, obj);

		CHECK_RET(ret);
	}

	ast_vector_unlock(node->v);
	scope_leave(sc);
	return 0;
}

int scoper_transform_block(scope_t* sc, ast_ptr* ast)
{
	assert(ast && *ast);

	scope_enter(sc);
	block_node_t* node = (*ast)->node;
	size_t s = ast_vector_size(node->v);
	ast_vector_lock(node->v);

	for (size_t i = 0; i < s; ++i)
	{
		ast_ptr* obj = ast_vector_aat(node->v, i);
		int ret = scoper_transform_ast(sc, obj);

		CHECK_RET(ret);
	}

	ast_vector_unlock(node->v);
	scope_leave(sc);
	return 0;
}

int scoper_transform_return(scope_t* sc, ast_ptr* ast)
{
	assert(ast && *ast);

	return_node_t* node = (*ast)->node;

	return scoper_transform_ast(sc, &node->exp);
}

int scoper_transform_ident(scope_t* sc, ast_ptr* ast)
{
	(void)sc, (void)ast;
	return 0;
}

int scoper_transform_type(scope_t* sc, ast_ptr* ast)
{
	(void)sc, (void)ast;
	PANIC;
}

int scoper_transform_texp(scope_t* sc, ast_ptr* ast)
{
	(void)sc, (void)ast;
	PANIC;
}

int scoper_transform_var_decl(scope_t* sc, ast_ptr* ast)
{
	assert(ast && *ast);

	var_decl_node_t* node = (*ast)->node;
	context_add_t ret = scope_add_var(sc, node);

	if (ret)
		return -1;
	return 0;
}

int scoper_transform_fun_decl(scope_t* sc, ast_ptr* ast)
{
	assert(ast && *ast);

	fun_decl_node_t* node = (*ast)->node;
	context_add_t ret = scope_add_fun(sc, node);

	if (ret)
		return -1;
	return 0;
}

int scoper_transform_fun_call(scope_t* sc, ast_ptr* ast)
{
	assert(ast && *ast);

	fun_call_node_t* node = (*ast)->node;

	if (! (node->decl = scope_get_fun(sc, node->name)))
		return -1;

	ast_vector_lock(node->args);
	size_t s = ast_vector_size(node->args);

	for (size_T i = 0; i < s; ++i)
	{
		ast_ptr* obj = ast_vector_aat(node->args, i);

		int ret	= scoper_transform_ast(sc, obj);
		CHECK_RET(ret);
	}

	ast_vector_unlock(node->args);
	return 0;
}

int scoper_transform_if(scope_t* sc, ast_ptr* ast)
{
	assert(ast && *ast);

	int ret;
	if_node_t* node = (*ast)->node;

	ret = scoper_transform_ast(sc, &node->cond);
	CHECK_RET(ret);
	ret = scoper_transform_block(sc, &node->true_node);
	CHECK_RET(ret);
	ret = scoper_transform_block(sc, &node->else_node);

	return ret;
}

int scoper_transform_while(scope_t* sc, ast_ptr* ast)
{
	assert(ast && *ast);

	int ret;
	while_node_t* node = (*ast)->node;

	ret = scoper_transform_ast(sc, &node->cond);
	CHECK_RET(ret);
	ret = scoper_transform_block(sc, &node->true_node);

	return ret;
}

int scoper_transform_for(scope_t* sc, ast_ptr* ast)
{
	assert(ast && *ast);

}
