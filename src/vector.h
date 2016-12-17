#pragma once

// Github thinks this is C++, but its pure C11
#ifdef __cplusplus
extern "C"
{
	#include "boost/preprocessor.hpp"
}
#else
	#include "boost/preprocessor.hpp"
#endif

#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define MAKE_VECTOR_H(t)\
typedef struct BOOST_PP_CAT(t, _vec)\
{\
	BOOST_PP_CAT(t, _ptr)* data;\
	BOOST_PP_CAT(t, _ptr)* ptr;\
	size_t capacity;\
	int locked;\
} BOOST_PP_CAT(t, _vec_t);\
\
BOOST_PP_CAT(t, _vec_t)* BOOST_PP_CAT(t, _vec_new_ng)(size_t capacity);\
int BOOST_PP_CAT(t, _vec_free)(BOOST_PP_CAT(t, _vec_t)* v);\
int BOOST_PP_CAT(t, _vec_del)(BOOST_PP_CAT(t, _vec_t)* v);\
BOOST_PP_CAT(t, _vec_t)* BOOST_PP_CAT(t, _vec_resize)(BOOST_PP_CAT(t, _vec_t)* v, size_t new_capacity);\
size_t BOOST_PP_CAT(t, _vec_size)(BOOST_PP_CAT(t, _vec_t)* v);\
BOOST_PP_CAT(t, _ptr) BOOST_PP_CAT(t, _vec_at)(BOOST_PP_CAT(t, _vec_t)* v, size_t i);\
BOOST_PP_CAT(t, _ptr)* BOOST_PP_CAT(t, _vec_aat)(BOOST_PP_CAT(t, _vec_t)* v, size_t i);\
BOOST_PP_CAT(t, _ptr) BOOST_PP_CAT(t, _vec_rat)(BOOST_PP_CAT(t, _vec_t)* v, size_t ri);\
BOOST_PP_CAT(t, _ptr)* BOOST_PP_CAT(t, _vec_arat)(BOOST_PP_CAT(t, _vec_t)* v, size_t ri);\
BOOST_PP_CAT(t, _ptr) BOOST_PP_CAT(t, _vec_push_back)(BOOST_PP_CAT(t, _vec_t)* v, BOOST_PP_CAT(t, _ptr) e);\
BOOST_PP_CAT(t, _ptr) BOOST_PP_CAT(t, _vec_pop_back)(BOOST_PP_CAT(t, _vec_t)* v);\
void BOOST_PP_CAT(t, _vec_print)(BOOST_PP_CAT(t, _vec_t)* v);\
void BOOST_PP_CAT(t, _vec_lock)(BOOST_PP_CAT(t, _vec_t)* v);\
void BOOST_PP_CAT(t, _vec_unlock)(BOOST_PP_CAT(t, _vec_t)* v);


