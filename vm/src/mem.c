#include "mem.h"
#include "error.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error Cuilien requires that the host machine is little endian. This machine is not, and thus it cannot run Cuilien.
#endif

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
	memcpy(&data, address, sizeof(c_long));
	return data;
}

c_short mem_read_short(c_byte* address)
{
	c_short data;
	memcpy(&data, address, sizeof(c_short));
	return data;
}

c_byte mem_read_byte(c_byte* address)
{
	return *address;
}

void mem_write_long(c_byte* address, c_long data)
{
	memcpy(address, &data, sizeof(c_long));
}

void mem_write_short(c_byte* address, c_short data)
{
	memcpy(address, &data, sizeof(c_short));
}

void mem_write_byte(c_byte* address, c_byte data)
{
	*address = data;
}
