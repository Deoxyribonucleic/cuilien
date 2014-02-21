#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stddef.h>

#define VECTOR_CHUNK	4

struct vector
{
	size_t size, capacity;
	size_t element_size;
	void* data;
};
typedef struct vector vector_t;

void vector_create(size_t element_size, size_t initial_size, vector_t* output);
void vector_free(vector_t* vector);

void vector_resize(vector_t* vector, size_t new_size);

void vector_push_back(vector_t* vector, void* new_element);
void vector_pop_back(vector_t* vector);

void vector_insert(vector_t* vector, void* new_element, size_t at_index);
void vector_remove(vector_t* vector, size_t index);

void vector_set(vector_t* vector, size_t index, void* element);
void vector_get(vector_t* vector, size_t index, void* element_out);

void* vector_resolve(vector_t* vector, size_t index);

#endif
