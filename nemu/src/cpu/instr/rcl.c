#include"cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_rcl(
		sign_ext(opr_src.val,opr_src.data_size), 
	sign_ext(opr_dest.val,opr_dest.data_size), 
	opr_dest.data_size
	);
	// opr_dest.val = alu_rcl(opr_src.val, opr_dest.val, opr_dest.data_size);
	operand_write(&opr_dest);
}

make_instr_impl_2op(rcl, i, rm, b)
make_instr_impl_2op(rcl, i, rm, bv)
make_instr_impl_2op(rcl, c, rm, b)
make_instr_impl_2op(rcl, c, rm, bv)

static void instr_execute_1op() {
	operand_read(&opr_src);
	opr_src.val = alu_rcl(1, opr_src.val, opr_src.data_size);
	operand_write(&opr_src);
}

make_instr_impl_1op(rcl, rm, b)
make_instr_impl_1op(rcl, rm, v)