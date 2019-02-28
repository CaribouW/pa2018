#include "cpu/cpu.h"
#include "memory/memory.h"
#ifdef IA32_PAGE
typedef union {
	struct
	{
		uint32_t offset : 12;
		uint32_t page_index : 10;
		uint32_t dic_index : 10;
	};
	uint32_t val;
} laddr_analyse;
// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	laddr_analyse l;
	l.val = laddr;
	PDE *pde_header = (void *)(hw_mem + (cpu.cr3.PDBR << 12));
	PDE target_dic = pde_header[l.dic_index];
	// assert(pde_header->present);
	PTE *pte_header = (void *)(hw_mem +
							   (target_dic.page_frame << 12));
	PTE target_tbl = pte_header[l.page_index];
	assert(target_tbl.present);
	return (target_tbl.page_frame << 12) + l.offset;
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
	;
#endif
}
#endif