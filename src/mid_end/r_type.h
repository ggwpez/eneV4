#pragma once

#include "inbuild_type.hpp"

typedef enum r_type_moe
{
	R_TYPE_MOD_INBUILD = 0,		// its atom
	R_TYPE_MOD_PTR,
	R_TYPE_MOD_CONST,
	R_TYPE_MOD_size
} r_type_mode_t;

// Resolved type
typedef struct r_type
{
	r_type_mode_t mod;

	// Either
	inbuild_type_t* inbuild;
	struct r_type* sub;
} r_type_t;
r_type_t* r_type_new(r_type_mode_t mod, inbuild_type_t* inbuild, r_type_t* sub);
void r_type_del(r_type_t* node);
void r_type_print(r_type_t* node);
