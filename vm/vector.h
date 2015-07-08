#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stddef.h>

#define C_VECTOR_CHUNK	4

struct c_vector
{
	size_t size, capacity;
	size_t element_size;
	void* data;
};
typedef struct c_vector c_vector_t;

void c_vector_create(size_t element_size, size_t initial_size, c_vector_t* output);
void c_vector_free(c_vector_t* vector);

void c_vector_resize(c_vector_t* vector, size_t new_size);

void c_vector_push_back(c_vector_t* vector, void* new_element);
void c_vector_pop_back(c_vector_t* vector);

void c_vector_insert(c_vector_t* vector, void* new_element, size_t at_index);
void c_vector_remove(c_vector_t* vector, size_t index);

void c_vector_set(c_vector_t* vector, size_t index, void* element);
void c_vector_get(c_vector_t* vector, size_t index, void* element_out);

void* c_vector_resolve(c_vector_t* vector, size_t index);

#endif
