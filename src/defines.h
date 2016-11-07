#pragma once

#include <stdlib.h>
#include <assert.h>

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

#define VEC_START_SIZE 1
