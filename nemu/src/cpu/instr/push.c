#include "cpu/instr.h"
static void push(uint32_t num)
{
        OPERAND esp;
        cpu.esp -= data_size / 8; //dec
        esp.type = OPR_MEM;
        esp.data_size = data_size;
        esp.addr = cpu.esp;
        esp.sreg = SREG_CS;
        esp.val = num & mask_code; //push
        operand_write(&esp);
}

make_instr_func(push_i_b)
{
        int len = 1;
        OPERAND imm, esp;
        imm.addr = eip + 1;
        imm.data_size = 8;
        imm.type = OPR_IMM;
        // imm.sreg = SREG_SS;

        cpu.esp -= data_size / 8; //dec the sp before pushing
        esp.data_size = data_size;
        esp.type = OPR_MEM;
        esp.sreg = SREG_SS;
        esp.addr = cpu.esp; //esp
        load_sreg(SREG_SS);
        operand_read(&imm);
        esp.val = imm.val;
        operand_write(&esp);
        print_asm_2("push", "", len, &esp, &imm);
        return len + imm.data_size / 8;
}

make_instr_func(push_i_v)
{
        int len = 1;
        OPERAND imm, esp;
        imm.addr = eip + 1;
        imm.data_size = data_size;
        imm.type = OPR_IMM;

        cpu.esp -= data_size / 8; //dec the sp before pushing
        esp.data_size = data_size;
        esp.type = OPR_MEM;
        esp.sreg = SREG_SS;
        esp.addr = cpu.esp; //sp
        load_sreg(SREG_SS);
        load_sreg(SREG_CS);
        operand_read(&imm);
        esp.val = imm.val & mask_code;
        operand_write(&esp);
        print_asm_2("push", "", len, &esp, &imm);

        return len + imm.data_size / 8;
}

make_instr_func(push_rm_v)
{
        OPERAND rm, des;
        int len = 1;
        cpu.esp -= data_size / 8;
        des.data_size = data_size;
        des.type = OPR_MEM;
        des.sreg = SREG_SS;
        des.addr = cpu.esp;
        load_sreg(SREG_SS);
        load_sreg(SREG_CS);
        rm.data_size = data_size;

        len += modrm_rm(eip + 1, &rm);

        operand_read(&rm);
        des.val = rm.val & mask_code;
        operand_write(&des);
        print_asm_2("push", "", len, &des, &rm);
        return len;
}
make_instr_func(push_r_v)
{
        OPERAND r, des;
        int len = 1;

        r.data_size = data_size;
        r.type = OPR_REG;
        r.addr = 0x7 & opcode;
        // len+=modrm_opcode(eip,&r);

        cpu.esp -= data_size / 8;
        des.data_size = data_size;
        des.type = OPR_MEM;
        des.sreg = SREG_SS;
        des.addr = cpu.esp;
        load_sreg(SREG_CS);
        load_sreg(SREG_SS);
        operand_read(&r);

        des.val = r.val;
        if (r.addr == REG_ESP)
        {
                des.val += data_size / 8;
        }
        operand_write(&des);
        print_asm_1("push", "", len, &r);

        return len;
}

make_instr_func(pusha)
{
        int len = 1;
        OPERAND esp;
        esp.type = OPR_MEM;
        esp.data_size = data_size;
        esp.addr = cpu.esp;
        esp.sreg = SREG_CS;
        operand_read(&esp);
        push(cpu.eax);
        push(cpu.ecx);
        push(cpu.edx);
        push(cpu.ebx);
        push(esp.val);
        push(cpu.ebp);
        push(cpu.esi);
        push(cpu.edi);

        print_asm_0("push", "", len);
        return len;
}