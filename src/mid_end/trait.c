#include "trait.h"
#include "cast.h"
#include "inbuild_type.h"
#include "../ast/texp_node.h"

type_size_t trait_sizeof(r_type_t* t)
{
	assert(t);

	t = cast_drop_const(t, false);

	if (trait_is_plain_inbuild(t))
		return t->inbuild->size;
	else if (trait_is_ptr(t))
		return TS_PTR;
	else if (trait_is_void(t))
		return 0;
	else if (trait_is_arr(t))
		return trait_sizeof(t->sub) *t->arr_size;
	else
		PANIC
}

r_type_t* trait_typeof(ast_ptr node)
{
	assert(node);

	if (node->t == AST_TEXP)	// this should be the case, since it went through the scoper
	{
		texp_node_t* texp = node->node;
		return texp->type;
	}
	else
		return NULL;
}

bool trait_is_same(r_type_t* t1, r_type_t* t2)
{
	if (t1 == t2)
		return true;

	assert(t1);
	assert(t2);
	if (t1->mod == R_TYPE_MOD_INBUILD && t2->mod == R_TYPE_MOD_INBUILD)
		return (t1->inbuild == t2->inbuild);
	else if (t1->mod == t2->mod)
		return trait_is_same(t1->sub, t2->sub);
	else
		return false;
}

typedef bool(*trait_ptr)(r_type_t*);
typedef struct
{
	trait_ptr proposition;
	trait_ptr conclusion[5];
} ctuple_t;

ctuple_t castable_table[] = {
								{ trait_is_const, { trait_is_const, trait_is_void } },
								{ trait_is_integral, { trait_is_arithmetic, trait_is_void } },
								{ trait_is_ptr, { trait_is_ptr, trait_is_integral, trait_is_void } },
								{ trait_is_bool, { trait_is_bool, trait_is_void } },
								{ trait_is_arr, { trait_is_arr, trait_is_ptr, trait_is_void } },
								{ trait_is_void, { trait_is_void } },
								{ trait_is_incomplete, { trait_is_void } }
							};

bool trait_is_castable_to(r_type_t* t1, r_type_t* t2)
{
	assert(t1);
	assert(t2);

	bool found = false;
	size_t i, j;
	for (i = 0; i < _countof(castable_table); ++i)
	{
		if (castable_table[i].proposition(t1))
		{
			found = true;
			for (j = 0; j < 5; ++j)
			{
				if (castable_table[i].conclusion[j] && castable_table[i].conclusion[j](t2))
					break;
			}
		}
		if (j == 5)
			return false;
	}

	if (! found)
		return fprintf(stderr, "%s", "Forgot castable-case for: "), r_type_pprint(t1), putc('\n', stderr);
	return true;
}

bool trait_is_arithmetic(r_type_t* t)
{
	return trait_is_ptr(t) || trait_is_integral(t) || trait_is_floating(t);
}

bool trait_is_const(r_type_t* t)
{
	assert(t);

	return t->mod == R_TYPE_MOD_CONST;
}

bool trait_is_ptr(r_type_t* t)
{
	assert(t);

	return t->mod == R_TYPE_MOD_PTR;
}

bool trait_is_arr(r_type_t* t)
{
	assert(t);

	return t->mod == R_TYPE_MOD_ARRAY;
}

bool trait_is_stack_top(r_type_t* t)
{
	assert(t);

	return trait_is_plain_inbuild(t) && (t->inbuild->type == IT_STACK_TOP);
}

bool trait_is_void(r_type_t* t)
{
	assert(t);

	return trait_is_plain_inbuild(t) && (t->inbuild->type == IT_VOID);
}

bool trait_is_integral(r_type_t* t)
{
	assert(t);

	return trait_is_plain_inbuild(t) && (t->inbuild->type >= IT_U8 && t->inbuild->type <= IT_I64);
}

bool trait_is_floating(r_type_t* t)
{
	assert(t);

	return trait_is_plain_inbuild(t) && t->inbuild->is_floating;
}

bool trait_is_bool(r_type_t* t)
{
	return trait_is_plain_inbuild(t) && (t->inbuild->type == IT_BOOL);
}

bool trait_is_signed(r_type_t* t)
{
	assert(t);

	return trait_is_floating(t) || (trait_is_integral(t) && t->inbuild->is_signed);
}

bool trait_is_unsigned(r_type_t* t)
{
	assert(t);

	return trait_is_integral(t) && ! t->inbuild->is_signed;
}

bool trait_is_incomplete(r_type_t* t)
{
	return ! trait_sizeof(t);
}

bool trait_is_inbuild(r_type_t* t)
{
	return trait_is_plain_inbuild(t) || trait_is_inbuild(t->sub);
}

bool trait_is_plain_inbuild(r_type_t* t)
{
	assert(t);

	return t->mod == R_TYPE_MOD_INBUILD
			|| (t->mod == R_TYPE_MOD_CONST && trait_is_plain_inbuild(t->sub));
}
