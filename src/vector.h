#pragma once

#include <stddef.h>
#include "boost/preprocessor.hpp"
#include "ast/ast.h"

#define MAKE_VECTOR_H(t)\
	typedef struct BOOST_PP_CAT(t, _vector)\
	{\
		BOOST_PP_CAT(t, _ptr*) data;\
		BOOST_PP_CAT(t, _ptr*) ptr;\
		size_t capacity;\
	} BOOST_PP_CAT(t, _vector_t);\
	\
	BOOST_PP_CAT(t, _vector_t*) BOOST_PP_CAT(t, _vector_new)(size_t capacity);\
	void BOOST_PP_CAT(t, _vector_del)(BOOST_PP_CAT(t, _vector_t*) v);\
	BOOST_PP_CAT(t, _vector_t*) BOOST_PP_CAT(t, _vector_resize)(BOOST_PP_CAT(t, _vector_t*) v, size_t new_capacity);\
	size_t BOOST_PP_CAT(t, _vector_size)(BOOST_PP_CAT(t, _vector_t*) v);\
	BOOST_PP_CAT(t, _ptr) BOOST_PP_CAT(t, _vector_at)(BOOST_PP_CAT(t, _vector_t*) v, size_t i);\
	BOOST_PP_CAT(t, _ptr) BOOST_PP_CAT(t, _vector_rat)(BOOST_PP_CAT(t, _vector_t*) v, size_t ri);\
	BOOST_PP_CAT(t, _ptr) BOOST_PP_CAT(t, _vector_push_back)(BOOST_PP_CAT(t, _vector_t*) v, BOOST_PP_CAT(t, _ptr) e);\
	BOOST_PP_CAT(t, _ptr) BOOST_PP_CAT(t, _vector_pop_back)(BOOST_PP_CAT(t, _vector_t*) v);\
	void BOOST_PP_CAT(t, _vector_print)(BOOST_PP_CAT(t, _vector_t*) v);


MAKE_VECTOR_H(ast)
