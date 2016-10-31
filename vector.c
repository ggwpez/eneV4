#include "ast.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

ast_vector_t* ast_vector_new(size_t capacity)
{
	malloc_ptr(ast_vector_t, ret);

	memset(ret, 0, sizeof(ast_vector_t));
	ast_vector_resize(ret, capacity);

	return ret;
}

void ast_vector_del(ast_vector_t* v)
{
	assert(v);

	for (size_t i = 0; i < ast_vector_size(v); ++i)      // TODO optimize
	{
		ast_ptr e = ast_vector_at(v, i);
		ast_del(e);
	}

	free(v);
}

ast_vector_t* ast_vector_resize(ast_vector_t* v, size_t new_capacity)
{
	assert(v);

	if (new_capacity)
	{
		if (!v->data)		// init?
		{
			v->data = malloc(new_capacity *sizeof(ast_vector_t));
			if (!v->data)
				return NULL;
			v->capacity = new_capacity;
			v->ptr = v->data;
		}
		else
		{
			void* tmp = realloc(v->data, new_capacity *sizeof(ast_vector_t));
			if (!tmp || tmp != v->data)
				return NULL;		// TODO do i have to free failed realloc?
			v->capacity = new_capacity;
		}
	}
	else if (v->data) // delete all?
	{
		free(v->data);
		v->ptr = v->data = NULL;
	}

	return v;
}

size_t ast_vector_size(ast_vector_t* v)
{
	assert(v);
	return !v->data ? 0 : (v->ptr -v->data);
}

ast_ptr ast_vector_at(ast_vector_t* v, size_t i)
{
	assert(v);
	assert(v->data);

	if (i >= ast_vector_size(v))
		return NULL;

	return v->data[i];
}

ast_ptr ast_vector_rat(ast_vector_t* v, size_t i)
{
	assert(v);
	assert(v->data);
	size_t l = ast_vector_size(v);

	if (!l || i >= l)
		return NULL;

	return v->data[l -i];
}

ast_ptr ast_vector_push_back(ast_vector_t* v, ast_ptr e)
{
	assert(v);
	assert(e);

	size_t l = ast_vector_size(v);
	if (l >= v->capacity)
	{
		if (!(v = ast_vector_resize(v, !v->capacity ? 5 : v->capacity *2)))
			return NULL;
	}

	return (*v->ptr++ = e);
}

ast_ptr ast_vector_pop_back(ast_vector_t* v)
{
	assert(v);
	assert(v->data);

	if (!ast_vector_size(v))		// <=> (v->ptr == v->data)
		return NULL;

	return (*--v->ptr);				// TODO check contract
}

void ast_vector_print(ast_vector_t* v)
{
	size_t l = ast_vector_size(v);

	printf("v->data=%p, v->ptr=%p, v->capacity=%zu, v->size=%zu\n", (void*)v->data, (void*)v->ptr, v->capacity, ast_vector_size(v));
	printf("<ast_vector(");
	for (size_t i = 0; i < l; ++i)
	{
		ast_ptr e = ast_vector_at(v, i);
		ast_print(e), printf(i +1 == l ? "" : ",");
	}

	printf(")>");
}
