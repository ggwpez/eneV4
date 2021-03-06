#include "scoper.h"
#include "scoper_inbuild.h"
#include "cast.h"
#include "trait.h"
#include <limits.h>

MAKE_VTABLE_C(ast_visits, scoper_transform_,, ast_transform_ptr_t)
static bool ast_transformable[] = {	1, 1, 1,
									0, 0, 0,
									1, 0, 1, 1,
									0, 0, 1,
									0, 0, 0 };

static_assert(_countof(ast_transformable) == AST_size, "_countof(ast_transform) == AST_size");

static r_type_t* f_ret_type = NULL;

error_t scoper_process(scope_t* sc, program_node_t* ast)
{
	assert(ast);
	bool got_sc = sc;

	if (! got_sc)
		sc = scope_new();
	{
		scope_enter(sc);
		CHECK_ERR(scoper_transform_program(sc, NULL, ast));
		scope_leave(sc);
	}

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

	if (ast_transformable[(size_t)(*ast)->t])
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
		case ATOM_BOOL:
			nt = scope_resolve_inbuild_str(sc, "bool");
			break;
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
			PANIC
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

	//if (!trait_is_arithmetic(x->type))
		//return fprintf(stderr, "%s\n", "Binary operators can only be applied to [trait_is_arithmetic(t1)] for all t1"), SC_OVERLOAD_FAULT;

	r_type_t* nt = NULL;

	switch (node->t)
	{
		case UNOP_DRF:
			if (! trait_is_ptr(x->type))
				return fprintf(stderr, "%s\n", "Operator: '~' can only be applied to trait_is_ptr(t1) for any t1"), SC_BAD_TYPE_CAST;
			nt = cast_drop_ptr(x->type, true);
			break;
		case UNOP_NOT:
			if (! trait_is_integral(x->type) && ! trait_is_bool(x->type))
				return fprintf(stderr, "%s\n", "Operator: '!' can only be applied to [trait_is_integral(t1) || trait_is_bool(t1)] for any t1"), SC_BAD_TYPE_CAST;
			//break;
		default:
			nt = r_type_cpy(x->type);
	}

	assert(nt);
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

	if (trait_is_same(t1, t2))
		return SUCCESS;
	CHECK(trait_is_castable_to(t1, t2), SC_BAD_TYPE_CAST,);

	if (trait_is_ptr(t1) && trait_is_ptr(t2))	// does this require recursion?
		return scoper_create_texp_cast(parent, node, t2->sub);

	size_t s1 = trait_sizeof(t1),
		   s2 = trait_sizeof(t2);
	bool sig1 = trait_is_signed(t1),
		 sig2 = trait_is_signed(t2);

	if (trait_is_void(t2))				// T -> void
		;
	//else if (trait_is_bool(t2))			// T -> bool
		//cast_type = trait_is_bool(t1) ? CAST_size : CAST_TO_BOOL;
	else if (trait_is_integral(t1))
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
		else if (trait_is_ptr(t2))		// int -> int'
			cast_type = CAST_INT_TO_PTR;
		else
			PANIC
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
		else if (trait_is_ptr(t2))		// float -> int'
			return fprintf(stderr, "%s\n", "Cast from trait_is_floating(t1) to trait_is_ptr(t2) is illegal for all t1, t2"), SC_BAD_TYPE_CAST;
		else
			PANIC
	}
	else if (trait_is_ptr(t1))
	{
		if (trait_is_floating(t2))		// int' -> float
			return fprintf(stderr, "%s\n", "Cast from trait_is_ptr(t1) to trait_is_floating(t2) is illegal for all t1, t2"), SC_BAD_TYPE_CAST;
		else if (trait_is_integral(t2))	// float' -> int
			cast_type = CAST_PTR_TO_INT;
		else if (trait_is_ptr(t2))		// flaot' -> int'
			;
		else
			PANIC
	}
	/*else if (trait_is_bool(t1))
	{
		if (trait_is_floating(t2))		// bool -> float
			return fprintf(stderr, "%s\n", "Cast from trait_is_bool(T1) to trait_is_floating(T2) is illegal for all T1, T2"), SC_BAD_TYPE_CAST;
		else if (trait_is_integral(t2))	// bool -> int
			cast_type = CAST_BOOL_INT;
		else if (trait_is_ptr(t2))		// bool -> T'
			return fprintf(stderr, "%s\n", "Cast from trait_is_bool(T1) to trait_is_ptr(T2) is illegal for all T1, T2"), SC_BAD_TYPE_CAST;
	}*/
	else if (trait_is_void(t1))
		return SC_BAD_TYPE_CAST;
	else
		PANIC

	assert(! (cast_type == CAST_size ^ ! trait_is_castable_to(t1, t2)));

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

	if (node->t == BINOP_ASS)
	{
		r_type_t* var_t = cast_drop_ptr(x->type, true);
		if (! trait_is_ptr(x->type))
			return fprintf(stderr, "%s\n", "Left part of an assignment must be a pointer"), SC_OVERLOAD_FAULT;

		if (! trait_is_same(var_t, y->type))
		{
			if (! trait_is_castable_to(y->type, var_t))
				return fprintf(stderr, "%s\n", "Implicit cast in assignment failed"), SC_BAD_TYPE_CAST;

			CHECK_ERR(scoper_create_texp_cast(&node->y, y, var_t));
		}

		*ast = new(texp, *ast, var_t);
		return SUCCESS;
	}
	else if (node->t == BINOP_ARR)
	{
		if (! trait_is_arr(x->type) || ! trait_is_integral(y->type))
			return fprintf(stderr, "Operator: '#' awaits [trait_is_arr(T1) trait_is_integral(T2)] for all T1, T2"), SC_OVERLOAD_FAULT;

		*ast = new(texp, *ast, cast_drop_arr(x->type, true));
		return SUCCESS;
	}
	else if (!trait_is_integral(x->type) || !trait_is_integral(y->type))	// no implicit casts, LLVM forbits it, maybe add later
		return fprintf(stderr, "%s\n", "Binary arithmetic operators take [trait_is_arithmetic(T1) trait_is_arithmetic(T2)] for all T1, T2"), SC_OVERLOAD_FAULT;
	else
	{
		// TODO look here
		r_type_t* nt = cast_common_type(x->type, y->type, true);

		CHECK(nt, SC_BAD_TYPE_CAST,);
		CHECK_ERR(scoper_create_texp_cast(&node->x, x, nt));	// cast both to common type
		CHECK_ERR(scoper_create_texp_cast(&node->y, y, nt));

		*ast = new(texp, *ast, nt);
		return SUCCESS;
	}
}

