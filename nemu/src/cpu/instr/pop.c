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

make_instr_func(pop_r_v)
{
    int len = 1;
    OPERAND esp, r;

    r.type = OPR_REG;
    r.data_size = data_size;
    r.addr = opcode & 0x7;

    esp.data_size = data_size;
    esp.type = OPR_MEM;
    esp.sreg = SREG_SS;
    esp.addr = cpu.esp;
    load_sreg(SREG_SS);

    operand_read(&esp);
    r.val = esp.val;
    operand_write(&r);
    cpu.esp += data_size / 8;
    print_asm_1("pop", "", len, &r);
    return len;
}

make_instr_func(pop_rm)
{
    int len = 1;
    OPERAND rm, esp;

    rm.data_size = data_size;

    esp.data_size = data_size;
    esp.type = OPR_MEM;
    esp.sreg = SREG_SS;
    esp.addr = cpu.esp;
    load_sreg(SREG_SS);
    len += modrm_rm(eip + 1, &rm);
    printf("len=%d\n", len);
    operand_read(&esp);
    rm.val = esp.val;
    operand_write(&rm);
    cpu.esp += data_size / 8;
    print_asm_1("pop", "", len, &rm);
    return len;
}

make_instr_func(popa)
{
    int len = 1;
    cpu.edi = pop();
    cpu.esi = pop();
    cpu.ebp = pop();
    pop();
    cpu.ebx = pop();
    cpu.edx = pop();
    cpu.ecx = pop();
    cpu.eax = pop();

    print_asm_0("popa", "", len);
    return len;
}