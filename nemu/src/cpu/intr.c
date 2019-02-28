#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"
#ifdef IA32_INTR
static void push(uint32_t num)
{
	OPERAND esp;
	cpu.esp -= data_size / 8; //dec
	esp.type = OPR_MEM;
	esp.data_size = data_size;
	esp.addr = cpu.esp;
	esp.sreg = SREG_CS;
	esp.val = num & mask_code; //push
	operand_write(&esp);
}
#endif
void raise_intr(uint8_t intr_no)
{ //used in page fault , IO interupt .
#ifdef IA32_INTR
	// printf("Please implement raise_intr()");
	push(cpu.eflags.val);
	push(cpu.cs.val);
	push(cpu.eip);
	/**
	 * if externel interrput
	 * 	cpu.eflags.IF=0;
	 * 
	 * 
	 * check IDT and jump to the target intrrpution programme.
	*/

	GateDesc *item;
	paddr_t result = page_translate((uint32_t)hw_mem +
									cpu.idtr.base + (intr_no << 3));
	item = (void *)result;
	assert(item->present);
	if (item->type == 0x06)
	{ //  0110  interrupt gate type
		cpu.eflags.IF = 0;
		assert(0);
	}
	uint32_t offset = item->offset_15_0 + (item->offset_31_16 << 16);
	cpu.eip = offset;
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	//exception pit . such as _int_
	cpu.eip += 2;
	raise_intr(intr_no);
}
