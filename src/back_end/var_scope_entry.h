#include "var.h"
#include "../vector.h"
#include "../defines.h"

typedef struct
{
	var_vec_t* vars;
} var_scope_entry_t;

var_scope_entry_t* var_scope_entry_new();
void var_scope_entry_add(var_scope_entry_t* sc, var_t* var);
var_t* var_scope_entry_get(var_scope_entry_t* sc, char const* name);
void var_scope_entry_del(var_scope_entry_t* sc);
void var_scope_entry_print(var_scope_entry_t* sc);

typedef var_scope_entry_t* var_scope_entry_ptr;
MAKE_VECTOR_H(var_scope_entry)
