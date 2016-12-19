#pragma once

#include "enums.h"
#include <boost/preprocessor.hpp>

/*typedef enum
{
	SUCCESS = 0,
	INTERNAL,
	UNDERLYING,
	UNKNOWN,
	BAD_ARGS,				// call args are bad
	FILE_NOT_FOUND,

	CON_DOUBLE,
	CON_FUN_REG_AS_VAR,		// Function name already used as variable name
	CON_VAR_REG_AS_FUN,		// vice versa
	CON_TYPE_NOT_FOUND,

	SC_TRANSFORM_TYPE,
	SC_OVERLOAD_FAULT,
	SC_BAD_TYPE_CAST,
	SC_VAR_NOT_FOUND,
	SC_FUN_NOT_FOUND,
	SC_CALL_ARGC_WRONG,		// arg count wrong
	SC_MAIN_MISSING,

	ME_SCOPER,
	ME_BAD_TYPE_CAST,

	BE_BAD_FILE,			// bad file stream
} error_t;*/

MAKE_ENUM(error, (SUCCESS)(INTERNAL)(UNDERLYING)(UNKNOWN)(BAD_ARGS)(FILE_NOT_FOUND)(CON_DOUBLE)(CON_FUN_REG_AS_VAR)(CON_VAR_REG_AS_FUN)(CON_TYPE_NOT_FOUND)(SC_TRANSFORM_TYPE)(SC_OVERLOAD_FAULT)(SC_BAD_TYPE_CAST)(SC_VAR_NOT_FOUND)(SC_FUN_NOT_FOUND)(SC_CALL_ARGC_WRONG)(SC_MAIN_MISSING)(ME_SCOPER)(ME_BAD_TYPE_CAST)(BE_BAD_FILE))

int ERR(error_t type, ...) __attribute__ ((noreturn));
