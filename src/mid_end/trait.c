#include "trait.h"
#include "cast.h"
#include "inbuild_type.hpp"

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
	else
		PANIC;
}

/*bool trait_is_castable_to(r_type_t* t1, r_type_t* t2)
{
	return true;
}*/

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

bool trait_is_void(r_type_t* t)
{
	assert(t);

	return trait_is_plain_inbuild(t) && (t->inbuild->type == IT_VOID);
}

bool trait_is_integral(r_type_t* t)
{
	assert(t);

	return t->mod == R_TYPE_MOD_INBUILD && !t->inbuild->is_floating;
}

bool trait_is_floating(r_type_t* t)
{
	assert(t);

	return t->mod == R_TYPE_MOD_INBUILD && t->inbuild->is_floating;
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

	return t->mod == R_TYPE_MOD_INBUILD;
}
