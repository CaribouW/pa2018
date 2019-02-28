#include "cpu/instr.h"
//invet 1 into 0 

static void instr_execute_1op() {
	operand_read(&opr_src);
	opr_src.val ^=mask_code; 
	operand_write(&opr_src);
}

make_instr_impl_1op(not, rm, b)
make_instr_impl_1op(not, rm, v)