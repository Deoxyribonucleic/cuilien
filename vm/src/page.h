#ifndef _PAGE_H_
#define _PAGE_H_

#include "types.h"

#define C_PAGE_SIZE	256

/*struct page_info
{
	c_byte perms;
};

struct page
{
	page_info info;
	c_byte mem[C_PAGE_SIZE];
};*/

// Description: translates a virtual address to a physical address
// Return value: returns physical memory address
c_byte* page_resolve(c_addr virtual_address);



int page_get_number(c_addr virtual_address);

#endif
