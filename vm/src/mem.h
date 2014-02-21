#ifndef _MEM_H_
#define _MEM_H_

#include <stddef.h>
#include <stdint.h>

#include "page.h"
#include "types.h"

/*struct memory
{
	page* page_table[2^];
	size_t max_size;

	c_byte* data;
};
*/
typedef c_byte* mem_handle;

mem_handle mem_init(size_t size);
void mem_free(mem_handle memory);
c_byte* mem_resolve(mem_handle memory, c_addr address);
c_addr mem_resolve_reverse(mem_handle memory, c_byte* address);

c_long mem_read_long(c_byte* address);
c_short mem_read_short(c_byte* address);
c_byte mem_read_byte(c_byte* address);

void mem_write_long(c_byte* address, c_long data);
void mem_write_short(c_byte* address, c_short data);
void mem_write_byte(c_byte* address, c_byte data);

#endif
