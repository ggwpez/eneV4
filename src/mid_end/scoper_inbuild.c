#include "scoper_inbuild.h"
#include "trait.h"

typedef struct
{
	char const* name;
	ast_transform_ptr_t f;
} inbuild_f;

static inbuild_f inbuilds[] = {
								{ "__typeof", (ast_transform_ptr_t)scoper_inbuild_typeof },
								{ "__is_ptr", (ast_transform_ptr_t)scoper_inbuild_trait_is_ptr },
								{ "__is_const", (ast_transform_ptr_t)scoper_inbuild_trait_is_const }
							  };

error_t scoper_transform_inbuild_fun_call(scope_t* sc, ast_ptr* ast, fun_call_node_t* node)
{
	for (size_t i = 0; i < _countof(inbuilds); ++i)
		if (! ident_cmp_str(node->name, inbuilds[i].name))
		{
			if (ast_vec_size(node->args) > 1)
				return fprintf(stderr, "Function: '%s' is unary", node->name->str), SC_CALL_ARGC_WRONG;

			return inbuilds[i].f(sc, ast, node);
		}

	return SC_FUN_NOT_FOUND;
}

error_t scoper_inbuild_typeof(scope_t* sc, ast_ptr* ast, fun_call_node_t* node)
{
	ast_ptr* arg = ast_vec_aat(node->args, 0);
	CHECK_ERR(scoper_transform_ast(sc, arg));

	r_type_t* t = trait_typeof(*arg);
	CHECK(t, INTERNAL);

	{
		texp_node_t* texp = assert_cast((*arg), texp_node_t*, AST_TEXP);
		printf("__typeof("), ast_print(texp->exp), printf(") = ["), r_type_pprint(t), puts("]");
	}

	int smth = 0;
	*ast = texp_new(new(atom, ATOM_UINT8, &smth), scope_resolve_inbuild_str(sc, "void"));		// return 0[void];
	return SUCCESS;
}

error_t scoper_inbuild_trait_is_ptr(scope_t* sc, ast_ptr* ast, fun_call_node_t* node)
{
	ast_ptr* arg = ast_vec_aat(node->args, 0);
	CHECK_ERR(scoper_transform_ast(sc, arg));

	r_type_t* t = trait_typeof(*arg);
	CHECK(t, INTERNAL);

	int smth = trait_is_ptr(t);
	*ast = texp_new(new(atom, ATOM_UINT8, &smth), scope_resolve_inbuild_str(sc, "u8"));
	return SUCCESS;
}

error_t scoper_inbuild_trait_is_const(scope_t* sc, ast_ptr* ast, fun_call_node_t* node)
{
	ast_ptr* arg = ast_vec_aat(node->args, 0);
	CHECK_ERR(scoper_transform_ast(sc, arg));

	r_type_t* t = trait_typeof(*arg);
	CHECK(t, INTERNAL);

	int smth = trait_is_const(t);
	*ast = texp_new(new(atom, ATOM_UINT8, &smth), scope_resolve_inbuild_str(sc, "u8"));
	return SUCCESS;
}
