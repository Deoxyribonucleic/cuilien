#include "error.h"

#include <stdio.h>
#include <stdlib.h>

int c_error_last = C_ERR_NONE;

char* c_error_tostring(int code)
{
	switch(code)
	{
	case C_ERR_NONE:
		return "No error";
	case C_ERR_VM_FNF:
		return "VM: file not found";
	case C_ERR_MEM_HOST_OOM:
		return "Memory: host machine out of memory";
	case C_ERR_MEM_LIMIT_REACHED:
		return "Memory: the memory limit has been reached";
	case C_ERR_MEM_PF_FAILED:
		return "Memory: pagefault failed due to an unknown reason";
	case C_ERR_MEM_PF_INVALID:
		return "Memory: segmentation fault: invalid address";
	case C_ERR_MEM_NOREAD:
		return "Memory: segmentation fault: access violation (read)";
	case C_ERR_MEM_NOWRITE:
		return "Memory: segmentation fault: access violation (write)";
	case C_ERR_MEM_NOEXEC:
		return "Memory: segmentation fault: access violation (execute)";
	case C_ERR_CPU_HOST_OOM:
		return "CPU: host machine out of memory";
	case C_ERR_CPU_INVALID_MEMORY:
		return "CPU: invalid memory pointer";
	default:
		return "Unknown error";
	}
}

void c_error_print(int code)
{
	printf("Error (%d): %s\n", code, c_error_tostring(code));
}

inline void c_error_clear()
{
	c_error_last = C_ERR_NONE;
}

inline void c_fatal(const char* message)
{
	fprintf(stderr, "fatal error: %s\n", message);
	abort();
}

