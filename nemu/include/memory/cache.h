#ifndef __CACHE_H__
#define __CACHE_H__
#include "nemu.h"
#include "memory/mmu/segment.h"
#include "memory/mmu/page.h"
#include "memory/mmu/tlb.h"

//Cache part
//64 B cache_block
#define CACHE_BLOCK_SIZE_B 64
//64 KB cache
#define CACHE_data_SIZE_B 64 * 1024
//line-counter is 1024. We have 1024 lines
#define CACHE_LINE 1024
//8-way
#define CACHE_WAY 8

#define tag_part_len 19
#define group_part_len 7
#define block_part_len 6
typedef struct
{
    uint8_t data_block[64];
    uint32_t tag_part : 19;
    uint32_t valid_bit : 1;
} CacheLine;

//cache
CacheLine L1_dcache[CACHE_LINE];

//paddr split
typedef union {
    uint32_t addr;
    struct
    {
        uint32_t offset : 6;
        uint32_t group_index : 7;
        uint32_t tag : 19;
    };
    struct
    {
        uint32_t base_offset : 6;
        uint32_t base : 26;
    };
} cache_addr;

void init_cache();
uint32_t cache_read(paddr_t paddr,
                    size_t len, CacheLine *cache);
void cache_write(paddr_t paddr,
                 size_t len, 
                 uint32_t data, 
                 CacheLine *cache);
#endif