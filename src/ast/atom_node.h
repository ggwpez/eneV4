#pragma once

#include "ast.h"
#include "../mid_end/r_type.h"

typedef enum value
{
	ATOM_BOOL,
	ATOM_SINT8,
	ATOM_SINT16,
	ATOM_SINT32,
	ATOM_SINT64,
	ATOM_UINT8,
	ATOM_UINT16,
	ATOM_UINT32,
	ATOM_UINT64,
	ATOM_FLOAT,
	ATOM_DOUBLE,
	ATOM_REF_TO_RES,			// a reference to the value on below in the stack
	ATOM_size
} atom_t;

typedef struct atom_node
{
	atom_t t;

	union
	{
		bool	 boolean;
		uint8_t  u8;
		uint16_t u16;
		uint32_t u32;
		uint64_t u64;

		int8_t  s8;
		int16_t s16;
		int32_t s32;
		int64_t s64;

		float f32;
		double f64;
	};
} atom_node_t;

ast_ptr atom_new(atom_t t, void const* value);
void atom_del(atom_node_t* val);
void atom_print(atom_node_t* val);
