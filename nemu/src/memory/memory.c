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
	
	
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data) {
	
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

}


uint32_t laddr_read(laddr_t laddr, size_t len) {
	//paddr_t paddr=laddr;
#ifdef IA32_PAGE
	if(cpu.cr0.pg==1)
	{
		//paddr=page_translate(laddr);

		uint32_t pr=(laddr&0xfff);
		if(pr+len>0x1000)
		{
			if((pr==0xfff)&(len==2))
			{
				paddr_t hwaddr=page_translate(laddr);
				uint32_t temp=paddr_read(hwaddr,1);
				paddr_t hwaddr_t=page_translate(laddr+1);
				uint32_t temp_t=paddr_read(hwaddr_t,1);
				uint16_t data=(temp_t<<8)+temp;
				return data;
			}
			else if((pr==0xfff)&(len==4))
			{
				paddr_t hwaddr=page_translate(laddr);
				uint32_t temp=paddr_read(hwaddr,1);
				paddr_t hwaddr_t=page_translate(laddr+1);
				uint32_t temp_t=paddr_read(hwaddr_t,3);
				uint16_t data=(temp_t<<8)+temp;
				return data;
			}
			else if((pr==0xfff)&(len==2))
			{
				paddr_t hwaddr=page_translate(laddr);
				uint32_t temp=paddr_read(hwaddr,2);
				paddr_t hwaddr_t=page_translate(laddr+2);
				uint32_t temp_t=paddr_read(hwaddr_t,2);
				uint16_t data=(temp_t<<16)+temp;
				return data;
			}
			else if((pr==0xfff)&(len==2))
			{
				paddr_t hwaddr=page_translate(laddr);
				uint32_t temp=paddr_read(hwaddr,3);
				paddr_t hwaddr_t=page_translate(laddr+3);
				uint32_t temp_t=paddr_read(hwaddr_t,1);
				uint16_t data=(temp_t<<24)+temp;
				return data;
			}
			//else
				//assert(0);
		}
		else
		{
			paddr_t hwaddr=page_translate(laddr);
			return paddr_read(hwaddr,len);
		}

	}
	else
		return paddr_read(laddr,len);
		
#else
	return paddr_read(laddr,len);
#endif
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


