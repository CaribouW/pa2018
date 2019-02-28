#include "cpu/instr.h"
static uint32_t pop()
{
    OPERAND esp;
    esp.type = OPR_MEM;
    esp.data_size = data_size;
    esp.addr = cpu.esp;
    esp.sreg = SREG_CS;
    operand_read(&esp);
    cpu.esp += data_size / 8;
    return esp.val;
}

make_instr_func(iret)
{
    cpu.eip=pop();
    cpu.cs.val=pop();
    cpu.eflags.val=pop();
    print_asm_0("iret","",1);
    return 0;
}