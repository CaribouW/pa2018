#include "cpu/cpu.h"

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	// printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//assert(0);
	//	assert(data_size == 8 | data_size == 16 | data_size == 32);
	uint32_t res = src + dest;

	set_OF_add(res, src, dest, data_size);
	set_CF_add(res, src, data_size); //其他的有什么区别？
	set_SF(res, data_size);
	set_ZF(res, data_size);
	set_PF(res);

	return res & mask_code; //mask bit
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//assert(0);
	uint32_t res = dest + src + cpu.eflags.CF;

	set_OF_adc(res, src, dest, data_size);
	set_SF(res, data_size);
	set_ZF(res, data_size);
	//set_AF_adc(res, src, data_size);
	set_CF_adc(res, src, data_size);
	set_PF(res);

	return res & mask_code;
#endif
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//assert(0);
	uint32_t res = dest - src;

	set_OF_sub(res, src, dest, data_size);
	set_SF(res, data_size);
	set_ZF(res, data_size);
	//set_AF_adc(res, src, data_size);
	set_CF_sub(src, dest, data_size);
	set_PF(res);

	return res & mask_code;
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	int32_t res = dest - (src + cpu.eflags.CF);
	set_OF_sbb(res, src, dest, data_size);
	set_SF(res, data_size);
	set_ZF(res, data_size);
	set_CF_sbb(src, dest, data_size);
	set_PF(res);

	return res & mask_code;
#endif
}
/**
 * We put the uint32_t numbers in the 64bits register.
 * 
 * 
*/
uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	uint64_t src64 = (uint64_t)(src & mask_code);
	uint64_t dest64 = (uint64_t)(dest & mask_code);
	uint64_t res = src64 * dest64;
	set_OF_mul(res, data_size);
	set_CF_mul(res, data_size);
	return res;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	uint64_t src64 = sign_ext(src, data_size);
	uint64_t dest64 = sign_ext(dest, data_size);
	uint64_t result = src64 * dest64;
	set_OF_imul(result, data_size);
	set_CF_imul(result, data_size);
	return result;
#endif
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	assert(src); //除数为零的时候报错退出
	src = (uint64_t)(src & mask_code);
	dest = (uint64_t)(dest & mask_code);
	uint64_t result = dest / src;
	// result = (uint32_t)result; //get the quotient.
	return result & mask_code;
#endif
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//assert(0);
	int64_t src64 = sign_ext_64(src & mask_code, data_size);
	int64_t dest64 = sign_ext_64(dest & mask_code, data_size);
	int64_t result = dest64 / src64;
	return result & mask_code;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//	assert(0);
	assert(src); //除数为零的时候报错退出
	return (uint32_t)(dest % src);
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	int64_t result = dest % src;
	return (int32_t)result;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//assert(0);
	uint32_t result;
	result = src & dest;
	set_SF(result, data_size);
	set_ZF(result, data_size);
	set_PF(result);
	cpu.eflags.CF = cpu.eflags.OF = 0;

	return result & mask_code;

#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//assert(0);
	//return 0;
	uint32_t result;
	result = src ^ dest;

	set_SF(result, data_size);
	set_ZF(result, data_size);
	set_PF(result);

	return result & mask_code;
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	//	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//	assert(0);
	//	return 0;
	uint32_t result = src | dest;

	set_SF(result, data_size);
	set_ZF(result, data_size);
	set_PF(result);

	return result & mask_code;
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	uint32_t result = dest << src;
	set_CF_shl(dest, src, data_size);
	set_SF(result, data_size);
	set_ZF(result, data_size);
	set_PF(result);
	return result & mask_code;
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	return alu_sal(src, dest, data_size);
#endif
}

//logical shift.
uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//assert(0);
	//return 0;
	dest = dest & mask_code;
	uint32_t result = dest >> src; //uint类型，右移就是逻辑右

	set_CF_shr(dest, src, data_size);
	set_PF(result);
	set_SF(result, data_size);
	set_ZF(result, data_size);

	return result & mask_code;

#endif
}

//算术右移,左边补符号
uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//assert(0);
	int32_t dest32 = sign_ext(dest & mask_code, data_size); //转换成int类型，这样右移就是算术右移
	dest32 = dest32 >> src;
	uint32_t result = dest32;
	set_CF_shr(dest, src, data_size);
	set_PF(result);
	set_SF(result, data_size);
	set_ZF(result, data_size);

	return result & mask_code;
#endif
}

uint32_t alu_rol(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t result = dest & mask_code;
	uint32_t tmp = src;
	while (tmp > 0)
	{
		//get the missing bit
		uint8_t bit = high_bit(result);
		// uint8_t bit=(result>>(data_size-1))&0x1;

		//launch the shift
		result <<= 1;
		//plus the bit to the low-bit.
		result &= bit;
		//set the CF
		cpu.eflags.CF = bit;
		tmp--;
	}
	set_OF_rol(result, src, data_size);

	return result & mask_code;
}
uint32_t alu_ror(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t result = dest & mask_code;
	uint32_t tmp = src;
	while (tmp > 0)
	{
		//get the missing bit
		uint8_t bit = low_bit(result);
		// uint8_t bit=(result>>(data_size-1))&0x1;

		//launch the shift
		result >>= 1;
		//plus the bit to the low-bit.
		result &= (bit << (data_size-1));
		//set the CF
		cpu.eflags.CF = bit;
		tmp--;
	}
	set_OF_ror(result, src, data_size);
	return result;
}
uint32_t alu_rcl(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t result = dest & mask_code;
	uint32_t tmp = src;

	while (tmp > 0)
	{
		uint8_t tmp_CF = cpu.eflags.CF;
		cpu.eflags.CF = high_bit(result);
		result <<= 1;
		result &= tmp_CF;
		tmp--;
	}

	set_OF_rcl(result, src, data_size);
	return result;
}
uint32_t alu_rcr(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t result = dest & mask_code;
	uint32_t tmp = src;

	while (tmp > 0)
	{
		uint8_t tmp_CF = cpu.eflags.CF;
		cpu.eflags.CF = low_bit(result);
		result >>= 1;
		result &= (tmp_CF << (data_size - 1));
		tmp--;
	}

	set_OF_rcr(result, src, data_size);
	return result;
}