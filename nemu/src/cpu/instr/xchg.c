#include "cpu/instr.h"


make_instr_func(exchagne_rm_r_b)
{
    OPERAND rm, r;
    rm.data_size = r.data_size = 8;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    operand_read(&r);
    operand_read(&rm);
    uint32_t val = r.val;
    r.val = rm.val;
    rm.val = val;
    operand_write(&r);
    operand_write(&rm);
    return len;
}
make_instr_func(exchagne_rm_r_v)
{
    OPERAND rm, r;
    rm.data_size = r.data_size = data_size;
    int len = 1;
    len += modrm_r_rm(eip + 1, &r, &rm);
    operand_read(&r);
    operand_read(&rm);
    uint32_t val = r.val;
    r.val = rm.val;
    rm.val = val;
    operand_write(&r);
    operand_write(&rm);

    return len;
}

make_instr_func(exchange_r_v)
{
    int len = 1;
    uint32_t tmp = cpu.eax;
    cpu.eax = cpu.gpr[opcode & 0x07].val;
    cpu.gpr[opcode & 0x07].val = tmp;

    return len;
}

// make_instr_func(xchg_rm_r_v)
// {

//     return 0;
// }

// make_instr_func(xchg_rm_r_b)
// {
//     return 0;
// }
