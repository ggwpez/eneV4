#include "scoper.h"
#include "cast.h"
#include "trait.h"

MAKE_VTABLE_C(ast_visits, scoper_transform_, ast_transform_ptr_t)
int ast_transformable[] = { 1, 1, 1,
							0, 0, 0,
							1, 0, 1,
							0, 0, 1,
							0, 0, 0 };

static_assert(_countof(ast_visits       ) == AST_size, "_countof(ast_visits   ) == AST_size");
static_assert(_countof(ast_transformable) == AST_size, "_countof(ast_transform) == AST_size");
error_t scoper_process(scope_t* sc, ast_ptr* ast)
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
	return SUCCESS;
}

error_t scoper_transform_ast(scope_t* sc, ast_ptr* ast)
{
	assert(ast && *ast);
	assert((*ast)->t < AST_size);

	ast_ptr* arg1 = NULL;
	void* arg2 = NULL;

	if (ast_transformable[(int)(*ast)->t])
		arg1 = ast,
		arg2 = (*ast)->node;
	else
		arg2 = (*ast)->node;

	return ast_visits[(*ast)->t](sc, arg1, arg2);
}

error_t scoper_transform_atom(scope_t* sc, ast_ptr* ast, atom_node_t* node)
{
	assert(ast && *ast);

	r_type_t* nt = NULL;

	if (node->t == ATOM_FLOAT)
		nt = scope_resolve_inbuild_str(sc, "f32");
	else if (node->t == ATOM_INT)
		nt = scope_resolve_inbuild_str(sc, "i32");
	else
		return INTERNAL;

	*ast = new(texp, *ast, nt);

	return SUCCESS;
}

error_t scoper_transform_unop(scope_t* sc, ast_ptr* ast, unop_node_t *node)
{
	assert(ast && *ast);

	CHECK_ERR(scoper_transform_ast(sc, &node->node));
	// TODO make unsigned to signed on negation
	texp_node_t* x = assert_cast(node->node, texp_node_t*, AST_TEXP);
	r_type_t* nt = r_type_cpy(x->type);

	*ast = new(texp, *ast, nt);
	return SUCCESS;
}

error_t scoper_transform_binop(scope_t* sc, ast_ptr* ast, binop_node_t *node)
{
	assert(ast && *ast);

	CHECK_ERR(scoper_transform_ast(sc, &node->x));
	CHECK_ERR(scoper_transform_ast(sc, &node->y));

	texp_node_t* x = assert_cast(node->x, texp_node_t*, AST_TEXP),
			   * y = assert_cast(node->y, texp_node_t*, AST_TEXP);

	// TODO look here
	r_type_t* nt = cast_common_type(x->type, y->type, true);

	*ast = new(texp, *ast, nt);
	return SUCCESS;
}

error_t scoper_transform_program(scope_t* sc, ast_ptr* ast, program_node_t *node)
{
	(void)ast;
	assert(node);

	scope_enter(sc);

	// Add inbuild functions

	size_t s = ast_vector_size(node->v);
	ast_vector_lock(node->v);

	for (size_t i = 0; i < s; ++i)
	{
		ast_ptr* obj = ast_vector_aat(node->v, i);
		CHECK_ERR(scoper_transform_ast(sc, obj));
	}

	ast_vector_unlock(node->v);
	scope_leave(sc);
	return SUCCESS;
}

error_t scoper_transform_block(scope_t* sc, ast_ptr* ast, block_node_t *node)
{
	(void)ast;
	assert(node);

	scope_enter(sc);
	size_t s = ast_vector_size(node->v);
	ast_vector_lock(node->v);

	for (size_t i = 0; i < s; ++i)
	{
		ast_ptr* obj = ast_vector_aat(node->v, i);
		CHECK_ERR(scoper_transform_ast(sc, obj));
	}

	ast_vector_unlock(node->v);
	scope_leave(sc);
	return SUCCESS;
}

