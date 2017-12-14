#ifndef CACHE_H
#define CACHE_H
#include "nemu.h"
#include "memory/mmu/segment.h"
#include "memory/mmu/page.h"
#include "memory/mmu/tlb.h"
typedef struct line
{
	char valid_bit;
	uint32_t sign;
	uint8_t content[64];
}  Line;
Line L1_dcache[1024];
void init_cache();
uint32_t cache_read(paddr_t paddr,size_t len);
void cache_write(paddr_t paddr,size_t len,uint32_t data);
#endif
