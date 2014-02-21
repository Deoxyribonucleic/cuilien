#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>

typedef uint32_t c_word, c_int, c_long, c_addr;
typedef uint16_t c_short;
typedef uint8_t c_byte;

#define C_NULL		0
#define C_ADDR_SIZE	sizeof(c_addr)

#endif