error_t scoper_transform_program(scope_t* sc, ast_ptr* ast, program_node_t* node)
{
	(void)ast;
	assert(node);

	scope_enter(sc);

	size_t s = ast_vec_size(node->v);
	ast_vec_lock(node->v);

	for (size_t i = 0; i < s; ++i)
	{
		ast_ptr* obj = ast_vec_aat(node->v, i);
		CHECK_ERR(scoper_transform_ast(sc, obj));
	}{
		ident_node_t* tmp = ident_new_ng((char*)"main", false);

		if (! scope_get_fun(sc, tmp))
			return fprintf(stderr, "%s\n", "Missing 'main' function"), SC_MAIN_MISSING;

		ident_del(tmp);
	}

	ast_vec_unlock(node->v);
	scope_leave(sc);
	return SUCCESS;
}

error_t scoper_transform_block(scope_t* sc, ast_ptr* ast, block_node_t* node)
{
	(void)ast;
	assert(node);

	scope_enter(sc);
	size_t s = ast_vec_size(node->v);
	ast_vec_lock(node->v);

	for (size_t i = 0; i < s; ++i)
	{
		ast_ptr* obj = ast_vec_aat(node->v, i);
		CHECK_ERR(scoper_transform_ast(sc, obj));
	}

	ast_vec_unlock(node->v);
	scope_leave(sc);
	return SUCCESS;
}

error_t scoper_transform_return(scope_t* sc, ast_ptr* ast, return_node_t *node)
{
	(void)ast;
	assert(node);

	CHECK_ERR(scoper_transform_ast(sc, &node->exp));
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

	CHECK(decl, (fprintf(stderr, "Var: '%s' undeclared", node->str), SC_VAR_NOT_FOUND),);
	assert(nt = r_type_cpy(decl->type->r_type));

	*ast = new(texp, *ast, nt);
	return SUCCESS;
}

error_t scoper_transform_type(scope_t* sc, ast_ptr* ast, type_node_t* node)
{
	(void)sc, (void)ast, (void)node;
	PANIC
}

error_t scoper_transform_texp(scope_t* sc, ast_ptr* ast, texp_node_t* node)
{
	(void)sc, (void)ast, (void)node;
	PANIC
}

error_t scoper_transform_cast(scope_t* sc, ast_ptr* ast, cast_node_t* node)
{
	assert(sc && ast && *ast && node);

	CHECK_ERR(scoper_transform_ast(sc, &node->node));

	texp_node_t* x = assert_cast(node->node, texp_node_t*, AST_TEXP);
	r_type_t* nt = scope_resolve_ur_type(sc, node->t);

	CHECK(nt, SC_TRANSFORM_TYPE,);
	CHECK_ERR(scoper_create_texp_cast(&node->node, x, nt));	// TODO very ugly, also leaking node

	{
		ur_type_del(node->t);
	}

	r_type_del(nt);
	*ast = node->node;
	//*ast = new(texp, *ast, nt);
	return SUCCESS;
}

