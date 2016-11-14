#pragma once

#include "r_type.h"
#include <stdbool.h>

typedef enum type_size
{
	TS_VOID = 0,
	TS_PTR = -1
} type_size_t;

type_size_t trait_sizeof(r_type_t* t);

bool trait_is_castable_to(r_type_t* t1, r_type_t* t2);

bool trait_is_arithmetic(r_type_t* t);
bool trait_is_const(r_type_t* t);
bool trait_is_ptr(r_type_t* t);

bool trait_is_void(r_type_t* t);
bool trait_is_integral(r_type_t* t);
bool trait_is_floating(r_type_t* t);

bool trait_is_signed(r_type_t* t);
bool trait_is_unsigned(r_type_t* t);

bool trait_is_incomplete(r_type_t* t);
bool trait_is_inbuild(r_type_t* t);
bool trait_is_plain_inbuild(r_type_t* t);
