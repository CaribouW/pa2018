#include "cpu/instr.h"

make_instr_func(inc_r_v)
{
    int len = 1;
    OPERAND r;
    r.addr = opcode & 0x07;
    r.data_size = data_size;
    r.type = OPR_REG;
    operand_read(&r);
    alu_add(1,r.val,data_size);
    r.val++;
    operand_write(&r);
    return len;
}
make_instr_func(inc_rm_v)
{
    int len = 1;
    OPERAND rm;
    rm.data_size = data_size;

    len += modrm_rm(eip + 1, &rm);
    operand_read(&rm);
    alu_add(1, rm.val, data_size);
    rm.val++;
    operand_write(&rm);
    return len;
}