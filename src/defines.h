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

#define malloct(t) (t*)(malloc(sizeof(t)))
#define define_ptr(t, name) t* name = malloct(t)
#define new(t, ...) (t##_new(__VA_ARGS__))
#define new_ng(t, ...) (t##_new_ng(__VA_ARGS__))
#define delete(t, var) (t##_del(var))
#define _countof(arr) (sizeof(arr) /sizeof(arr[0]))
#define assert_cast(var, type, e) (assert(var), assert(var->node), assert(var->t == e), (type)var->node)
#define RAISE(v, err) { if (! (v)) return (err); }
#define CHECK_ERR(v) { error_t ret_val = (v); if (ret_val != SUCCESS) return ret_val; }

#define PANIC { fprintf(stderr, "PANIC\n"), abort(), exit(-1); }

#define VEC_START_SIZE 1

#define MAKE_VTABLE_C(table_name, prefix, delegate_t)\
\
static delegate_t table_name[] = {	(delegate_t)BOOST_PP_CAT(prefix, atom), (delegate_t)BOOST_PP_CAT(prefix, unop), (delegate_t)BOOST_PP_CAT(prefix, binop),\
							(delegate_t)BOOST_PP_CAT(prefix, program), (delegate_t)BOOST_PP_CAT(prefix, block), (delegate_t)BOOST_PP_CAT(prefix, return),\
							(delegate_t)BOOST_PP_CAT(prefix, ident), (delegate_t)BOOST_PP_CAT(prefix, type), (delegate_t)BOOST_PP_CAT(prefix, texp), (delegate_t)BOOST_PP_CAT(prefix, cast),\
							(delegate_t)BOOST_PP_CAT(prefix, var_decl), (delegate_t)BOOST_PP_CAT(prefix, fun_decl), (delegate_t)BOOST_PP_CAT(prefix, fun_call),\
							(delegate_t)BOOST_PP_CAT(prefix, if), (delegate_t)BOOST_PP_CAT(prefix, while), (delegate_t)BOOST_PP_CAT(prefix, for) };\
static_assert(_countof(table_name) == AST_size, "Size of table wront does not match AST_size");
