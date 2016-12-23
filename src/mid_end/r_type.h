#pragma once

#include "inbuild_type.h"

typedef enum r_type_mod
{
	R_TYPE_MOD_INBUILD = 0,		// its atom
	R_TYPE_MOD_PTR = 1,
	R_TYPE_MOD_CONST = 2,
	R_TYPE_MOD_ARRAY = 3,
	R_TYPE_MOD_size
} r_type_mod_t;

// Resolved type
typedef struct r_type
{
	r_type_mod_t mod;

	// Either
	inbuild_type_t* inbuild;
	struct r_type* sub;

	size_t arr_size;
} r_type_t;
r_type_t* r_type_new(r_type_mod_t mod, inbuild_type_t* type, r_type_t* sub, size_t elements);
r_type_t* r_type_cpy(r_type_t* obj);
void r_type_del(r_type_t* node);
void r_type_print(r_type_t* node);
void r_type_pprint(r_type_t* node);
