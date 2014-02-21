#include "page.h"
#include "error.h"

#include <stddef.h>

vector_t page_create_table()
{
	vector_t table;
	vector_create(sizeof(page_t), 0, &table);
	return table;
}

void validate_perms(c_byte perms, c_byte required)
{
	error_clear();
	switch(required)
	{
	case PAGE_EXEC:
		if(!(perms & PAGE_EXEC))
		{
			error_last = ERR_MEM_NOEXEC;
		}
		// No return as we also need to make sure we have read permission
	case PAGE_READ:
		if(!(perms & PAGE_READ))
		{
			error_last = ERR_MEM_NOREAD;
		}
		return;
	case PAGE_WRITE:
		if(!(perms & PAGE_WRITE))
		{
			error_last = ERR_MEM_NOWRITE;
		}
		return;
	default:
		return;
	}
}

c_byte* page_resolve(vector_t* table, c_addr virtual_address, c_byte required_perms)
{
	page_id_t page_id = page_get_id(virtual_address);

	page_t* page = page_find(table, page_id);

	if(page == NULL)
	{
		return NULL;
	}
	
	validate_perms(page->info.perms, required_perms);

	return page->mem + virtual_address % C_PAGE_SIZE;
}

void page_map(vector_t* table, c_addr virtual_address, page_info_t const* info, c_byte* physical_address)
{
	page_t new_page;
	new_page.id = page_get_id(virtual_address);
	new_page.info = *info;
	new_page.mem = physical_address;

	vector_push_back(table, &new_page);
}

page_t* page_find(vector_t* table, page_id_t id)
{
	int i;
	for(i=0; i<table->size; ++i)
	{
		page_t* page = vector_resolve(table, i);
		
		if(page->id == id)
		{
			return page;
		}
	}

	return NULL;
}

page_id_t page_get_id(c_addr virtual_address)
{
	return virtual_address / C_PAGE_SIZE;
}
