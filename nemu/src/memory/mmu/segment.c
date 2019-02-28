#include "cpu/cpu.h"
#include "memory/memory.h"
// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg)
{
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	return cpu.segReg[sreg].base + offset;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg)
{
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	SegReg target_reg = cpu.segReg[sreg];
	SegDesc *item;
	item = (void *)(hw_mem + cpu.gdtr.base +
					(target_reg.index << 3));
	target_reg.base = item->base_15_0 +
					  (item->base_23_16 << 16) +
					  (item->base_31_24 << 24);
	target_reg.limit = item->limit_15_0 +
					   (item->limit_19_16 << 16);
	// assert(item->granularity && target_reg.base == 0);
}