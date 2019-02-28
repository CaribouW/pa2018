#include"trap.h"

int main(){
    int x=4;
    asm volatile(
        "addb $4,%eax;"
        "subb $4,%eax;"
        "movl $0xFFFFFFFF,%ecx;"
        "movb $0xFFFFFF1f,0xFF(%ecx);"
        "movl 0xFF(%ecx),%ebx;"
    );
    nemu_assert(x==4);
    HIT_GOOD_TRAP;

    return 0;
}