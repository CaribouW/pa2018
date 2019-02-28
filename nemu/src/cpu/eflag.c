#include "cpu/cpu.h"
/*
0xFFFFFFFF>>(32-data_size) means that we create the number that 
has (data_size) numbers of '1' in the lower bits.

we use & operation to get the lower bits of the result;
*/

/**
 * We have to do the sign_ext before the 
 * calculation.
 * OF is used in the signed operation.
 * CF is used in the unsigned operation.
 * 
*/

/**
*  Get the Zero flag.
*/
void set_ZF(uint32_t result, size_t data_size)
{
    result = result & (0xFFFFFFFF >> (32 - data_size));
    cpu.eflags.ZF = (result == 0);
}
/**
 * Get the sign flag
 * 
*/
void set_SF(uint32_t result, size_t data_size)
{
    result = sign_ext((result & mask_code), data_size);
    cpu.eflags.SF = sign(result);
}

void set_PF(uint32_t result)
{
    int i = 0;
    int sum = 0;
    for (i = 0; i < 8; i++)
    {
        sum += result & 1;
        result = result >> 1; //move rightWard.
    }
    cpu.eflags.PF = (sum % 2 == 0);
}
/**
 * While adding, and we check the carry flag
 * We could see that if the src is less than the result, the carry occurs.
*/

void set_CF_add(uint32_t result, uint32_t src, size_t data_size)
{
    result = sign_ext(result & mask_code, data_size); //符号拓展
    src = sign_ext(src & mask_code, data_size);
    cpu.eflags.CF = result < src; //since the operator is int32_t , So the carry bit will let the
                                  //result less than the src.
}

void set_OF_add(uint32_t result, uint32_t src, uint32_t dest, size_t data_size)
{
    result = sign_ext(result & mask_code, data_size);
    src = sign_ext(src & mask_code, data_size);
    dest = sign_ext(dest & mask_code, data_size);
    // switch (data_size)
    // {
    // case 8:
    //     result = sign_ext(result & 0xFF, 8);
    //     src = sign_ext(src & 0xFF, 8);
    //     dest = sign_ext(dest & 0xFF, 8);
    //     break;
    // case 16:
    //     result = sign_ext(result & 0xFFFF, 16);
    //     src = sign_ext(src & 0xFFFF, 16);
    //     dest = sign_ext(dest & 0xFFFF, 16);
    //     break;
    // default:
    //     break;
    // }
    if (sign(src) == sign(dest))
    { //overflow only occur when the sign is the same.
        if (sign(src) != sign(result))
            cpu.eflags.OF = 1;
        else
            cpu.eflags.OF = 0;
    }
    else
    {
        cpu.eflags.OF = 0;
    }
}
/**
 * signed adc.
 * 
*/
void set_OF_adc(uint32_t result, uint32_t src, uint32_t dest, size_t data_size)
{
    set_OF_add(result, src, dest, data_size);
}

void set_CF_adc(uint32_t res, uint32_t src, size_t data_size)
{
    res = sign_ext(res & mask_code, data_size);
    src = sign_ext(src & mask_code, data_size);
    if (cpu.eflags.CF == 0)
    {
        cpu.eflags.CF = (res < src);
    }
    else
    {
        cpu.eflags.CF = (res <= src);
    }
}

void set_OF_sub(uint32_t result, uint32_t src, uint32_t dest, size_t data_size)
{

    result = sign_ext(result & mask_code, data_size);
    src = sign_ext(src & mask_code, data_size);
    dest = sign_ext(dest & mask_code, data_size);
    if (sign(src) != sign(dest)) //result=dest-src
    {
        if (sign(result) != sign(dest))
            cpu.eflags.OF = 1;
        else
            cpu.eflags.OF = 0;
    }
    else
    {
        cpu.eflags.OF = 0;
    }
}
void set_CF_sub(uint32_t result, uint32_t src, size_t data_size)
{
    result = sign_ext(result & mask_code, data_size);
    src = sign_ext(src & mask_code, data_size);
    cpu.eflags.CF = (result > src); // if dest-src<0 then cf=1; and the result=dest-src+2^n
}

void set_CF_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
    dest = sign_ext(dest & mask_code, data_size);
    src = sign_ext(src & mask_code, data_size);
    if (cpu.eflags.CF != 0)
    {
        cpu.eflags.CF = (dest <= src);
    }
    else
        cpu.eflags.CF = dest < src;
}
void set_OF_sbb(uint32_t result, uint32_t src, uint32_t dest, size_t data_size)
{
    set_OF_sub(result, src, dest, data_size);
}

void set_OF_mul(uint64_t result, size_t data_size)
{
    uint32_t hight_bit = result >> (data_size);
    cpu.eflags.OF = hight_bit != 0;
}
void set_CF_mul(uint64_t result, size_t data_size)
{
    uint32_t hight_bit = result >> (data_size);
    cpu.eflags.CF = hight_bit != 0;
}

void set_OF_imul(uint64_t result, size_t data_size)
{
    result = sign_ext_64(result & mask_code, data_size);
    uint8_t signs = sign(result);
    uint32_t high_bits = result >> data_size;
    if ((signs == 1 && high_bits == 0xFFFFFFFF) || (signs == 0 && high_bits == 0))
    {
        cpu.eflags.OF = 0;
    }
    else
        cpu.eflags.OF = 1;
}

void set_CF_imul(uint64_t result, size_t data_size)
{
    result = sign_ext_64(result & mask_code, data_size);
    uint8_t signs = sign(result);
    uint32_t high_bits = result >> data_size;
    if ((signs == 1 && high_bits == 0xFFFFFFFF) || (signs == 0 && high_bits == 0))
    {
        cpu.eflags.CF = 0;
    }
    else
        cpu.eflags.CF = 1;
}
/**
 * Move the last bit outing into CF
 * 
*/
void set_CF_shl(uint32_t dest, uint32_t src, size_t data_size)
{
    src = src & mask_code;
    cpu.eflags.CF = dest >> (data_size - src) & 1;
}

void set_CF_shr(uint32_t dest, uint32_t src, size_t data_size)
{
    src = src & mask_code;
    cpu.eflags.CF = dest >> (src - 1) & 1;
}
void set_OF_shl(uint32_t dest, uint32_t src, size_t data_size)
{
}

void set_OF_shr(uint32_t dest, uint32_t src, size_t data_size)
{
}

void set_CF_rol(uint32_t dest, uint32_t src, size_t data_size)
{
}
void set_OF_rol(uint32_t dest, uint32_t src, size_t data_size)
{
    if (src != 1)
        return;
    else
    {
        cpu.eflags.OF = low_bit(dest >> (data_size - 1)) ^
                        cpu.eflags.CF;
    }
}
void set_OF_rcr(uint32_t dest, uint32_t src, size_t data_size)
{
}
void set_CF_rcr(uint32_t dest, uint32_t src, size_t data_size)
{
    set_CF_ror(dest, src, data_size);
}

void set_CF_rcl(uint32_t dest, uint32_t src, size_t data_size)
{
}
void set_OF_rcl(uint32_t dest, uint32_t src, size_t data_size)
{
    set_OF_rol(dest, src, data_size);
}

void set_OF_ror(uint32_t dest, uint32_t src, size_t data_size)
{
    if (src != 1)
        return;
    else
    {
        cpu.eflags.OF = low_bit(dest >> (data_size - 1)) ^
                        low_bit(dest >> (data_size - 2));
    }
}

void set_CF_ror(uint32_t dest, uint32_t src, size_t data_size)
{
}