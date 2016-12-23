#include "errors.h"
#include "defines.h"
#include <stdio.h>

MAKE_ENUM_STRINGS(error, ERROR_ENUM)

typedef void(*err_ptr)(va_list);
err_ptr err_handler[] = { err_success, err_assert, err_underlying, err_unknown, err_bad_args, err_file_not_found, err_parsing_failed, err_con_double, err_con_fun_reg_as_var, err_con_var_reg_as_fun, err_con_type_not_found, err_sc_transform_type, err_sc_overload_fault, err_sc_bad_type_cast, err_sc_var_not_found, err_sc_fun_not_found, err_sc_call_argc_wrong, err_sc_main_missing, err_me_scoper, err_me_bad_type_cast, err_be_bad_file };
static_assert(_countof(err_handler) == ERROR_SIZE, "_countof(err_handler) == ERROR_SIZE");

void err_success(va_list ap)
{

}

void err_assert(va_list ap)
{

}

void err_underlying(va_list ap)
{

}

void err_unknown(va_list ap)
{

}

void err_bad_args(va_list ap)
{

}

void err_file_not_found(va_list ap)
{

}

void err_parsing_failed(va_list ap)
{

}

void err_con_double(va_list ap)
{

}

void err_con_fun_reg_as_var(va_list ap)
{

}

void err_con_var_reg_as_fun(va_list ap)
{

}

void err_con_type_not_found(va_list ap)
{

}

void err_sc_transform_type(va_list ap)
{

}

void err_sc_overload_fault(va_list ap)
{

}

void err_sc_bad_type_cast(va_list ap)
{

}

void err_sc_var_not_found(va_list ap)
{

}

void err_sc_fun_not_found(va_list ap)
{

}

void err_sc_call_argc_wrong(va_list ap)
{

}

void err_sc_main_missing(va_list ap)
{

}

void err_me_scoper(va_list ap)
{

}

void err_me_bad_type_cast(va_list ap)
{

}

void err_be_bad_file(va_list ap)
{

}

//tok* t = va_arg(ap, tok*);

void ERR(error_t type, ...)
{
	va_list ap;
	va_start(ap, type);

	fprintf(stderr, "Error of type: '%s'\n", error_strings[(int)type]);
	err_handler[(int)type](ap);

	va_end(ap);
	PANIC
}
