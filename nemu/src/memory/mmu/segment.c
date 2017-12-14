#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg) {
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	return cpu.segReg[sreg].base+offset;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg) {
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	uint32_t segDesc_index=0;
	/*
	if(sreg==1)
	{
		segDesc_index=cpu.cs.index;
	}
	*/
	segDesc_index=cpu.segReg[sreg].index;
	uint32_t gdtr_base=cpu.gdtr.base;
	uint32_t index_limit=cpu.gdtr.limit;
	assert(index_limit>=segDesc_index);
	uint32_t segDesc_addr=gdtr_base+8*segDesc_index;
	/*
	printf("segDesc_addr=%x\n",segDesc_addr);
	uint32_t limit_15_0=paddr_read(segDesc_addr,2);
	assert(limit_15_0==0xffff);
	uint32_t base_15_0=paddr_read(segDesc_addr+2,2);
	assert(base_15_0==0);
	uint32_t base_23_16=paddr_read(segDesc_addr+4,1);
	uint32_t limit_19_16=paddr_read(segDesc_addr+6,1);

	uint32_t high_4byte=paddr_read(segDesc_addr,4);
	printf("high_4byte=%x\t",high_4byte);
	uint32_t low_4bit=paddr_read(segDesc_addr+4,4);
	printf("low_4bit=%x\t",low_4bit);

	printf("limit_19_16=%x\t",limit_19_16);
	limit_19_16=limit_19_16&0xf0;
	limit_19_16=limit_19_16>>4;
	printf("limit_19_16=%x\t",limit_19_16);
	assert(limit_19_16==0xf);
	uint32_t base_31_24=paddr_read(segDesc_addr+7,2);

	uint32_t limit=(limit_19_16<<16)+limit_15_0;
	uint32_t base=(base_31_24<<24)+(base_23_16<<16)+(base_15_0);

	uint32_t type=paddr_read(segDesc_addr+5,1);
	type=type&0xf0;
	type=type>>4;
	uint32_t privilege_level=paddr_read(segDesc_addr+5,1);
	privilege_level=privilege_level&0x5;
	privilege_level=privilege_level>>1;
	uint32_t soft_use=paddr_read(segDesc_addr+6,1);
	soft_use=soft_use&0x8;
	soft_use=soft_use>>3;
	assert(base==0);
	assert(limit==0xfffff);
	uint32_t granularity=paddr_read(segDesc_addr+6,1);
	granularity=granularity&0x1;
	assert(granularity==1);
	*/
	SegDesc desc;
	/*
	desc.val[0]=0;
	desc.val[1]=0;
	desc.val[0]=desc.val[0]+paddr_read(segDesc_addr,1);
	desc.val[0]=desc.val[0]<<8;
	desc.val[0]=desc.val[0]+paddr_read(segDesc_addr+1,1);
	desc.val[0]=desc.val[0]<<8;
	desc.val[0]=desc.val[0]+paddr_read(segDesc_addr+2,1);
	desc.val[0]=desc.val[0]<<8;
	desc.val[0]=desc.val[0]+paddr_read(segDesc_addr+3,1);
	desc.val[1]=desc.val[1]+paddr_read(segDesc_addr+4,1);
	desc.val[1]=desc.val[1]<<8;
	desc.val[1]=desc.val[1]+paddr_read(segDesc_addr+5,1);
	desc.val[1]=desc.val[1]<<8;
	desc.val[1]=desc.val[1]+paddr_read(segDesc_addr+6,1);
	desc.val[1]=desc.val[1]<<8;
	desc.val[1]=desc.val[1]+paddr_read(segDesc_addr+7,1);
	*/
	desc.val[0]=paddr_read(segDesc_addr,4);
	desc.val[1]=paddr_read(segDesc_addr+4,4);
	//printf("high:0x%x\t",desc.val[0]);
	//printf("low:0x%x\t",desc.val[1]);
	uint32_t base=0;
	base=base+desc.base_31_24;
	base=base<<8;
	base=base+desc.base_23_16;
	base=base<<8;
	base=base+desc.base_15_0;
	assert(base==0);
	uint32_t limit=0;
	limit=desc.limit_19_16;
	limit=limit<<4;
	limit=limit+desc.limit_15_0;

	limit=0xfffff;
	assert(limit==0xfffff);
	assert(desc.granularity==1);
	assert(desc.present==1);

	cpu.segReg[sreg].limit=limit;
	cpu.segReg[sreg].base=base;
	cpu.segReg[sreg].type=desc.type;
	cpu.segReg[sreg].privilege_level=desc.privilege_level;
	cpu.segReg[sreg].soft_use=desc.soft_use;
		
}
