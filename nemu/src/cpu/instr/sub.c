#include "cpu/instr.h"
static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_sub(
		sign_ext(opr_src.val,opr_src.data_size), 
	sign_ext(opr_dest.val,opr_dest.data_size), 
	opr_dest.data_size
	);
	operand_write(&opr_dest);
}

make_instr_impl_2op(sub, i, rm, b)
make_instr_impl_2op(sub, i, rm, bv)
make_instr_impl_2op(sub, i, rm, v)

make_instr_impl_2op(sub, r, rm, v)
make_instr_impl_2op(sub, r, rm, b)

make_instr_impl_2op(sub, rm, r, v)
make_instr_impl_2op(sub, rm, r, b)

make_instr_impl_2op(sub, i, a, b)
make_instr_impl_2op(sub, i, a, v)
// make_instr_func(sub_r2rm_v)
// {
//     OPERAND r, rm;
//     int len = 1;
//     r.data_size = data_size;
//     rm.data_size = data_size;
//     len += modrm_r_rm(eip + 1, &r, &rm);
//     operand_read(&r);
//     r.val = alu_sub(rm.val, r.val, data_size);
//     operand_write(&rm);
//     return len;
// }
// make_instr_func(sub_r2rm_b)
// {
//     return 0;
// }
// make_instr_func(sub_rm2r_b)
// {
//     return 0;
// }
// make_instr_func(sub_rm2r_v)
// {
//     return 0;
// }
// make_instr_func(sub_i2rm_b)
// {
//     OPERAND imm;
//     OPERAND rm;
//     int len = 1;

//     imm.data_size = 8;
//     imm.type = OPR_IMM;

//     rm.data_size = 8;
//     len += modrm_rm(eip + 1, &rm);
//     imm.addr = eip + len;

//     operand_read(&imm);
//     operand_read(&rm);
//     rm.val = alu_sub(rm.val, imm.val, imm.data_size);
//     operand_write(&rm);

//     return len + imm.data_size / 8;
// }
// make_instr_func(sub_i2rm_v)
// {
//     OPERAND imm;
//     OPERAND rm;
//     int len = 1;

//     imm.data_size = data_size;
//     imm.type = OPR_IMM;

//     rm.data_size = data_size;
//     len += modrm_rm(eip + 1, &rm);
//     imm.addr = eip + len;

//     operand_read(&imm);
//     rm.val = alu_sub(rm.val, imm.val, imm.data_size);
//     operand_write(&rm);

//     return len + imm.data_size / 8;
// }
// make_instr_func(sub_i2rm_bv)
// {
//     OPERAND imm;
//     OPERAND rm;
//     int len = 1;

//     imm.data_size = 8;
//     imm.type = OPR_IMM;

//     rm.data_size = data_size;
//     len += modrm_rm(eip + 1, &rm);
//     imm.addr = eip + len;

//     operand_read(&imm);
//     rm.val = alu_sub(rm.val, imm.val, imm.data_size);
//     operand_write(&rm);

//     return len + imm.data_size / 8;
// }