error_t scoper_transform_var_decl(scope_t* sc, ast_ptr* ast, var_decl_node_t* node)
{
	(void)ast;
	assert(node);

	CHECK_ERR(scope_transform_type(sc, node->type));
	//if (! trait_is_arr(node->type->r_type))
		node->type->r_type = cast_add_ptr(node->type->r_type, false);
	CHECK_ERR(scope_add_var(sc, node));

	return SUCCESS;
}

// Shut your eyes, spaghetti code ahead!
error_t scoper_transform_fun_decl(scope_t* sc, ast_ptr* ast, fun_decl_node_t* node)
{
	(void)ast;
	assert(node);

	CHECK_ERR(scope_transform_type(sc, node->type));					// transform return	type
	CHECK_ERR(scope_transform_vars(sc, node->args));					// transform arg	types

	fun_decl_node_t* found = NULL;
	if (found = scope_get_fun(sc, node->name))
	{
		if (found->has_implementation)
		{
			if (node->has_implementation)
				return fprintf(stderr, "Function: '%s' implemented twice\n", node->name->str), SC_OVERLOAD_FAULT;
			else
				return SUCCESS;
		}
		else	// update it TODO check equality, because no overloading jet
			memcpy(found, node, sizeof(fun_decl_node_t));	// TODO impure
	}
	else
		CHECK_ERR(scope_add_fun(sc, node));

	if (node->has_implementation)
	{// Generate code
		{// Add pointers (needed in the block)
			size_t s = var_decl_vec_size(node->args);
			for (size_t i = 0; i < s; ++i)
			{
				var_decl_node_t* decl = var_decl_vec_at(node->args, i);
				decl->type->r_type = cast_add_ptr(decl->type->r_type, true);
			}
		}

		scope_enter(sc);
		CHECK_ERR(scope_add_vars(sc, node->args));					// Add args
		f_ret_type = node->type->r_type;
		CHECK_ERR(scoper_transform_block(sc, NULL, node->code));
		scope_leave(sc);

		{// Remove pointers, superfluous in back-end
			size_t s = var_decl_vec_size(node->args);
			for (size_t i = 0; i < s; ++i)
			{
				var_decl_node_t* decl = var_decl_vec_at(node->args, i);
				decl->type->r_type = cast_drop_ptr(decl->type->r_type, true);
			}
		}
	}

	return SUCCESS;
}

error_t scoper_transform_fun_call(scope_t* sc, ast_ptr* ast, fun_call_node_t* node)
{
	assert(ast && *ast);

	error_t mb_inbuild = scoper_transform_inbuild_fun_call(sc, ast, node);
	if (mb_inbuild == SUCCESS || mb_inbuild != SC_FUN_NOT_FOUND)
		return mb_inbuild;

	if (! (node->decl = scope_get_fun(sc, node->name)))
		return fprintf(stderr, "Function: '%s' unknown", node->name->str), SC_FUN_NOT_FOUND;

	ast_vec_lock(node->args);
	size_t s = ast_vec_size(node->args);

	for (size_t i = 0; i < s; ++i)
	{
		ast_ptr* obj = ast_vec_aat(node->args, i);
		CHECK_ERR(scoper_transform_ast(sc, obj));
	}

	ast_vec_unlock(node->args);

	r_type_t* nt = r_type_cpy(node->decl->type->r_type);
	*ast = new(texp, *ast, nt);

	return SUCCESS;
}

error_t scoper_transform_if(scope_t* sc, ast_ptr* ast, if_node_t *node)
{
	(void)ast;
	assert(node);

	CHECK_ERR(scoper_transform_ast(sc, &node->cond));
	r_type_t* t = trait_typeof(node->cond);
	if (! trait_is_bool(t))
		return fprintf(stderr, "If awaits an [trait_is_bool(T)] for any T"), SC_BAD_TYPE_CAST;

	CHECK_ERR(scoper_transform_block(sc, NULL, node->true_node));
	CHECK_ERR(scoper_transform_block(sc, NULL, node->else_node));

	return SUCCESS;
}

error_t scoper_transform_while(scope_t* sc, ast_ptr* ast, while_node_t *node)
{
	(void)ast;
	assert(node);

	CHECK_ERR(scoper_transform_ast(sc, &node->cond));
	return scoper_transform_block(sc, NULL, node->true_node);
}

error_t scoper_transform_for(scope_t* sc, ast_ptr* ast, for_node_t *node)
{
	(void)ast;
	assert(node);

	CHECK_ERR(scoper_transform_ast(sc, &node->init));
	CHECK_ERR(scoper_transform_ast(sc, &node->cond));
	CHECK_ERR(scoper_transform_ast(sc, &node->inc));
	return scoper_transform_block(sc, NULL, node->block);
}
