#ifndef _ERROR_H_
#define _ERROR_H_

#define ERR_NONE		0
#define ERR_VM_FNF		1
#define ERR_MEM_HOST_OOM	50

#define ERR_OK			ERR_NONE

char* error_tostring(int code);

#endif
