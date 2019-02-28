#include "nemu.h"
#include "cpu/reg.h"
#include "memory/memory.h"
#include <stdlib.h>
uint32_t look_up_fun_symtab(char *sym, bool *success);
uint32_t look_up_symtab(char *sym, bool *success);
const char* find_fun_name(uint32_t eip);