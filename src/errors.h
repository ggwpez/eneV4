#pragma once

#include "enums.h"
#include <stdarg.h>
#include <boost/preprocessor.hpp>

#define ERROR_ENUM	(SUCCESS)\
					(ASSERT)\
					(UNDERLYING)\
					(UNKNOWN)\
					(BAD_ARGS)\
					(FILE_NOT_FOUND)\
					(PARSING_FAILED)\
					(CON_DOUBLE)\
					(CON_FUN_REG_AS_VAR)\
					(CON_VAR_REG_AS_FUN)\
					(CON_TYPE_NOT_FOUND)\
					(SC_TRANSFORM_TYPE)\
					(SC_OVERLOAD_FAULT)\
					(SC_BAD_TYPE_CAST)\
					(SC_VAR_NOT_FOUND)\
					(SC_FUN_NOT_FOUND)\
					(SC_CALL_ARGC_WRONG)\
					(SC_MAIN_MISSING)\
					(ME_SCOPER)\
					(ME_BAD_TYPE_CAST)\
					(BE_BAD_FILE)\
					(ERROR_SIZE)

MAKE_ENUM(error, ERROR_ENUM)

void ERR(error_t type, ...) __attribute__ ((noreturn));

void err_success(va_list ap);
void err_assert(va_list ap);
void err_underlying(va_list ap);
void err_unknown(va_list ap);
void err_bad_args(va_list ap);
void err_file_not_found(va_list ap);
void err_parsing_failed(va_list ap);
void err_con_double(va_list ap);
void err_con_fun_reg_as_var(va_list ap);
void err_con_var_reg_as_fun(va_list ap);
void err_con_type_not_found(va_list ap);
void err_sc_transform_type(va_list ap);
void err_sc_overload_fault(va_list ap);
void err_sc_bad_type_cast(va_list ap);
void err_sc_var_not_found(va_list ap);
void err_sc_fun_not_found(va_list ap);
void err_sc_call_argc_wrong(va_list ap);
void err_sc_main_missing(va_list ap);
void err_me_scoper(va_list ap);
void err_me_bad_type_cast(va_list ap);
void err_be_bad_file(va_list ap);
