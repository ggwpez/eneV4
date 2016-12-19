#include "errors.h"
#include <stdarg.h>

static char const* error_strings[] = {	"SUCCESS",
										"INTERNAL",
										"UNDERLYING",
										"UNKNOWN",
										"BAD_ARGSFILE_NOT_FOUND",
										"CON_DOUBLE",
										"CON_FUN_REG_AS_VARCON_VAR_REG_AS_FUNCON_TYPE_NOT_FOUND",
										"SC_TRANSFORM_TYPE",
										"SC_OVERLOAD_FAULT",
										"SC_BAD_TYPE_CAST",
										"SC_VAR_NOT_FOUND",
										"SC_FUN_NOT_FOUND",
										"SC_CALL_ARGC_WRONGSC_MAIN_MISSING",
										"ME_SCOPER",
										"ME_BAD_TYPE_CAST",
										"BE_BAD_FILE" };



static int sc_var_not_found(va_list ap)
{
	char* var = va_arg(ap, char*);


}

int ERR(error_t type, ...)
{
	va_list ap;
	va_start(ap, type);



	va_end(ap);
}
