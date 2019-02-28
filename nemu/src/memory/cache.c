#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/cache.h"
#include "memory/memory.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#define random(x) (rand() % x)
static void fetch_mem(paddr_t paddr, CacheLine *line);

void init_cache()
{
	for (int index = 0; index < CACHE_LINE; index++)
	{
		L1_dcache->valid_bit = 0;
	}
}

uint32_t cache_read(paddr_t paddr,
					size_t len, CacheLine *cache)
{
	if (len <= 0)
		return 0;
	bool hit = false;
	uint32_t result = 0;
	int index;

	cache_addr splited_addr;
	splited_addr.addr = paddr; //Split the paddr
	CacheLine *base_cache_group = cache + splited_addr.group_index * CACHE_WAY;
	//If we surge the data is larger than the single cache line
	if (len + splited_addr.offset > CACHE_BLOCK_SIZE_B)
	{
		// deal with two part of the cahe
		// We compute the two parts of the data among the two different cache group
		uint32_t new_len = len + splited_addr.offset - CACHE_BLOCK_SIZE_B;
		uint32_t current_len = len - new_len;
		uint32_t new_paddr = paddr + current_len;
		result += cache_read(paddr, current_len, L1_dcache) +
				  (cache_read(new_paddr, new_len, L1_dcache)
				   << (current_len * 8));
		return result;
	}
	for (index = 0; index < CACHE_WAY; index++)
	{
		if (base_cache_group[index].tag_part == splited_addr.tag)
		{
			if (base_cache_group[index]
					.valid_bit != 0)
				hit = true;
			break;
		}
	}
	if (hit)
	{
		// printf("\nlen = %d \noffset = %d\n", len, splited_addr.offset);
		// index = random(8);
		fetch_mem(paddr, base_cache_group + index);
		for (int i = len - 1; i >= 0; i--)
		{
			result = result << 8;
			result += base_cache_group[index]
						  .data_block[(splited_addr.offset + i) % 64];
		}
		return result;
	}
	//Miss. we should bring the block from the memory
	else
	{
		index = random(8);
		//replace one cache line
		fetch_mem(paddr, base_cache_group + index);
		for (int i = len - 1; i >= 0; i--)
		{
			result = result << 8;
			result += base_cache_group[index]
						  .data_block[splited_addr.offset + i];
		}
		return result;
	}
	return result;
}
void cache_write(paddr_t paddr,
				 size_t len, uint32_t data, CacheLine *cache)
{
	if (len <= 0)
	{
		printf("h");
		return;
	}
	bool hit = false;
	int index;

	cache_addr splited_addr;
	splited_addr.addr = paddr; //Split the paddr
	CacheLine *base_cache_group = cache + splited_addr.group_index * CACHE_WAY;
	//If we surge the data is larger than the single cache line
	if (len + splited_addr.offset > CACHE_BLOCK_SIZE_B)
	{
		// deal with two part of the cahe
		uint32_t new_len = len + splited_addr.offset - CACHE_BLOCK_SIZE_B;
		uint32_t current_len = len - new_len;
		uint32_t new_paddr = paddr + current_len;
		//We have to write twice to accomplish the line writing
		cache_write(paddr, current_len, data & (0xFFFFFFFF >> (32 - current_len * 8)), L1_dcache);
		cache_write(new_paddr, new_len, data >> (current_len * 8), L1_dcache);
		return;
	}
	for (index = 0; index < CACHE_WAY; index++)
	{
		if (base_cache_group[index].tag_part == splited_addr.tag)
		{
			if (base_cache_group[index]
					.valid_bit != 0)
				hit = true;
			break;
		}
	}
	if (hit)
	{
		uint32_t tmp = data;
		memcpy(base_cache_group[index]
					   .data_block +
				   splited_addr.offset,
			   &data, len);
		memcpy(hw_mem + paddr, &tmp, len);
	}
	//Miss. we should bring the block from the memory
	else
	{
		index = random(8);
		//replace one cache line
		uint32_t tmp = data;
		for (int i = 0; i < len; i++)
		{
			uint8_t byte_ = data;
			base_cache_group[index]
				.data_block[splited_addr.offset + i] = byte_;
			data = data >> 8;
		}
		// memcpy(&base_cache_group[index]
		// 			.data_block[splited_addr.offset],
		// 	   &data, len);
		memcpy(hw_mem + paddr, &tmp, len); //write back
	}
}

static void fetch_mem(paddr_t paddr, CacheLine *line)
{
	cache_addr splited_addr;
	splited_addr.addr = paddr; //Split the paddr
	uint32_t base_paddr = splited_addr.base << 6;
	for (int i = 0; i < CACHE_BLOCK_SIZE_B; i++)
		line->data_block[i] = hw_mem[base_paddr + i];
	line->valid_bit = 1;
	line->tag_part = splited_addr.tag;
}