#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "memory/cache.h"
uint8_t hw_mem[MEM_SIZE_B];

uint32_t hw_mem_read(paddr_t paddr, size_t len) {
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data) {
	memcpy(hw_mem + paddr, &data, len);
}

uint32_t paddr_read(paddr_t paddr, size_t len) {
	/*
	uint32_t ret = 0;
#ifdef HAS_DEVICE_VGA
	if(is_mmio(paddr)!=-1)
	{
		ret=mmio_read(paddr,len,is_mmio(paddr));
	}
	else
		ret=hw_mem_read(paddr,len);
#else

	#ifdef CACHE_ENABLED
		ret = cache_read(paddr,len);
	#else
		ret=hw_mem_read(paddr,len);

	#endif

#endif
	return ret;
	*/
	if(is_mmio(paddr)==-1)
	{
		uint32_t ret=0;
#ifdef CACHE_ENABLED
		ret=cache_read(paddr,len);
#else
		ret=hw_mem_read(paddr,len);
#endif
		return ret;
	}
	else
	{
		uint32_t ret=0;
		ret=mmio_read(paddr,len,is_mmio(paddr));
		return ret;
	}
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data) {
	/*
#ifdef HAS_DEVICE_VGA
	if(is_mmio(paddr)!=-1)
		mmio_write(paddr,len,data,is_mmio(paddr));
	else
		hw_mem_write(paddr,len,data);

#else

	#ifdef CACHE_ENABLED
		cache_write(paddr,len,data);
	#else
		hw_mem_write(paddr, len, data);
	#endif
#endif
*/
	if(is_mmio(paddr)==-1)
	{
#ifdef CACHE_ENABLED
		cache_write(paddr,len,data);
#else
	hw_mem_write(paddr,len,data);
#endif
	return;
	}
	else
	{
		mmio_write(paddr,len,data,is_mmio(paddr));
		return;
	}
}


uint32_t laddr_read(laddr_t laddr, size_t len) {
	paddr_t paddr=laddr;
#ifdef IA32_PAGE
	if(cpu.cr0.pg==1)
		paddr=page_translate(laddr);
#endif
	return paddr_read(paddr,len);
}

void laddr_write(laddr_t laddr, size_t len, uint32_t data) {
	paddr_t paddr=laddr;
#ifdef IA32_PAGE
	if(cpu.cr0.pg==1)
		paddr=page_translate(laddr);
#endif

	paddr_write(paddr, len, data);
}


uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len) {
	if(cpu.cr0.pe==1)
	{
		/*
		if(vaddr%0x10==0xf&&len>1)
		{
			uint32_t result;
			uint32_t result1=laddr_read(segment_translate(vaddr,sreg),1);
			uint32_t result2=laddr_read(segment_translate(vaddr+1,sreg),len-1);
			if(len==2)
			{
				result=result
			}
		}
		*/
		vaddr=segment_translate(vaddr,sreg);
	}
	assert(len == 1 || len == 2 || len == 4);
	return laddr_read(vaddr, len);
}

void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data) {
	if(cpu.cr0.pe==1)
		vaddr=segment_translate(vaddr,sreg);
	assert(len == 1 || len == 2 || len == 4);
	laddr_write(vaddr, len, data);
}

void init_mem() {
	// clear the memory on initiation
	memset(hw_mem, 0, MEM_SIZE_B);
	init_cache();
#ifdef TLB_ENABLED
	init_all_tlb();
	make_all_tlb();
#endif
}

uint32_t instr_fetch(vaddr_t vaddr, size_t len) {
	assert(len == 1 || len == 2 || len == 4);
	return vaddr_read(vaddr, SREG_CS, len);
}

uint8_t * get_mem_addr() {
	return hw_mem;
}


