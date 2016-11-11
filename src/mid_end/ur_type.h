#pragma once

#include "../ast/ident_node.h"

typedef enum ur_type_mod
{
    UR_TYPE_MOD_ID = 0,		// ident, eg. "i8", "float"…
    UR_TYPE_MOD_PTR = 1,
    UR_TYPE_MOD_CONST = 2,
    UR_TYPE_MOD_size
} ur_type_mod_t;

// Unresolved type
typedef struct ur_type
{
    ur_type_mod_t mod;

    // Either
    ident_node_t* id;
    struct ur_type* sub;
} ur_type_t;
ur_type_t* ur_type_new_ng(ur_type_mod_t mod, ident_node_t* id, ur_type_t* sub);
void ur_type_del(ur_type_t* node);
void ur_type_print(ur_type_t* node);
