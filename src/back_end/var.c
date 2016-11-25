#include "var.h"
#include "../defines.h"

#include <string.h>
#include <stdio.h>

var_t* var_new(char const* name, LLVMValueRef val)
{
	assert(name);
	assert(val);
	define_ptr(var_t, ret);

	ret->name = strdup(name);
	ret->val = val;

	if (! ret->name)
	{
		free(ret);
		return NULL;
	}

	return ret;
}

void var_del(var_t* node)
{
	assert(node);

	free(node->name);
	free(node);
}

void var_print(var_t* node)
{
	assert(node);

	printf("<var(%s,<llvm_value_ref>)>", node->name);
}
