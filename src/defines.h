#pragma once

#include "errors.h"
#include <stdlib.h>
#include <assert.h>
#include <boost/preprocessor.hpp>

#ifdef new
	#undef new
#endif
#ifdef delete
	#undef delete
#endif

#define assert_cast(var, type, e) (assert(var), assert(var->node), assert(var->t == e), (type)var->node)
#define VA_ARGS(...) , ##__VA_ARGS__
#define CHECK(v, err, ...) { if (! (v)) { ERR(err VA_ARGS(__VA_ARGS__)); PANIC } }
#define CHECK_ERR(v) { error_t ret_val = (v); if (ret_val != SUCCESS) return ret_val; }
#define PANIC { fprintf(stderr, "PANIC\n"), abort(), exit(-1); }

#define malloct(t) (t*)(malloc(sizeof(t)))
#define define_ptr(t, name) t* name = malloct(t)
#define new(t, ...) (t##_new(__VA_ARGS__))
#define new_ng(t, ...) (t##_new_ng(__VA_ARGS__))
#define delete(t, var) (t##_del(var))
#define _countof(arr) (sizeof(arr) /sizeof(arr[0]))

#define VEC_START_SIZE 1

#define MAKE_VTABLE_C(table_name, prefix, suffix, delegate_t)\
\
static delegate_t table_name[] =\
	{\
		(delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, atom), suffix), (delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, unop), suffix), (delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, binop), suffix),\
		(delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, program), suffix), (delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, block), suffix), (delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, return), suffix),\
		(delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, ident), suffix), (delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, type), suffix), (delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, texp), suffix), (delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, cast), suffix),\
		(delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, var_decl), suffix), (delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, fun_decl), suffix), (delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, fun_call), suffix),\
		(delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, if), suffix), (delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, while), suffix), (delegate_t)BOOST_PP_CAT(BOOST_PP_CAT(prefix, for), suffix)\
	};\
static_assert(_countof(table_name) == AST_size, "Size of table wront does not match AST_size");
