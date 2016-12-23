#pragma once

#define MAKE_ENUM(name, seq)\
extern char const* ast_type_strings[];\
typedef enum\
{\
	BOOST_PP_SEQ_ENUM(seq)\
} BOOST_PP_CAT(name, _t);


#define HELPER_ESTR2(r, _, e) (BOOST_PP_STRINGIZE(e))
#define HELPER_ESTR(seq) BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(HELPER_ESTR2, _, seq))

#define MAKE_ENUM_STRINGS(name, seq)\
char const* BOOST_PP_CAT(name, _strings)[] = { HELPER_ESTR(seq) };
