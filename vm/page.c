#include "page.h"
#include "error.h"

#include <stddef.h>

c_vector_t c_page_create_table()
{
	c_vector_t table;
	c_vector_create(sizeof(c_page_t), 0, &table);
	return table;
}

void validate_perms(c_byte perms, c_byte required)
{
	c_error_clear();
	switch(required)
	{
	case C_PAGE_EXEC:
		if(!(perms & C_PAGE_EXEC))
		{
			c_error_last = C_ERR_MEM_NOEXEC;
		}
		// No return as we also need to make sure we have read permission
	case C_PAGE_READ:
		if(!(perms & C_PAGE_READ))
		{
			c_error_last = C_ERR_MEM_NOREAD;
		}
		return;
	case C_PAGE_WRITE:
		if(!(perms & C_PAGE_WRITE))
		{
			c_error_last = C_ERR_MEM_NOWRITE;
		}
		return;
	default:
		return;
	}
}

c_byte* c_page_resolve(c_vector_t* table, c_addr virtual_address, c_byte required_perms)
{
	c_page_id_t page_id = c_page_get_id(virtual_address);

	c_page_t* page = c_page_find(table, page_id);

	if(page == NULL)
	{
		return NULL;
	}
	
	validate_perms(page->info.perms, required_perms);

	return page->mem + virtual_address % C_PAGE_SIZE;
}

void c_page_map(c_vector_t* table, c_addr virtual_address, c_page_info_t const* info, c_byte* physical_address)
{
	c_page_t new_page;
	new_page.id = c_page_get_id(virtual_address);
	new_page.info = *info;
	new_page.mem = physical_address;

	c_vector_push_back(table, &new_page);
}

c_page_t* c_page_find(c_vector_t* table, c_page_id_t id)
{
	int i;
	for(i=0; i<table->size; ++i)
	{
		c_page_t* page = c_vector_resolve(table, i);
		
		if(page->id == id)
		{
			return page;
		}
	}

	return NULL;
}

c_page_id_t c_page_get_id(c_addr virtual_address)
{
	return virtual_address / C_PAGE_SIZE;
}
