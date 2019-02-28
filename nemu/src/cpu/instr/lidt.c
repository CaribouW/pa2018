#include "cpu/instr.h"

make_instr_func(lidt)
{
    int len = 1;
    OPERAND mem;
    mem.data_size = data_size;
    mem.type = OPR_MEM;

    len += modrm_rm(eip + 1, &mem);

    OPERAND limit, base;
    limit.data_size = 16;
    limit.type = OPR_MEM;
    limit.addr = mem.addr;
    base.data_size = (data_size == 32) ? 32 : 24;
    base.type = OPR_MEM;
    base.addr = mem.addr + limit.data_size / 8;

    base.val = laddr_read(base.addr, base.data_size / 8);
    limit.val = laddr_read(limit.addr, limit.data_size / 8);

    cpu.idtr.base = base.val;
    cpu.idtr.limit = limit.val;

    print_asm_0("lgdt", "", len + 5);

    return len;
}