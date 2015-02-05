#ifndef _ERROR_H_
#define _ERROR_H_

#define ERR_NONE                 0
#define ERR_VM_FNF               1

#define ERR_MEM_HOST_OOM         50
#define ERR_MEM_LIMIT_REACHED    51
#define ERR_MEM_PF_FAILED        52
#define ERR_MEM_PF_INVALID       53
#define ERR_MEM_NOREAD           54
#define ERR_MEM_NOWRITE          55
#define ERR_MEM_NOEXEC           56

#define ERR_CPU_HOST_OOM         100
#define ERR_CPU_INVALID_MEMORY   101

#define ERR_OK                   ERR_NONE

char* error_tostring(int code);
void error_print(int code);
inline void error_clear();

inline void fatal(const char* message);

extern int error_last;

#endif
