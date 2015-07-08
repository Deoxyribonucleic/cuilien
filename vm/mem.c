#include "mem.h"
#include "error.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error Cuilien currently requires that the host machine is little endian. This machine is not, and thus it cannot run Cuilien.
#endif

c_memory_t* c_mem_init(size_t size)
{
	c_error_clear();

	c_memory_t* memory = malloc(sizeof(c_memory_t));
	memory->page_table = c_page_create_table();
	memory->max_size = C_PAGE_ALIGN(size);
	memory->used = 0;

	if(size != memory->max_size)
	{
		printf("[memory] warning: effective memory limit: %lu kB (%lu B is not page aligned)\n", memory->max_size / 1024, size);
	}

	return memory;
}

void c_mem_free(c_memory_t* memory)
{
	int i, pages_freed = 0;
	for(i = 0; i<memory->page_table.size; ++i)
	{
		c_page_t* page = c_vector_resolve(&memory->page_table, i);
		if(page->info.owned)
		{
			free(page->mem);
			page->mem = 0;

			++pages_freed;
		}
	}
	printf("[memory] %d/%lu virtual memory pages freed\n", pages_freed, memory->page_table.size);
	c_vector_free(&memory->page_table);
	free(memory);
}

void c_mem_pagefault(c_memory_t* memory, c_addr address)
{
	c_error_clear();

	printf("[memory] pagefault: paging 0x%08x\n", C_PAGE_ALIGN(address));

	if(memory->used + C_PAGE_SIZE > memory->max_size)
	{
		c_error_last = C_ERR_MEM_LIMIT_REACHED;
		return;
	}

	c_page_info_t new_page_info;
	new_page_info.owned = true;
	new_page_info.perms = C_PAGE_READ | C_PAGE_WRITE | C_PAGE_EXEC;

	c_byte* mem = malloc(sizeof(c_byte) * C_PAGE_SIZE);
	if(mem == NULL)
	{
		c_error_last = C_ERR_MEM_HOST_OOM;
		return;
	}

	c_page_map(&memory->page_table, address, &new_page_info, mem);
	memory->used += C_PAGE_SIZE;
}

c_byte* c_mem_resolve_address(c_memory_t* memory, c_addr address, c_byte required_perms)
{
	c_error_clear();

	if(address == C_NULL)
	{
		c_error_last = C_ERR_MEM_PF_INVALID;
		return NULL;
	}

	c_byte* physical_addr = c_page_resolve(&memory->page_table, address, required_perms);

	if(physical_addr == NULL) // Page fault
	{
		c_mem_pagefault(memory, address);
		if(c_error_last)
			return NULL;

		physical_addr = c_page_resolve(&memory->page_table, address, required_perms);
		if(physical_addr == NULL)
		{
			c_error_last = C_ERR_MEM_PF_FAILED;
			return NULL;
		}
	}

	return physical_addr;
}

void c_mem_read_value(c_memory_t* memory, c_addr address, c_byte* out, size_t length, bool exec)
{
	c_error_clear();
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	int offset;
	for(offset = 0; offset < length; ++offset)
	{
		c_byte* phys = c_mem_resolve_address(memory, address + offset, (exec ? C_PAGE_EXEC : C_PAGE_READ));
		if(c_error_last)
		{
			printf("[memory] error: %s\n", c_error_tostring(c_error_last));
			return;
		}

		out[offset] = *phys;
	}
#else
#error Big endian host machines are not currently supported.
#endif
}

void c_mem_write_value(c_memory_t* memory, c_addr address, c_byte const* value, size_t length)
{
	c_error_clear();
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	int offset;
	for(offset = 0; offset < length; ++offset)
	{
		c_byte* phys = c_mem_resolve_address(memory, address + offset, C_PAGE_WRITE);
		if(c_error_last)
		{
			printf("[memory] error: %s\n", c_error_tostring(c_error_last));
			return;
		}

		*phys = value[offset];
	}
#else
#error Big endian host machines are not currently supported.
#endif
}

c_long c_mem_read_long(c_memory_t* memory, c_addr address, bool exec)
{
	c_long data = 0;
	c_mem_read_value(memory, address, (c_byte*)&data, sizeof(c_long), exec);
	return data;
}

c_short c_mem_read_short(c_memory_t* memory, c_addr address, bool exec)
{
	c_short data = 0;
	c_mem_read_value(memory, address, (c_byte*)&data, sizeof(c_short), exec);
	return data;
}

c_byte c_mem_read_byte(c_memory_t* memory, c_addr address, bool exec)
{
	c_byte data = 0;
	c_mem_read_value(memory, address, (c_byte*)&data, sizeof(c_byte), exec);
	return data;
}

void c_mem_write_long(c_memory_t* memory, c_addr address, c_long data)
{
	c_mem_write_value(memory, address, (c_byte*)&data, sizeof(c_long));
}

void c_mem_write_short(c_memory_t* memory, c_addr address, c_short data)
{
	c_mem_write_value(memory, address, (c_byte*)&data, sizeof(c_short));
}

void c_mem_write_byte(c_memory_t* memory, c_addr address, c_byte data)
{
	c_mem_write_value(memory, address, (c_byte*)&data, sizeof(c_byte));
}

size_t c_mem_load_file(c_mem_handle memory, char const* filename, c_addr start)
{
	int file = open(filename, O_RDONLY);

	if(file < 0)
	{
		printf("File not found.\n");
		return 0;
	}

	c_byte buffer[256];
	ssize_t chunk_length;
	size_t bytes_written = 0;
	while((chunk_length = read(file, buffer, 256)) > 0)
	{
		int i;
		for(i=0; i<chunk_length; ++i)
		{
			c_mem_write_byte(memory, start + bytes_written, buffer[i]);
			++bytes_written;
		}
	}

	close(file);
	return bytes_written;
}

