#pragma once

#include "ast.h"
#include "../enums.h"
#include "../mid_end/r_type.h"

#define CAST_ENUM	(CAST_TRUNC = 30)\
					(CAST_Z_EXT)\
					(CAST_S_EXT)\
					(CAST_FP_TO_UI)\
					(CAST_FP_TO_SI)\
					(CAST_UI_TO_FP)\
					(CAST_SI_TO_FP)\
					(CAST_FP_TRUNC)\
					(CAST_FP_EXT)\
					(CAST_PTR_TO_INT)\
					(CAST_INT_TO_PTR)\
					(CAST_BITCAST)\
					(CAST_TO_BOOL)\
					(CAST_BOOL_INT)\
					(CAST_size)
MAKE_ENUM(texp_cast, CAST_ENUM)

typedef struct texp_node
{
	ast_ptr exp;
	r_type_t* type;

	texp_cast_t cast_type;
} texp_node_t;

ast_ptr texp_new(ast_ptr exp, r_type_t* type);
texp_node_t* texp_new_ng(ast_ptr exp, r_type_t* type);
void texp_del(texp_node_t* node);
void texp_print(texp_node_t* node);
