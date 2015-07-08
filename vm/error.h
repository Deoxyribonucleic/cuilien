#ifndef _ERROR_H_
#define _ERROR_H_

#define C_ERR_NONE                 0
#define C_ERR_VM_FNF               1

#define C_ERR_MEM_HOST_OOM         50
#define C_ERR_MEM_LIMIT_REACHED    51
#define C_ERR_MEM_PF_FAILED        52
#define C_ERR_MEM_PF_INVALID       53
#define C_ERR_MEM_NOREAD           54
#define C_ERR_MEM_NOWRITE          55
#define C_ERR_MEM_NOEXEC           56

#define C_ERR_CPU_HOST_OOM         100
#define C_ERR_CPU_INVALID_MEMORY   101

#define C_ERR_OK                   C_ERR_NONE

char* c_error_tostring(int code);
void c_error_print(int code);
void c_error_clear();

void c_fatal(const char* message);

extern int c_error_last;

#endif
