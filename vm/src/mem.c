#include "mem.h"
#include "error.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
#error Cuilien currently requires that the host machine is little endian. This machine is not, and thus it cannot run Cuilien.
#endif

memory_t* mem_init(size_t size)
{
	error_clear();

	memory_t* memory = malloc(sizeof(memory_t));
	memory->page_table = page_create_table();
	memory->max_size = PAGE_ALIGN(size);
	memory->used = 0;

	if(size != memory->max_size)
	{
		printf("[memory] warning: effective memory limit: %d kB (%d B is not page aligned)\n", memory->max_size / 1024, size);
	}

	return memory;
}

void mem_free(memory_t* memory)
{
	int i, pages_freed = 0;
	for(i = 0; i<memory->page_table.size; ++i)
	{
		page_t* page = vector_resolve(&memory->page_table, i);
		if(page->info.owned)
		{
			free(page->mem);
			page->mem = 0;

			++pages_freed;
		}
	}
	printf("[memory] %d/%d virtual memory pages freed\n", pages_freed, memory->page_table.size);
	vector_free(&memory->page_table);
	free(memory);
}

void mem_pagefault(memory_t* memory, c_addr address)
{
	error_clear();

	printf("[memory] pagefault: paging 0x%08x\n", PAGE_ALIGN(address));

	if(memory->used + C_PAGE_SIZE > memory->max_size)
	{
		error_last = ERR_MEM_LIMIT_REACHED;
		return;
	}

	page_info_t new_page_info;
	new_page_info.owned = true;
	new_page_info.perms = PAGE_READ | PAGE_WRITE | PAGE_EXEC;	

	c_byte* mem = malloc(sizeof(c_byte) * C_PAGE_SIZE);
	if(mem == NULL)
	{
		error_last = ERR_MEM_HOST_OOM;
		return;
	}

	page_map(&memory->page_table, address, &new_page_info, mem);
	memory->used += C_PAGE_SIZE;
}

c_byte* mem_resolve_address(memory_t* memory, c_addr address, c_byte required_perms)
{
	error_clear();

	if(address == C_NULL)
	{
		error_last = ERR_MEM_PF_INVALID;
		return NULL;
	}

	c_byte* physical_addr = page_resolve(&memory->page_table, address, required_perms);
	
	if(physical_addr == NULL) // Page fault
	{
		mem_pagefault(memory, address);
		if(error_last)
			return NULL;
		
		physical_addr = page_resolve(&memory->page_table, address, required_perms);
		if(physical_addr == NULL)
		{
			error_last = ERR_MEM_PF_FAILED;
			return NULL;
		}
	}

	return physical_addr;
}

void mem_read_value(memory_t* memory, c_addr address, c_byte* out, size_t length, bool exec)
{
	error_clear();
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	int offset;
	for(offset = 0; offset < length; ++offset)
	{
		c_byte* phys = mem_resolve_address(memory, address + offset, (exec ? PAGE_EXEC : PAGE_READ));
		if(error_last)
		{
			printf("[memory] error: %s\n", error_tostring(error_last));
			return;
		}

		out[offset] = *phys;
	}
#else
#error Big endian host machines are not currently supported.
#endif
}

void mem_write_value(memory_t* memory, c_addr address, c_byte* value, size_t length)
{
	error_clear();
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	int offset;
	for(offset = 0; offset < length; ++offset)
	{
		c_byte* phys = mem_resolve_address(memory, address + offset, PAGE_WRITE);
		if(error_last)
		{
			printf("[memory] error: %s\n", error_tostring(error_last));
			return;
		}

		*phys = value[offset];
	}
#else
#error Big endian host machines are not currently supported.
#endif
}

c_long mem_read_long(memory_t* memory, c_addr address, bool exec)
{
	c_long data = 0;
	mem_read_value(memory, address, (c_byte*)&data, sizeof(c_long), exec);
	return data;
}

c_short mem_read_short(memory_t* memory, c_addr address, bool exec)
{
	c_short data = 0;
	mem_read_value(memory, address, (c_byte*)&data, sizeof(c_short), exec);
	return data;
}

c_byte mem_read_byte(memory_t* memory, c_addr address, bool exec)
{
	c_byte data = 0;
	mem_read_value(memory, address, (c_byte*)&data, sizeof(c_byte), exec);
	return data;
}

void mem_write_long(memory_t* memory, c_addr address, c_long data)
{
	mem_write_value(memory, address, (c_byte*)&data, sizeof(c_long));
}

void mem_write_short(memory_t* memory, c_addr address, c_short data)
{
	mem_write_value(memory, address, (c_byte*)&data, sizeof(c_short));
}

void mem_write_byte(memory_t* memory, c_addr address, c_byte data)
{
	mem_write_value(memory, address, (c_byte*)&data, sizeof(c_byte));
}
