#include "mem.h"
#include "error.h"

#include <stdlib.h>
#include <stdio.h>

void mem_read(c_byte* address, size_t size, void* output)
{
	printf("Reading bytes from %x\n", address);

	int offset = size;
	while(offset --> 0)
	{
		printf("Wrote byte at %x\n", address + offset);
	}
}

void mem_write(c_byte* address, size_t size, c_byte* input)
{
}

c_byte* memory;

int mem_init(size_t size)
{
	memory = malloc(size);

	if(memory == NULL)
		return ERR_MEM_HOST_OOM;

	return ERR_OK;
}

void mem_clean()
{
	free(memory);
}

c_byte* mem_resolve(c_addr address)
{
	return memory + address;
}

c_long mem_read_long(c_byte* address)
{
	c_long data;

	mem_read(address, sizeof(c_long), &data);
}

c_short mem_read_short(c_byte* address)
{
}

c_byte mem_read_byte(c_byte* address)
{
	return *address;
}

void mem_write_long(c_byte* address, c_long data)
{
}

void mem_write_short(c_byte* address, c_short data)
{
}

void mem_write_byte(c_byte* address, c_byte data)
{
	*address = data;
}
