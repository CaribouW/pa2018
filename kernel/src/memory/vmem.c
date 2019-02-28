#include "common.h"
#include "memory.h"
#include <string.h>
// #define PAGE_SIZE 4096				  //block size
// #define NR_PAGE (PHY_MEM / PAGE_SIZE)  //whole number of the pages
// #define NR_PDE 1024					  //number of dir of the whole
// #define NR_PTE 1024					  //number of table in every dir
// #define PAGE_MASK (4096 - 1)
// #define PT_SIZE ((NR_PTE) * (PAGE_SIZE)) //whole size in byte of each dir

#define VMEM_ADDR 0xa0000	//IO ADDR begin
#define SCR_SIZE (320 * 200) //IO addr range(byte) 64000
typedef union {
	struct
	{
		uint32_t offset : 12;
		uint32_t page_index : 10;
		uint32_t dic_index : 10;
	};
	uint32_t val;
} addr;
#define NR_PT ((SCR_SIZE + PT_SIZE - 1) / PT_SIZE) //same as NR_PDE

PDE *get_updir();
PTE ioptable[SCR_SIZE / PAGE_SIZE] align_to_page;
extern CR3 ucr3;
#define NR_IO_dir 1

void create_video_mapping()
{

	/* TODO: create an identical mapping from virtual memory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memeory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
	addr vaddr;
	vaddr.val = VMEM_ADDR;

	int frame_index = vaddr.page_index; // a0
	PDE *pdir = (PDE *)va_to_pa(get_updir());
	PTE *ptable = (PTE *)va_to_pa(ioptable); //get dir location
	// mm_malloc(vaddr.val, SCR_SIZE);
	pdir[0].val = make_pde(ptable); //put table into dir
	ptable = ptable + frame_index;
	for (uint32_t index = 0; index < 20; index++)
	{
		ptable->val = make_pte((frame_index << 12));
		frame_index++;
		ptable++;
	}
}

void video_mapping_write_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{	
		buf[i] = i;
	}
}

void video_mapping_read_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		assert(buf[i] == i);
	}
}

void video_mapping_clear()
{
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}
