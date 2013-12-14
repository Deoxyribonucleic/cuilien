#ifndef _VM_H_
#define _VM_H_

#include "args.h"

int vm_init(struct args_info* args);
void vm_run();
void vm_cleanup();

#endif
