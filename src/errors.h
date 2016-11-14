#pragma once

typedef enum error
{
	SUCCESS = 0,
	INTERNAL,
	UNDERLYING,
	UNKNOWN,
	BAD_ARGS,				// call args are bad

	CON_DOUBLE,
	CON_FUN_REG_AS_VAR,		// Function name already used as variable name
	CON_VAR_REG_AS_FUN,		// vice versa
	CON_TYPE_NOT_FOUND,
	CON_UNDERLYING,			// underlying error

	SC_TRANSFORM_TYPE,

	ME_SCOPER,

	BE_BAD_FILE,			// bad file stream
} error_t;
