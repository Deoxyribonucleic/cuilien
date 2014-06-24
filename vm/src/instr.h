#ifndef _INSTR_H_
#define _INSTR_H_


#define INSTR_NOP	0x0000
#define INSTR_HALT	0x0001
#define INSTR_INT	0x0002
#define INSTR_MOV	0x0003

#define INSTR_INC	0x0005
#define INSTR_DEC	0x0006

#define INSTR_SHOW	0x0010


#define INSTRUCTION_VECTOR_LENGTH	100

typedef struct cpu cpu_t;
typedef struct operand operand_t;
typedef void (*instruction_function)(cpu_t*, operand_t const* , operand_t const*);

extern instruction_function instruction_vector[INSTRUCTION_VECTOR_LENGTH];
void build_instruction_vector();

#endif
