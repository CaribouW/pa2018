#include "cpu/instr.h"
static void pop()
{
    OPERAND esp;
    esp.type = OPR_MEM;
    esp.addr = cpu.esp;
    // esp.sreg = SREG_SS;
    esp.data_size = data_size;

    operand_read(&esp);

    cpu.ebp = esp.val & mask_code; //top to eip

    cpu.esp += data_size / 8;
}
make_instr_func(leave)
{
    int len = 1;
    cpu.esp = cpu.ebp;  //ebp <- esp
    pop();
    return len;
}