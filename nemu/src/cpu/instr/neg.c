#include "cpu/instr.h"

static void instr_execute_1op()
{
    operand_read(&opr_src);
    cpu.eflags.CF = opr_src.val == 0 ? 0 : 1;
    opr_src.val = -opr_src.val;
    operand_write(&opr_src);
}

make_instr_impl_1op(neg, rm, b)
make_instr_impl_1op(neg, rm, v)
