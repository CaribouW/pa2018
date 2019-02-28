#include"cpu/instr.h"
static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	// opr_dest.val = alu_and(opr_src.val, opr_dest.val, opr_dest.data_size);
    // cpu.eflags.CF=cpu.eflags.OF=0;
	opr_dest.val = alu_and(
		sign_ext(opr_src.val,opr_src.data_size), 
	sign_ext(opr_dest.val,opr_dest.data_size), 
	opr_dest.data_size
	);
}

make_instr_impl_2op(test, i, rm, b)
make_instr_impl_2op(test, i, rm, bv)
make_instr_impl_2op(test, i, rm, v)

make_instr_impl_2op(test, r, rm, v)
make_instr_impl_2op(test, r, rm, b)

make_instr_impl_2op(test, rm, r, v)
make_instr_impl_2op(test, rm, r, b)

make_instr_impl_2op(test, i, a, b)
make_instr_impl_2op(test, i, a, v)
