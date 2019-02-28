#include <sys/types.h>
#include <regex.h>
#include "monitor/expr_helper.h"
#include "cpu/cpu.h"
uint32_t get_hex(char *str)
{
    int index = 2;
    uint32_t result = 0;
    while (str[index] != '\0')
    {
        result = result * 16;
        char ch = str[index];
        if (ch >= '0' && ch <= '9')
            result += ch - '0';
        else if (ch >= 'a' && ch <= 'z')
            result += ch - 'a' + 10;
        else if (ch >= 'A' && ch <= 'Z')
            result += ch - 'A' + 10;
        index++;
    }
    return result;
}

uint32_t get_num(char *str)
{
    int index = 0;
    uint32_t result = 0;
    while (str[index] != '\0')
    {
        result = result * 10;
        result += str[index] - '0';
        index++;
    }
    return result;
}
uint32_t get_reg(char *str)
{
    int index = 2;

    switch (str[index])
    {
    case 'a':
        return cpu.eax;
    case 'c':
        return cpu.ecx;
    case 'd':
        return cpu.edx;
    case 'b':
        return cpu.ebx;
    case 's':
        if (str[index + 1] == 'p')
            return cpu.esp;
        else if (str[index + 1] == 'i')
            return cpu.esi;
    default:
        printf("Invalid reg!\n");
        assert(0);
    }
    return 0;
}
uint32_t get_symbol(char *str)
{
    bool s = true;
    bool *success = &s;
    uint32_t result = look_up_fun_symtab(str, success);
    if (success)
        return result;
    else
        return 0;
}

bool put_val_str(char *dest, char *begin, uint32_t length)
{
    if (length > 31)
        return false;
    else
    {
        int index;
        for (index = 0; index < length; index++)
        {
            dest[index] = begin[index];
        }
        while (index < 32)
            dest[index++] = '\0';
        return true;
    }
}