#ifndef _PAGE_H_
#define _PAGE_H_

#include "vector.h"
#include "types.h"

#include <stdbool.h>
#include <stdint.h>

#define C_PAGE_SIZE  1024

#define PAGE_READ    1
#define PAGE_WRITE   2
#define PAGE_EXEC    4

#define PAGE_ALIGN(x)	(x - (x % C_PAGE_SIZE))

typedef uint32_t page_id_t;

typedef struct page_info
{
	c_byte perms;
	bool owned;
} page_info_t;

typedef struct page
{
	page_id_t id;
	page_info_t info;
	c_byte* mem;
} page_t;

vector_t page_create_table();
c_byte* page_resolve(vector_t* table, c_addr virtual_address, c_byte required_perms);
void page_map(vector_t* table, c_addr virtual_address, page_info_t const* info, c_byte* physical_address);
page_t* page_find(vector_t* table, page_id_t id);

page_id_t page_get_id(c_addr virtual_address);

#endif
