#include "atom_node.h"
#include <stdio.h>
#include <inttypes.h>

MAKE_ENUM_STRINGS(atom, ATOM_ENUM)

ast_ptr atom_new(atom_t t, void const* value)
{
	define_ptr(atom_node_t, ret);
	ret->t = t;

	switch (t)
	{
		case ATOM_STR:
			ret->str = strdup((char const*)value);
			break;
		case ATOM_BOOL:
			ret->boolean = (bool)value;
			break;
		case ATOM_UINT8:
			ret->u8  = *(uint8_t*)value;
			break;
		case ATOM_UINT16:
			ret->u16 = *(uint16_t*)value;
			break;
		case ATOM_UINT32:
			ret->u32 = *(uint32_t*)value;
			break;
		case ATOM_UINT64:
			ret->u64 = *(uint64_t*)value;
			break;

		case ATOM_SINT8:
			ret->s8  = *(int8_t*)value;
			break;
		case ATOM_SINT16:
			ret->s16 = *(int16_t*)value;
			break;
		case ATOM_SINT32:
			ret->s32 = *(int32_t*)value;
			break;
		case ATOM_SINT64:
			ret->s64 = *(int64_t*)value;
			break;

		case ATOM_FLOAT:
			ret->f32 = *(float*)value;
			break;
		case ATOM_DOUBLE:
			ret->f64 = *(double*)value;
			break;

		case ATOM_REF_TO_RES:
			break;
		default:
			PANIC
	}

	return new(ast, AST_ATOM, (void*)ret);
}

void atom_del(atom_node_t* val)
{
	assert(val);

	if (val->t == ATOM_STR)
		free(val->str);
	val->t = ATOM_size;

	free(val);
}

void atom_print(atom_node_t* val)
{
	assert(val);

	if (val->t >= ATOM_UINT8 && val->t <= ATOM_UINT64)
		printf("<int=%" PRIu64 ">", val->u64);
	else if (val->t >= ATOM_SINT8 && val->t <= ATOM_SINT64)
		printf("<int=%" PRId64 ">", val->s64);
	else if (val->t == ATOM_FLOAT)
		printf("<float=%f>", val->f32);
	else if (val->t == ATOM_DOUBLE)
		printf("<float=%f>", val->f64);
	else if (val->t == ATOM_STR)
		printf("<str=%s>", val->str);
	else if (val->t == ATOM_REF_TO_RES)
		printf("<&top>");
	else
		PANIC
}
