#include "cpu/instr.h"
static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_xor(
		sign_ext(opr_src.val,opr_src.data_size), 
	sign_ext(opr_dest.val,opr_dest.data_size), 
	opr_dest.data_size
	);
	operand_write(&opr_dest);
}

make_instr_impl_2op(xor, i, rm, b)
make_instr_impl_2op(xor, i, rm, bv)
make_instr_impl_2op(xor, i, rm, v)

make_instr_impl_2op(xor, r, rm, v)
make_instr_impl_2op(xor, r, rm, b)

make_instr_impl_2op(xor, rm, r, v)
make_instr_impl_2op(xor, rm, r, b)

make_instr_impl_2op(xor, i, a, b)
make_instr_impl_2op(xor, i, a, v)
// make_instr_func(xor_r2rm_v);
// make_instr_func(xor_r2rm_b);
// make_instr_func(xor_rm2r_b);
// make_instr_func(xor_rm2r_v);
// make_instr_func(xor_i2a_b);
// make_instr_func(xor_i2a_v);
// make_instr_func(xor_i2rm_bv)
// {
//     return 0;
// }
// make_instr_func(xor_i2rm_b)
// {
//     return 0;
// }
// make_instr_func(xor_i2rm_v)
// {
//     return 0;
// }