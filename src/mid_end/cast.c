#include "cast.h"

r_type_t* cast_common_type(r_type_t* t1, r_type_t* t2, bool make_cpy)
{
	assert(t1);
	assert(t2);
	r_type_t* ret = NULL;
	// TODO wont work with const
	if (trait_is_incomplete(t1) || trait_is_incomplete(t2))
		ret = NULL;
	else if (trait_is_void(t1) || trait_is_void(t2))
		ret = trait_is_void(t1) ? t1 : t2;
	else if (trait_is_floating(t1) || trait_is_floating(t2))
	{
		if (trait_is_floating(t1) && trait_is_floating(t2))
			ret = (trait_sizeof(t1) > trait_sizeof(t2)) ? t1 : t2;
		else if (trait_is_floating(t2))
			ret = t2;
		else
			ret = t1;
	}
	else if (trait_is_ptr(t1))
		ret = t1;
	else if (trait_is_ptr(t2))
		ret = t2;
	else if (trait_is_integral(t1) || trait_is_integral(t2))
	{
		if (trait_is_integral(t1) && trait_is_integral(t2))
			ret = (trait_sizeof(t1) > trait_sizeof(t2)) ? t1 : t2;
		else if (trait_is_integral(t2))
			ret = t2;
		else
			ret = t1;
	}
	else
		PANIC

	if (ret && make_cpy)
		ret = r_type_cpy(ret);

	return ret;
}

r_type_t* cast_add_const(r_type_t* t, bool make_cpy)
{
	assert(t);

	r_type_t* cpy = make_cpy ? r_type_cpy(t) : t;
	return r_type_new(R_TYPE_MOD_CONST, NULL, cpy);
}

r_type_t* cast_add_ptr(r_type_t* t, bool make_cpy)
{
	assert(t);

	r_type_t* cpy = make_cpy ? r_type_cpy(t) : t;
	return r_type_new(R_TYPE_MOD_PTR, NULL, cpy);
}

r_type_t* cast_drop_const(r_type_t* t, bool make_cpy)
{
	assert(t);

	if (! trait_is_const(t))
		return make_cpy ? r_type_cpy(t) : t;
	else
		return make_cpy ? r_type_cpy(t->sub) : t->sub;
}

r_type_t* cast_drop_ptr(r_type_t* t, bool make_cpy)
{
	assert(t);

	if (! trait_is_ptr(t))
		return make_cpy ? r_type_cpy(t) : t;
	else
		return make_cpy ? r_type_cpy(t->sub) : t->sub;
}
