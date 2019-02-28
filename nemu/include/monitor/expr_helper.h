#include "nemu.h"
#include "cpu/reg.h"
#include "memory/memory.h"
#include "elf.h"
#include <stdlib.h>
/**
 * Put the value into str.
 * 
*/
bool put_val_str(char *dest, char *begin, uint32_t length);

/**
 * begin :The begin index of the arr.
 * end: The end index of the arr.
 * success: We have to put the value of the success to see wether the expr is valid.
 * 
*/
uint32_t eval(int begin, int end, bool *success);
/*
*
*/
int check_parentheses(int begin, int end); //consider the special conditions.

//ret the position of the dominant operator
uint32_t dominant_op(int begin, int end);

/*
Get the value in the str.
*/
uint32_t val_fetch(char *str, int type);
bool is_operand(int type);
uint32_t operator_priority(uint32_t index);

uint32_t get_hex(char *str);
uint32_t get_reg(char *str);
uint32_t get_symbol(char *str);
uint32_t get_num(char *str);