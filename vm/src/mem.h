#ifndef _MEM_H_
#define _MEM_H_

#include <stdint.h>
#include <stddef.h>

#define MEM_MACHINE_ENDIAN LITTLE

typedef uint32_t c_word, c_int, c_long, c_addr;
typedef uint16_t c_short;
typedef uint8_t c_byte;

int mem_init(size_t size);
void mem_clean();
c_byte* mem_resolve(c_addr address);

c_long mem_read_long(c_byte* address);
c_short mem_read_short(c_byte* address);
c_byte mem_read_byte(c_byte* address);

void mem_write_long(c_byte* address, c_long data);
void mem_write_short(c_byte* address, c_short data);
void mem_write_byte(c_byte* address, c_byte data);

#endif