error_t scoper_transform_return(scope_t* sc, ast_ptr* ast, return_node_t *node)
{
	(void)ast;
	assert(node);

	return scoper_transform_ast(sc, &node->exp);
}

error_t scoper_transform_ident(scope_t* sc, ast_ptr* ast, ident_node_t *node)
{
	assert(ast && *ast);
	r_type_t* nt = NULL;
	var_decl_node_t* decl = scope_get_var(sc, node);

	if (! decl)
		return -1;

	assert(nt = r_type_cpy(decl->type->r_type));

	*ast = new(texp, *ast, nt);
	return SUCCESS;
}

error_t scoper_transform_type(scope_t* sc, ast_ptr* ast, type_node_t *node)
{
	(void)sc, (void)ast, (void)node;
	PANIC;
}

error_t scoper_transform_texp(scope_t* sc, ast_ptr* ast, texp_node_t *node)
{
	(void)sc, (void)ast, (void)node;
	PANIC;
}

error_t scoper_transform_var_decl(scope_t* sc, ast_ptr* ast, var_decl_node_t *node)
{
	(void)ast;
	assert(node);

	CHECK_ERR(scope_add_var(sc, node));

	return SUCCESS;
}

error_t scoper_transform_fun_decl(scope_t* sc, ast_ptr* ast, fun_decl_node_t *node)
{
	(void)ast;
	assert(node);

	CHECK_ERR(scope_transform_type(sc, node->type));					// transform type
	CHECK_ERR(scope_transform_vars(sc, node->args));					// transform args
	CHECK_ERR(scope_add_fun(sc, node));

	scope_enter(sc);
	CHECK_ERR(scope_add_vars(sc, node->args));					// Add args
	CHECK_ERR(scoper_transform_block(sc, NULL, node->code));
	scope_leave(sc);

	return SUCCESS;
}

error_t scoper_transform_fun_call(scope_t* sc, ast_ptr* ast, fun_call_node_t* node)
{
	assert(ast && *ast);

	if (! (node->decl = scope_get_fun(sc, node->name)))
		return -1;

	ast_vector_lock(node->args);
	size_t s = ast_vector_size(node->args);

	for (size_t i = 0; i < s; ++i)
	{
		ast_ptr* obj = ast_vector_aat(node->args, i);
		CHECK_ERR(scoper_transform_ast(sc, obj));
	}

	ast_vector_unlock(node->args);

	r_type_t* nt = r_type_cpy(node->decl->type->r_type);
	*ast = new(texp, *ast, nt);

	return SUCCESS;
}

error_t scoper_transform_if(scope_t* sc, ast_ptr* ast, if_node_t *node)
{
	(void)ast;
	assert(node);

	int ret;

	ret = scoper_transform_ast(sc, &node->cond);
	CHECK_ERR(ret);
	ret = scoper_transform_block(sc, NULL, node->true_node);
	CHECK_ERR(ret);
	ret = scoper_transform_block(sc, NULL, node->else_node);

	return ret;
}

error_t scoper_transform_while(scope_t* sc, ast_ptr* ast, while_node_t *node)
{
	(void)ast;
	assert(node);
	int ret;

	ret = scoper_transform_ast(sc, &node->cond);
	CHECK_ERR(ret);
	ret = scoper_transform_block(sc, NULL, node->true_node);

	return ret;
}

error_t scoper_transform_for(scope_t* sc, ast_ptr* ast, for_node_t *node)
{
	(void)ast;
	assert(node);
	int ret;

	ret = scoper_transform_ast(sc, &node->init);
	CHECK_ERR(ret);
	ret = scoper_transform_ast(sc, &node->cond);
	CHECK_ERR(ret);
	ret = scoper_transform_ast(sc, &node->inc);
	CHECK_ERR(ret);
	ret = scoper_transform_block(sc, NULL, node->block);

	return ret;
}
