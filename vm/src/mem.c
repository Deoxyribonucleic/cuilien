#include "mem.h"
#include "error.h"

#include <stdlib.h>
#include <stdio.h>

inline size_t mem_offset_for_endian(size_t size, size_t offset)
{
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	// Little endian, so leave offset unchanged (since the memory we're writing is little endian, as well)
	return offset;
#else
	// Big endian, so reverse offse
	return size - offset - 1;
#endif
}

void mem_read(c_byte* address, void* dest, size_t size)
{
	int offset = size;
	while(offset --> 0)
	{
		*((c_byte*)dest + mem_offset_for_endian(size, offset)) = *(address + offset);
	}
}

void mem_write(c_byte* address, void* src, size_t size)
{
	int offset = size;
	while(offset --> 0)
	{
		*(address + offset) = *((c_byte*)src + mem_offset_for_endian(size, offset));
	}
}

mem_handle mem_init(size_t size)
{
	error_clear();
	mem_handle memory = malloc(size);

	if(memory == NULL)
	{
		error_last = ERR_MEM_HOST_OOM;
	}

	return memory;
}

void mem_free(mem_handle memory)
{
	if(memory)
		free(memory);
}

c_byte* mem_resolve(mem_handle memory, c_addr address)
{
	return memory + address;
}

c_addr mem_resolve_reverse(mem_handle memory, c_byte* address)
{
	return address - memory;
}

c_long mem_read_long(c_byte* address)
{
	c_long data;
	mem_read(address, &data, sizeof(c_long));
	return data;
}

c_short mem_read_short(c_byte* address)
{
	c_short data;
	mem_read(address, &data, sizeof(c_short));
	return data;
}

c_byte mem_read_byte(c_byte* address)
{
	return *address;
}

void mem_write_long(c_byte* address, c_long data)
{
	mem_write(address, &data, sizeof(c_long));
}

void mem_write_short(c_byte* address, c_short data)
{
	mem_write(address, &data, sizeof(c_short));
}

void mem_write_byte(c_byte* address, c_byte data)
{
	*address = data;
}