#define MAKE_VECTOR_C(t)\
\
BOOST_PP_CAT(t, _vec_t)* BOOST_PP_CAT(t, _vec_new_ng)(size_t capacity)\
{\
	define_ptr(BOOST_PP_CAT(t, _vec_t), ret);\
	assert(ret);\
\
	ret->locked = 0;\
	ret->capacity = 0;\
	ret->ptr = NULL;\
	ret->data = NULL;\
	BOOST_PP_CAT(t, _vec_resize)(ret, capacity);\
\
	return ret;\
}\
\
int BOOST_PP_CAT(t, _vec_free)(BOOST_PP_CAT(t, _vec_t)* v)\
{\
	assert(v);\
	if (v->locked) return -1;\
	\
	v->locked = 0;\
	v->capacity = 0;\
	v->ptr = NULL;\
	v->data = NULL;\
	\
	if (v->data)\
		free(v->data);\
	free(v);\
	return 0;\
}\
\
int BOOST_PP_CAT(t, _vec_del)(BOOST_PP_CAT(t, _vec_t)* v)\
{\
	assert(v);\
	if (v->locked) return -1;\
\
	for (size_t i = 0; i < BOOST_PP_CAT(t, _vec_size)(v); ++i)      /*TODO optimize*/\
	{\
		BOOST_PP_CAT(t, _ptr) e = BOOST_PP_CAT(t, _vec_at)(v, i);\
		if (e)\
			BOOST_PP_CAT(t, _del)(e);\
		else return -1;\
	}\
\
	return BOOST_PP_CAT(t, _vec_free)(v);\
}\
\
BOOST_PP_CAT(t, _vec_t)* BOOST_PP_CAT(t, _vec_resize)(BOOST_PP_CAT(t, _vec_t)* v, size_t new_capacity)\
{\
	assert(v);\
	if (v->locked) return NULL;\
\
	if (new_capacity)\
	{\
		if (!v->data)		/* init?*/\
		{\
			v->data = (BOOST_PP_CAT(t, _ptr)*)malloc(new_capacity *sizeof(BOOST_PP_CAT(t, _ptr)));\
			assert(v->data);\
			if (!v->data)\
				return NULL;\
			v->capacity = new_capacity;\
			v->ptr = v->data;\
		}\
		else\
		{\
			size_t l = BOOST_PP_CAT(t, _vec_size)(v);			/* needed for moving v->ptr*/\
			BOOST_PP_CAT(t, _ptr)* tmp = (BOOST_PP_CAT(t, _ptr)*)realloc(v->data, new_capacity *sizeof(BOOST_PP_CAT(t, _ptr)));\
\
			assert(tmp);\
			if (!tmp)\
				return NULL;		/* TODO do i have to free failed realloc? YEP*/\
			else\
			{\
				v->data = tmp;\
				v->ptr = &v->data[l];\
			}\
			v->capacity = new_capacity;\
		}\
	}\
	else if (v->data) /* delete all?*/\
	{\
		if (BOOST_PP_CAT(t, _vec_free)(v))\
			return NULL;\
		return v;\
	}\
\
	return v;\
}\
\
size_t BOOST_PP_CAT(t, _vec_size)(BOOST_PP_CAT(t, _vec_t)* v)\
{\
	assert(v);\
	size_t s = !v->data ? 0 : (v->ptr -v->data);\
	assert(s < 10000000 || !"Remove if needed");\
	return s;\
}\
\
BOOST_PP_CAT(t, _ptr) BOOST_PP_CAT(t, _vec_at)(BOOST_PP_CAT(t, _vec_t)* v, size_t i)\
{\
	BOOST_PP_CAT(t, _ptr)* addr = BOOST_PP_CAT(t, _vec_aat)(v, i);\
	if (! addr) return NULL;\
\
	return *addr;\
}\
BOOST_PP_CAT(t, _ptr)* BOOST_PP_CAT(t, _vec_aat)(BOOST_PP_CAT(t, _vec_t)* v, size_t i)\
{\
	assert(v);\
	assert(v->data);\
\
	if (i >= BOOST_PP_CAT(t, _vec_size)(v))\
		return NULL;\
\
	return &v->data[i];\
}\
\
BOOST_PP_CAT(t, _ptr) BOOST_PP_CAT(t, _vec_rat)(BOOST_PP_CAT(t, _vec_t)* v, size_t i)\
{\
	BOOST_PP_CAT(t, _ptr)* addr = BOOST_PP_CAT(t, _vec_arat)(v, i);\
	if (! addr) return NULL;\
\
	return *addr;\
}\
BOOST_PP_CAT(t, _ptr)* BOOST_PP_CAT(t, _vec_arat)(BOOST_PP_CAT(t, _vec_t)* v, size_t i)\
{\
	assert(v);\
	assert(v->data);\
	size_t l = BOOST_PP_CAT(t, _vec_size)(v);\
\
	if (!l || i >= l)\
		return NULL;\
\
	return &v->data[l -i -1];\
}\
\
BOOST_PP_CAT(t, _ptr) BOOST_PP_CAT(t, _vec_push_back)(BOOST_PP_CAT(t, _vec_t)* v, BOOST_PP_CAT(t, _ptr) e)\
{\
	assert(v);\
	assert(e);\
	if (v->locked) return NULL;\
\
	size_t l = BOOST_PP_CAT(t, _vec_size)(v);\
	if (l >= v->capacity)\
	{\
		if (!(v = BOOST_PP_CAT(t, _vec_resize)(v, !v->capacity ? 5 : v->capacity *2)))\
			return NULL;\
	}\
\
	return (*v->ptr++ = e);\
}\
\
BOOST_PP_CAT(t, _ptr) BOOST_PP_CAT(t, _vec_pop_back)(BOOST_PP_CAT(t, _vec_t)* v)\
{\
	assert(v);\
	assert(v->data);\
	if (v->locked) return NULL;\
\
	if (!BOOST_PP_CAT(t, _vec_size)(v))		/* <=> (v->ptr == v->data)*/\
		return NULL;\
\
	return (*--v->ptr);				/* TODO check contract*/\
}\
\
void BOOST_PP_CAT(t, _vec_print)(BOOST_PP_CAT(t, _vec_t)* v)\
{\
	assert(v);\
	size_t l = BOOST_PP_CAT(t, _vec_size)(v);\
\
	printf("<" BOOST_PP_STRINGIZE(t) "_vec(");\
	for (size_t i = 0; i < l; ++i)\
	{\
		BOOST_PP_CAT(t, _ptr) e = BOOST_PP_CAT(t, _vec_at)(v, i);\
		if (e)\
			BOOST_PP_CAT(t, _print)(e);\
		else\
			printf("<null>");\
		printf(i +1 == l ? "" : ",");\
	}\
\
	printf(")>");\
}\
\
void BOOST_PP_CAT(t, _vec_lock)(BOOST_PP_CAT(t, _vec_t)* v)\
{\
	assert(v);\
	v->locked = 1;\
}\
\
void BOOST_PP_CAT(t, _vec_unlock)(BOOST_PP_CAT(t, _vec_t)* v)\
{\
	assert(v);\
	v->locked = 0;\
}
