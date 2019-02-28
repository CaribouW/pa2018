#include "cpu/instr.h"
static void instr_execute_2op() {
	operand_read(&opr_src);
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);
}
make_instr_impl_2op(mov, r, rm, b)
make_instr_impl_2op(mov, r, rm, v)
make_instr_impl_2op(mov, rm, r, b)
make_instr_impl_2op(mov, rm, r, v)
make_instr_impl_2op(mov, i, rm, b)
make_instr_impl_2op(mov, i, rm, v)
make_instr_impl_2op(mov, i, r, b)
make_instr_impl_2op(mov, i, r, v)
make_instr_impl_2op(mov, a, o, b)
make_instr_impl_2op(mov, a, o, v)
make_instr_impl_2op(mov, o, a, b)
make_instr_impl_2op(mov, o, a, v)

// make_instr_func(mov_rm2s_w){
// 	int len=1;
// 	return len;
// }

make_instr_func(mov_c2r_l){
	int len=1;
	OPERAND c,r;
	c.type=OPR_CREG;
	c.data_size=32;
	r.type=OPR_REG;
	r.data_size=32;
	//We have to analyse the mod/rm bits.
	// len+=modrm_r_rm(eip+1,&r,&c);
	len+=modrm_rm(eip+1,&r);
	uint32_t instr=instr_fetch(eip+1,1);
	uint8_t seg_index=(instr>>3)&(0x07);
	if(seg_index==0){
		r.val=cpu.cr0.val;
	}
	else{
		#ifdef IA32_PAGE
		r.val=cpu.cr3.val;
		// printf("seg index= %d\n",seg_index);
		// assert(0);
		#endif
	}
	operand_write(&r);
	print_asm_2("mov", "", len, &c, &r);
	return len;
}
make_instr_func(mov_r2c_l){
	int len=1;
	OPERAND c,r;
	c.type=OPR_CREG;
	c.data_size=32;
	r.type=OPR_REG;
	r.data_size=32;
	len+=modrm_rm(eip+1,&r);
	operand_read(&r);
	uint32_t instr=instr_fetch(eip+1,1);
	uint8_t seg_index=(instr>>3)&(0x07);
	if(seg_index==0){
		cpu.cr0.val=r.val;
	}
	else{
		#ifdef IA32_PAGE
		cpu.cr3.val=r.val;
		// printf("seg index= %d\n",seg_index);
		// assert(0);
		#endif
	}
	print_asm_2("mov", "", len, &r, &c);
	return len;
}

make_instr_func(mov_rm2s_w){
	int len=1;
	OPERAND s,r;
	s.type=OPR_SREG;
	s.data_size=16;
	r.type=OPR_REG;
	r.data_size=32;
	len+=modrm_r_rm(eip+1,&r,&s);
	operand_read(&r);
	s.val=r.val;
	operand_write(&s);
	print_asm_2("mov", "", len, &r, &s);
	return len;
}
/*
make_instr_func(mov_r2rm_b) {
	src.data_size = 8;
	dest.data_size = 8;

	len += modrm_r_rm(eip + 1, &src, &dest);
	
	OPERAND r, rm;
	r.data_size = 8;
	rm.data_size = 8;
	int len = 1;
	// read ModR/M for src and dest
	len += modrm_r_rm(eip + 1, &r, &rm);
	//operand_read(&r);
	//rm.val = r.val;
	//operand_write(&rm);
	execute();
	return len;
}*/



// make_instr_func(mov_r2rm_v) {
// 	OPERAND r, rm;
// 	r.data_size = data_size;
// 	rm.data_size = data_size;
// 	int len = 1;
// 	// read ModR/M for src and dest
// 	len += modrm_r_rm(eip + 1, &r, &rm);
// 	//printf("mov_r2rm rm addr = 0x%08x\n", rm.addr);
// 	//operand_read(&r);
// 	//rm.val = r.val;
// 	//operand_write(&rm);
// 	execute(&r, &rm);
// 	return len;
// }

// make_instr_func(mov_rm2r_b) {
// 	OPERAND r, rm;
// 	r.data_size = 8;
// 	rm.data_size = 8;

// 	int len = 1;
// 	// read ModR/M for src and dest
// 	len += modrm_r_rm(eip + 1, &r, &rm);

// 	//operand_read(&rm);
// 	//r.val = rm.val;
// 	//operand_write(&r);
// 	execute(&rm, &r);
// 	return len;
// }

// make_instr_func(mov_rm2r_v) {
// 	OPERAND r, rm;
// 	r.data_size = data_size;
// 	rm.data_size = data_size;

