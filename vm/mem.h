#ifndef _MEM_H_
#define _MEM_H_

#include <stddef.h>
#include <stdint.h>

#include "page.h"
#include "types.h"

typedef struct
{
	c_vector_t page_table;
	size_t max_size, used;
} c_memory_t;
typedef c_memory_t* c_mem_handle;

c_memory_t* c_mem_init(size_t size);
void c_mem_free(c_memory_t* from);

c_memory_t* c_mem_copy(c_memory_t* memory);

c_long c_mem_read_long(c_memory_t* memory, c_addr address, bool exec);
c_short c_mem_read_short(c_memory_t* memory, c_addr address, bool exec);
c_byte c_mem_read_byte(c_memory_t* memory, c_addr address, bool exec);

void c_mem_write_long(c_memory_t* memory, c_addr address, c_long data);
void c_mem_write_short(c_memory_t* memory, c_addr address, c_short data);
void c_mem_write_byte(c_memory_t* memory, c_addr address, c_byte data);

void c_mem_read_value(c_memory_t* memory, c_addr address, c_byte* out, size_t length, bool exec);
void c_mem_write_value(c_memory_t* memory, c_addr address, c_byte const* value, size_t length);

size_t c_mem_load_file(c_mem_handle memory, char const* filename, c_addr start);

size_t c_mem_load(c_mem_handle memory, char const* from, size_t length, c_addr start);
size_t c_mem_dump(c_mem_handle memory, char* to, size_t length, c_addr start);
size_t c_mem_dump_to_file(c_mem_handle memory, char const* filename);

#endif
