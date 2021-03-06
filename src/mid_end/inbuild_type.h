#pragma once

#include "../ast/ident_node.h"
#include "../enums.h"
#include <stdint.h>

/* *
 * LSB is the size in byte
 * 8. Bit:
 *		1 on Signed
 * 9. Bit:
 *		1 on Floating
 * */
#define INBUILD_ENUM	(IT_STACK_TOP)\
						(IT_VOID)\
						(IT_BOOL)\
						(IT_U8)\
						(IT_U16)\
						(IT_U32)\
						(IT_U64)\
						(IT_I8)\
						(IT_I16)\
						(IT_I32)\
						(IT_I64)\
						(IT_F32)\
						(IT_F64)\
						(IT_size)
MAKE_ENUM(inbuild, INBUILD_ENUM)

// is_floating => is_signed
typedef struct inbuild_type
{
	inbuild_t type;
	uint8_t size;
	bool is_signed;
	bool is_floating;
	char const* name;
} inbuild_type_t;
void inbuild_type_del(inbuild_type_t* node);
void inbuild_type_print(inbuild_type_t* node);
void inbuild_type_pprint(inbuild_type_t* node);

extern inbuild_type_t inbuild_type_table[];

inbuild_type_t* inbuild_type_resolve_str(char const* name);
inbuild_type_t* inbuild_type_resolve(ident_node_t* name);
