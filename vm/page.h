#ifndef _PAGE_H_
#define _PAGE_H_

#include "vector.h"
#include "types.h"

#include <stdbool.h>
#include <stdint.h>

#define C_PAGE_SIZE  1024

#define C_PAGE_READ    1
#define C_PAGE_WRITE   2
#define C_PAGE_EXEC    4

#define C_PAGE_ALIGN(x)	(x - (x % C_PAGE_SIZE))

typedef uint32_t c_page_id_t;

typedef struct c_page_info
{
	c_byte perms;
	bool owned;
} c_page_info_t;

typedef struct c_page
{
	c_page_id_t id;
	c_page_info_t info;
	c_byte* mem;
} c_page_t;

c_vector_t c_page_create_table();
c_byte* c_page_resolve(c_vector_t* table, c_addr virtual_address, c_byte required_perms);
void c_page_map(c_vector_t* table, c_addr virtual_address, c_page_info_t const* info, c_byte* physical_address);
c_page_t* c_page_find(c_vector_t* table, c_page_id_t id);

c_page_id_t c_page_get_id(c_addr virtual_address);

#endif
