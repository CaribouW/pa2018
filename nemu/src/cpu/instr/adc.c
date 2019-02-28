#include "cpu/instr.h"
static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_adc(
		sign_ext(opr_src.val,opr_src.data_size), 
	sign_ext(opr_dest.val,opr_dest.data_size), 
	opr_dest.data_size
	);
	operand_write(&opr_dest);
}
make_instr_impl_2op(adc, i, rm, b)
make_instr_impl_2op(adc, i, rm, bv)
make_instr_impl_2op(adc, i, rm, v)

make_instr_impl_2op(adc, r, rm, v)
make_instr_impl_2op(adc, r, rm, b)

make_instr_impl_2op(adc, rm, r, v)
make_instr_impl_2op(adc, rm, r, b)

make_instr_impl_2op(adc, i, a, b)
make_instr_impl_2op(adc, i, a, v)
// make_instr_func(adc_r2rm_v);
// make_instr_func(adc_r2rm_b);
// make_instr_func(adc_rm2r_b);
// make_instr_func(adc_rm2r_v);
// make_instr_func(adc_i2a_b);
// make_instr_func(adc_i2a_v);
// make_instr_func(adc_i2rm_bv){
//     OPERAND imm, rm;
//     int len = 1;

//     imm.type = OPR_IMM;
//     imm.data_size = 8;

//     rm.data_size = data_size;
//     len += modrm_rm(eip + 1, &rm);
//     imm.addr = eip + len;
//     operand_read(&imm);
//     operand_read(&rm);
//     rm.val = alu_adc(imm.val, rm.val, data_size);
//     operand_write(&rm);

//     return len + imm.data_size / 8;

// }
// make_instr_func(adc_i2rm_b){
//     OPERAND imm, rm;
//     int len = 1;

//     imm.type = OPR_IMM;
//     imm.data_size = 8;

//     rm.data_size = 8;
//     len += modrm_rm(eip + 1, &rm);
//     imm.addr = eip + len;
//     operand_read(&imm);
//     operand_read(&rm);
//     rm.val = alu_adc(imm.val, rm.val, data_size);
//     operand_write(&rm);

//     return len + imm.data_size / 8;

// }
// make_instr_func(adc_i2rm_v){
//     OPERAND imm, rm;
//     int len = 1;

//     imm.type = OPR_IMM;
//     imm.data_size = data_size;

//     rm.data_size = data_size;
//     len += modrm_rm(eip + 1, &rm);
//     imm.addr = eip + len;
//     operand_read(&imm);
//     operand_read(&rm);
//     rm.val = alu_adc(imm.val, rm.val, data_size);
//     operand_write(&rm);

//     return len + imm.data_size / 8;

// }