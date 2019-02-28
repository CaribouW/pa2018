#ifndef __INSTR_SPECIAL_H__
#define __INSTR_SPECIAL_H__

make_instr_func(inv);
make_instr_func(nemu_trap);
make_instr_func(break_point);
make_instr_func(nop);
make_instr_func(data_size_16);
make_instr_func(rep_repe);
make_instr_func(cltd);
make_instr_func(hlt);
make_instr_func(opcode_2_byte);
make_instr_func(cbw_a_v);

//segment
make_instr_func(lgdt);
make_instr_func(lidt);
make_instr_func(cli);
make_instr_func(sti);
make_instr_func(int_);
make_instr_func(iret);
// pusha popa
#endif
