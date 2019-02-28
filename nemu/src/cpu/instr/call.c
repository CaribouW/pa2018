#include "cpu/instr.h"
static void push(uint32_t eip)
{
    OPERAND esp;
    cpu.esp -= data_size / 8; //dec
    esp.type = OPR_MEM;
    esp.data_size = data_size;
    esp.addr = cpu.esp;
    esp.sreg = SREG_CS;
    esp.val = eip & mask_code; //push
    operand_write(&esp);
}
make_instr_func(call_near)
{
    int len = 1;
    OPERAND rel;
    rel.addr = eip + 1;
    rel.type = OPR_IMM;
    rel.data_size = data_size;
    operand_read(&rel);
    len += rel.data_size / 8; //eip begins with the next instr,
    push(eip + len);
    cpu.eip = (cpu.eip + rel.val) & mask_code;
    print_asm_1("call", "", len, &rel);
    return len;
}

make_instr_func(call_near_indirect)
{
    int len = 1;
    OPERAND rm;
    rm.data_size = data_size;
    len += modrm_rm(eip + 1, &rm);
    push(eip + len);
    operand_read(&rm);
    cpu.eip = rm.val & mask_code;
    rm.val = cpu.eip;
    print_asm_1("call", "", len, &rm);
    return 0;
}
