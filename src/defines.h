#pragma once

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
#define CHECK(v) { if (! (v)) return NULL; }
#define CHECK_ERR(v) { error_t ret_val = (v); if (ret_val != SUCCESS) return ret_val; }

#define PANIC { fprintf(stderr, "Unreachable reached!"), exit(-1); }

#define VEC_START_SIZE 1


#define MAKE_VTABLE_H(table_name, prefix, delegate_t)\
extern delegate_t ast_visits[];

#define MAKE_VTABLE_C(table_name, prefix, delegate_t)\
\
delegate_t table_name[] = {	(delegate_t)BOOST_PP_CAT(prefix, atom), (delegate_t)BOOST_PP_CAT(prefix, unop), (delegate_t)BOOST_PP_CAT(prefix, binop),\
							(delegate_t)BOOST_PP_CAT(prefix, program), (delegate_t)BOOST_PP_CAT(prefix, block), (delegate_t)BOOST_PP_CAT(prefix, return),\
							(delegate_t)BOOST_PP_CAT(prefix, ident), (delegate_t)BOOST_PP_CAT(prefix, type), (delegate_t)BOOST_PP_CAT(prefix, texp),\
							(delegate_t)BOOST_PP_CAT(prefix, var_decl), (delegate_t)BOOST_PP_CAT(prefix, fun_decl), (delegate_t)BOOST_PP_CAT(prefix, fun_call),\
							(delegate_t)BOOST_PP_CAT(prefix, if), (delegate_t)BOOST_PP_CAT(prefix, while), (delegate_t)BOOST_PP_CAT(prefix, for) };
