#include "inbuild_type.hpp"
#include <stdio.h>

inbuild_type_t inbuild_type_table[] =	{ { IT_U8, 8,0,0, "u8" }, { IT_U16, 16,0,0, "u16" }, { IT_U32, 32,0,0, "u32" }, { IT_U64, 64,0,0, "u64" },
										  { IT_I8, 8,1,0, "i8" }, { IT_I16, 16,1,0, "i16" }, { IT_I32, 32,1,0, "i32" }, { IT_I64, 64,1,0, "i64" },
										  { IT_F32, 8,1,1, "f32" }, { IT_F64, 16,1,1, "f64" } };
static_assert(_countof(inbuild_type_table) == IT_size, "inbuild_type_tr_table size wrong");

inbuild_type_t* inbuild_type_resolve(ident_node_t* name)
{
	size_t l = _countof(inbuild_type_table);

	for (size_t i = 0; i < l; ++i)
	{
		if (! ident_cmp_str(name, inbuild_type_table[i].name))
			return &inbuild_type_table[i];
	}

	return NULL;
}

void inbuild_type_del(inbuild_type_t* node)
{
	/* STUB */
	(void)node;
}

void inbuild_type_print(inbuild_type_t* node)
{
	assert(node);
	printf("<inbuild_type(%s)>", node->name);
}
