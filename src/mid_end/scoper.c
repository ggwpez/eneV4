#include "scoper.h"
#include "cast.h"
#include "trait.h"
#include <limits.h>

MAKE_VTABLE_C(ast_visits, scoper_transform_, ast_transform_ptr_t)
int ast_transformable[] = { 1, 1, 1,
							0, 0, 0,
							1, 0, 1,
							0, 0, 1,
							0, 0, 0 };

static_assert(_countof(ast_visits       ) == AST_size, "_countof(ast_visits   ) == AST_size");
static_assert(_countof(ast_transformable) == AST_size, "_countof(ast_transform) == AST_size");

static r_type_t* f_ret_type = NULL;

error_t scoper_process(scope_t* sc, program_node_t* ast)
{
	assert(ast);
	bool got_sc = sc;

	if (! got_sc)
		sc = scope_new();
	scope_enter(sc);

	CHECK_ERR(scoper_transform_program(sc, NULL, ast));

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

	switch (node->t)
	{
		case ATOM_UINT8:
			nt = scope_resolve_inbuild_str(sc, "u8");
			break;
		case ATOM_UINT16:
			nt = scope_resolve_inbuild_str(sc, "u16");
			break;
		case ATOM_UINT32:
			nt = scope_resolve_inbuild_str(sc, "u32");
			break;
		case ATOM_UINT64:
			nt = scope_resolve_inbuild_str(sc, "u64");
			break;

		case ATOM_SINT8:
			nt = scope_resolve_inbuild_str(sc, "i8");
			break;
		case ATOM_SINT16:
			nt = scope_resolve_inbuild_str(sc, "i16");
			break;
		case ATOM_SINT32:
			nt = scope_resolve_inbuild_str(sc, "i32");
			break;
		case ATOM_SINT64:
			nt = scope_resolve_inbuild_str(sc, "i64");
			break;

		case ATOM_FLOAT:
			nt = scope_resolve_inbuild_str(sc, "f32");
			break;
		case ATOM_DOUBLE:
			nt = scope_resolve_inbuild_str(sc, "f64");
			break;

		case ATOM_REF_TO_RES:
			break;
		default:
			PANIC;
	}

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

error_t scoper_create_texp_cast(ast_ptr* parent, texp_node_t* node, r_type_t* t2)
{
	assert(t2);
	assert(node);
	assert(parent);
	r_type_t* t1 = node->type;
	texp_cast_t cast_type = CAST_size;
	assert(!trait_is_ptr(t1) && !trait_is_ptr(t2)/* TODO Unsupported */);

	if (trait_is_same(t1, t2))
		return SUCCESS;
	if (! trait_is_castable_to(t1, t2))
		return SC_BAD_TYPE_CAST;

	size_t s1 = trait_sizeof(t1),
		   s2 = trait_sizeof(t2);
	bool sig1 = trait_is_signed(t1),
		 sig2 = trait_is_signed(t2);

	if (trait_is_integral(t1))
	{
		if (trait_is_integral(t2))		// int -> int
		{
			if (s2 > s1)				// int32 -> int64
				cast_type = sig1 ? CAST_S_EXT : CAST_Z_EXT;
			else if (s2 < s1)			// int64 -> int32
				cast_type = CAST_TRUNC;
		}
		else if (trait_is_floating(t2))	// int -> float
			cast_type = sig1 ? CAST_SI_TO_FP : CAST_UI_TO_FP;
	}
	else if (trait_is_floating(t1))
	{
		if (trait_is_floating(t2))		// float -> float
		{
			if (s2 > s1)				// float -> double
				cast_type = CAST_FP_EXT;
			else if (s2 < s1)			// double -> float
				cast_type = CAST_FP_TRUNC;
		}
		else if (trait_is_integral(t2))	// float -> int
			cast_type = sig2 ? CAST_FP_TO_SI : CAST_FP_TO_UI;
	}
	else if (trait_is_void(t1))
		return SC_BAD_TYPE_CAST;

	if (cast_type == CAST_size)	// nothing to do - wait, this is impossible !
		return INTERNAL;

	texp_node_t* ret = new_ng(texp, *parent, r_type_cpy(t2));
	ret->cast_type = cast_type;

	*parent = new(ast, AST_TEXP, ret);
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

	CHECK_ERR(scoper_create_texp_cast(&node->x, x, nt));	// cast both to commont type
	CHECK_ERR(scoper_create_texp_cast(&node->y, y, nt));

	*ast = new(texp, *ast, nt);
	return SUCCESS;
}

error_t scoper_transform_program(scope_t* sc, ast_ptr* ast, program_node_t* node)
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

error_t scoper_transform_block(scope_t* sc, ast_ptr* ast, block_node_t* node)
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

	error_t ret = scoper_transform_ast(sc, &node->exp);
	CHECK_ERR(ret);
	texp_node_t* texp = assert_cast(node->exp, texp_node_t*, AST_TEXP);

	if (f_ret_type)
		CHECK_ERR(scoper_create_texp_cast(&node->exp, texp, f_ret_type));

	return SUCCESS;
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
	f_ret_type = node->type->r_type;
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
