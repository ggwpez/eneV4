#ifndef DEFINES_H
#define DEFINES_H

#include <stdlib.h>
#include <assert.h>

#ifdef new
	#undef new
#endif
#ifdef delete
	#undef delete
#endif
#define malloct(t) (t*)(malloc(sizeof(t)))
#define malloc_ptr(t, name) t* name = malloct(t)
#define new(t, ...) (t##_new(__VA_ARGS__))
#define delete(t, var) (t##_del(var))
#define _countof(arr) (sizeof(arr) /sizeof(arr[0]))
#define assert_cast(var, type, e) (assert(var), assert(var->node), assert(var->t == e), (type)var->node)

#define AST_VEC_START_SIZE 1

#endif // DEFINES_H