// 	int len = 1;
// 	// read ModR/M for src and dest
// 	len += modrm_r_rm(eip + 1, &r, &rm);

// 	//operand_read(&rm);
// 	//r.val = rm.val;
// 	//operand_write(&r);
// 	execute(&rm, &r);
// 	return len;
// }

// make_instr_func(mov_i2rm_v) {
// 	OPERAND rm, imm;
// 	rm.data_size = data_size;
// 	imm.data_size = data_size;

// 	int len = 1;
// 	// read ModR/M for src and dest
// 	len += modrm_rm(eip + 1, &rm);

// 	imm.type = OPR_IMM;
// 	imm.addr = eip + len;   //The struct of the instr
// 	imm.data_size = data_size;
// 	// printf("addr imm %08x, len=%d\n", imm.addr, len);

// 	operand_read(&imm);
// 	rm.val = imm.val;
// 	operand_write(&rm);
// 	return len + data_size / 8;
// }

// make_instr_func(mov_i2r_b) {
// 	OPERAND r, imm;      //get the imm and the register.
// 	r.data_size = 8;
// 	r.type = OPR_REG;
// 	r.addr = opcode & 0x7;

// 	imm.type = OPR_IMM;
// 	imm.addr = eip + 1;
// 	imm.data_size = 8;

// 	operand_read(&imm);
// 	r.val = imm.val;
// 	operand_write(&r);

// 	// execute(&imm, &r);
// 	return 2;
// }

// make_instr_func(mov_i2r_v) {
// 	OPERAND r, imm;
// 	r.data_size = data_size;
// 	r.type = OPR_REG;
// 	r.addr = opcode & 0x7;

// 	imm.type = OPR_IMM;
// 	imm.addr = eip + 1;
// 	imm.data_size = data_size;

// 	//operand_read(&imm);
// 	//r.val = imm.val;
// 	//operand_write(&r);

// 	execute(&imm, &r);
// 	return 1 + data_size / 8;
// }

// make_instr_func(mov_off2a_b) {
// 	OPERAND r, m;
// 	r.data_size = 8;
// 	r.type = OPR_REG;
// 	r.addr = REG_AL;

// 	m.type = OPR_MEM;
// 	m.sreg = DS;
// 	m.addr = instr_fetch(eip + 1, 4);
// 	m.data_size = 8;

// 	operand_read(&m);
// 	r.val = m.val;
// 	operand_write(&r);
// 	return 5;
// }


// make_instr_func(mov_off2a_v) {
// 	OPERAND r, m;
// 	r.data_size = data_size;
// 	r.type = OPR_REG;
// 	r.addr = REG_AX;
	
// 	m.type = OPR_MEM;
// 	m.sreg = DS;
// 	m.addr = instr_fetch(eip + 1, 4);
// 	m.data_size = data_size;

// 	operand_read(&m);
// 	r.val = m.val;
// 	operand_write(&r);
// 	return 5;
// }

// make_instr_func(mov_a2off_b) {
// 	OPERAND r, m;
// 	r.data_size = 8;
// 	r.type = OPR_REG;
// 	r.addr = REG_AL;

// 	m.type = OPR_MEM;
// 	m.sreg = DS;
// 	m.addr = instr_fetch(eip + 1, 4);
// 	m.data_size = 8;

// 	operand_read(&r);
// 	m.val = r.val;
// 	operand_write(&m);
// 	return 5;
// }

// make_instr_func(mov_a2off_v) {
// 	OPERAND r, m;
// 	r.data_size = data_size;
// 	r.type = OPR_REG;
// 	r.addr = REG_AX;

// 	m.type = OPR_MEM;
// 	m.sreg = DS;
// 	m.addr = instr_fetch(eip + 1, 4);
// 	m.data_size = data_size;

// 	operand_read(&r);
// 	m.val = r.val;
// 	operand_write(&m);
// 	return 5;
// }



make_instr_func(mov_zrm82r_v) {
	int len = 1;
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = 8;
	len += modrm_r_rm(eip + 1, &r, &rm);
	
	operand_read(&rm);
	r.val = rm.val;
	operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
	return len;
}

make_instr_func(mov_zrm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);

        operand_read(&rm);
        r.val = rm.val;
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm82r_v) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = data_size;
        rm.data_size = 8;
        len += modrm_r_rm(eip + 1, &r, &rm);
        
	operand_read(&rm);
        r.val = sign_ext(rm.val, 8);
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);
        operand_read(&rm);
        r.val = sign_ext(rm.val, 16);
        operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
        return len;
}