#ifndef _MEM_H_
#define _MEM_H_

#include <stddef.h>
#include <stdint.h>

#include "page.h"
#include "types.h"

typedef struct memory
{
	vector_t page_table;
	size_t max_size, used;
} memory_t;
typedef memory_t* mem_handle;

memory_t* mem_init(size_t size);
void mem_free(memory_t* memory);

c_long mem_read_long(memory_t* memory, c_addr address, bool exec);
c_short mem_read_short(memory_t* memory, c_addr address, bool exec);
c_byte mem_read_byte(memory_t* memory, c_addr address, bool exec);

void mem_write_long(memory_t* memory, c_addr address, c_long data);
void mem_write_short(memory_t* memory, c_addr address, c_short data);
void mem_write_byte(memory_t* memory, c_addr address, c_byte data);

#endif
