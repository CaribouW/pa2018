#include "cpu/instr.h"
#include "cpu/intr.h"
// static void push(uint32_t num)
// {
//         OPERAND esp;
//         cpu.esp -= data_size / 8; //dec
//         esp.type = OPR_MEM;
//         esp.data_size = data_size;
//         esp.addr = cpu.esp;
//         esp.sreg = SREG_CS;
//         esp.val = num & mask_code; //push
//         operand_write(&esp);
// }
make_instr_func(int_)
{
    uint8_t imm = instr_fetch(eip + 1, 1);
    raise_sw_intr(imm);
    return 0;
}
