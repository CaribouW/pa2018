#include "cpu/instr.h"
make_instr_func(lea)
{
    OPERAND r, m;
    int len = 1;
    r.data_size = data_size;
    m.data_size = data_size;
    r.type = OPR_REG;
    m.type = OPR_MEM;

    len += modrm_r_rm(eip + 1, &r, &m);
    r.val = m.addr & mask_code;
    // // operand_read(&m);
    // switch (data_size)
    // {
    // // assert(data_size == 16 || data_size == 32);
    // case 16:
    //     r.val = m.addr & 0xFFFF;
    //     break;
    // case 32:
    //     r.val = m.addr & 0xFFFFFFFF;
    //     break;
    // default:
    //     r.val = m.addr;
    //     break;
    // }
    print_asm_2("lea", "", len, &m, &r);
    operand_write(&r);

    return len;
}
