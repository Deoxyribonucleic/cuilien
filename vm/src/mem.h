#ifndef _MEM_H_
#define _MEM_H_

#include <stdint.h>
#include <stddef.h>

#define MEM_MACHINE_ENDIAN LITTLE

typedef uint32_t c_word, c_int, c_long, c_addr;
typedef uint16_t c_short;
typedef uint8_t c_byte;
typedef c_byte* mem_handle;

mem_handle mem_init(size_t size);
void mem_free(mem_handle memory);
c_byte* mem_resolve(mem_handle memory, c_addr address);
c_addr mem_resolve_reverse(mem_handle memory, c_byte* address);

c_long mem_read_long(c_byte* address);
c_short mem_read_short(c_byte* address);
c_byte mem_read_byte(c_byte* address);

void mem_write_long(c_byte* address, c_long data);
void mem_write_short(c_byte* address, c_short data);
void mem_write_byte(c_byte* address, c_byte data);

void mem_read(c_byte* address, void* dest, size_t size);
void mem_write(c_byte* address, void* src, size_t size);
size_t mem_offset_for_endian(size_t size, size_t offset);

#endif
