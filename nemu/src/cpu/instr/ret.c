#include "cpu/instr.h"

//Undo__cs???
static void pop()
{
    OPERAND esp;
    esp.type = OPR_MEM;
    esp.addr = cpu.esp;
    esp.data_size = data_size;
    esp.sreg = SREG_CS;
    operand_read(&esp);

    // OPERAND test;
    // test.type = OPR_MEM;
    // test.addr = cpu.esp + data_size / 8;
    // test.data_size = data_size;
    // operand_read(&test);
    // printf("mem addr:%x and value= %x\n", test.addr, test.val);

    cpu.eip = esp.val & mask_code; //top to eip

    cpu.esp += data_size / 8;
}
make_instr_func(ret_near)
{
    int len = 1;
    pop();
    print_asm_0("ret", "", len);
    return 0;
}

make_instr_func(ret_near_imm16)
{
    int len = 1;
    OPERAND imm;
    imm.type = OPR_IMM;
    imm.data_size = 16;
    imm.addr = eip + 1;
    operand_read(&imm);
    pop();
    cpu.esp += imm.val;
    print_asm_0("ret", "", len);
    return 0;
}
