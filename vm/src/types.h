#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdint.h>

typedef uint32_t c_word, c_int, c_long, c_addr, c_reg;
typedef int32_t c_sword, c_sint, c_slong;
typedef uint16_t c_short;
typedef int16_t c_sshort;
typedef uint8_t c_byte;
typedef int8_t c_sbyte;

#define C_NULL		0
#define C_ADDR_SIZE	sizeof(c_addr)

#endif
