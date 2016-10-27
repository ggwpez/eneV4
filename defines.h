#ifndef DEFINES_H
#define DEFINES_H

#include <stdlib.h>

#ifdef new
    #undef new
#endif
#define malloct(t) (t*)(malloc(sizeof(t)))
#define malloc_ptr(t, name) t* name = malloct(t)
#define new(t, ...) (t##_new(__VA_ARGS__))
#define _countof(arr) (sizeof(arr) /sizeof(arr[0]))

#endif // DEFINES_H
