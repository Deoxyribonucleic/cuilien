#include "vector.h"
#include "error.h"

#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

static inline size_t calc_capacity(size_t size)
{
	return ((size - 1) / C_VECTOR_CHUNK + 1) * C_VECTOR_CHUNK;
}

static inline size_t min(size_t a, size_t b)
{
	return (a < b ? a : b);
}

void c_vector_create(size_t element_size, size_t initial_size, c_vector_t* output)
{
	output->element_size = element_size;
	output->size = initial_size;
	output->capacity = calc_capacity(initial_size);

	output->data = malloc(output->capacity * element_size);
}

void c_vector_free(c_vector_t* vector)
{
	vector->element_size = 0;
	vector->size = 0;
	vector->capacity = 0;

	free(vector->data);
	vector->data = 0;
}

bool c_vector_adjust_capacity(c_vector_t* vector, size_t new_size)
{
	vector->size = new_size;

	size_t new_capacity = calc_capacity(new_size);
	if(new_capacity == vector->capacity)
		return false; // No reallocation necessary

	vector->data = malloc(new_capacity * vector->element_size);
	vector->capacity = new_capacity;

	if(!vector->data)
	{
		c_fatal("failed to allocate new memory for vector");
	}

	return true; // Reallocation necessary
}

void c_vector_resize(c_vector_t* vector, size_t new_size)
{
	void* old_data = vector->data;
	size_t old_capacity = vector->capacity;
	size_t old_size = vector->size;

	if(c_vector_adjust_capacity(vector, new_size) && old_data != 0)
	{
		memcpy(vector->data, old_data, min(old_size * vector->element_size, new_size * vector->element_size));
		free(old_data);
	}
}

void c_vector_push_back(c_vector_t* vector, void* new_element)
{
	c_vector_resize(vector, vector->size + 1);
	c_vector_set(vector, vector->size - 1, new_element);
}

void c_vector_pop_back(c_vector_t* vector)
{
	c_vector_resize(vector, vector->size - 1);
}

void c_vector_insert(c_vector_t* vector, void* new_element, size_t at_index)
{
	assert(at_index <= vector->size);

	void* old_data = vector->data;
	size_t old_size = vector->size;
	
	// If a reallocation was necessary, copy elements preceding destination index
	bool reallocation;
	if(reallocation = c_vector_adjust_capacity(vector, vector->size + 1))
	{
		memcpy(vector->data, old_data, at_index * vector->element_size);
	}

	// Move remaining elements one index to the right
	memmove(c_vector_resolve(vector, at_index + 1), old_data + at_index * vector->element_size, (old_size - at_index) * vector->element_size);

	// Insert new element
	c_vector_set(vector, at_index, new_element);

	if(reallocation)
	{
		free(old_data);
	}
}

void c_vector_remove(c_vector_t* vector, size_t index)
{
	assert(index < vector->size);

	void* old_data = vector->data;
	size_t old_size = vector->size;

	// If a reallocation is necessary, copy elements preceding the index to be removed
	bool reallocation;
	if(reallocation = c_vector_adjust_capacity(vector, vector->size - 1))
	{
		memcpy(vector->data, old_data, index * vector->element_size);
	}

	// Move remaining elements one index to the left
	memmove(c_vector_resolve(vector, index), old_data + (index + 1) * vector->element_size, (old_size - index - 1) * vector->element_size);

	if(reallocation)
	{
		free(old_data);
	}
}

void c_vector_set(c_vector_t* vector, size_t index, void* element)
{
	assert(index < vector->size);
	memcpy(c_vector_resolve(vector, index), element, vector->element_size);
}

void c_vector_get(c_vector_t* vector, size_t index, void* element_out)
{
	assert(index < vector->size);
	memcpy(element_out, c_vector_resolve(vector, index), vector->element_size);
}

void* c_vector_resolve(c_vector_t* vector, size_t index)
{
	return vector->data + index * vector->element_size;
}
