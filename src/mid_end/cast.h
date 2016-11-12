#pragma once

#include "trait.h"
#include "r_type.h"

r_type_t* cast_common_type(r_type_t* t1, r_type_t* t2, bool make_cpy);

r_type_t* cast_add_const(r_type_t* t, bool make_cpy);
r_type_t* cast_add_ptr(r_type_t* t, bool make_cpy);
r_type_t* cast_drop_const(r_type_t* t, bool make_cpy);
r_type_t* cast_drop_ptr(r_type_t* t, bool make_cpy);
