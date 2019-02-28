#include "cpu/instr.h"

// make_instr_func(jmp_near);
// make_instr_func(jmp_far_imm);           EA  far jump.
// make_instr_func(jmp_short);             J   short jump.
// make_instr_func(jmp_near_indirect);

//ea jump to the disp of 32 bits.
make_instr_func(jmp_near)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        // rel.sreg = CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, data_size);
        print_asm_1("jmp", "", 2, &rel);

        cpu.eip += offset;
        cpu.eip &= mask_code;
        return 1 + rel.data_size / 8;
}

//eb  jump to the disp of 8bits.
make_instr_func(jmp_short)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        // rel.sreg = CS;
        rel.data_size = 8;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, rel.data_size);
        print_asm_1("jmp", "", 2, &rel);
        cpu.eip += offset;

        return 1 + rel.data_size / 8;
}
make_instr_func(jmp_far_imm)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        // rel.sreg = SREG_DS;
        rel.data_size = data_size;
        rel.addr = eip + 1;
        operand_read(&rel);
        int offset = sign_ext(rel.val, data_size);
        load_sreg(rel.sreg);
        print_asm_1("jmp", "", 2, &rel);
        cpu.eip = offset;
        return 0;
}

//FF
make_instr_func(jmp_near_indirect)
{
        int len = 1;
        OPERAND des;
        des.data_size = data_size;
        len += modrm_rm(eip + 1, &des);
        operand_read(&des);
        print_asm_1("jmp", "", 2, &des);
        cpu.eip = des.val;
        cpu.eip = cpu.eip & mask_code;

        return 0;
}
