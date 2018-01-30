#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define NR_PT ((SCR_SIZE + PT_SIZE - 1) / PT_SIZE)  // number of page tables to cover the vmem

static PDE vptable[NR_PT] align_to_page;
PDE* get_updir();

void create_video_mapping() {
	
	/* TODO: create an identical mapping from virtual memory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memeory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
	//assert(0);
	//panic("please implement me");
	PDE *updir=get_updir();
	PTE *vtable=(PTE*)va_to_pa(vptable);
	updir->val=make_pde(vtable);
	uint32_t pframe_addr=VMEM_ADDR;
	vtable+=160;
	for(;pframe_addr<VMEM_ADDR+SCR_SIZE;pframe_addr+=PAGE_SIZE)
	{
		((PTE*)(pa_to_va((void*)vtable)))->val=make_pte(pframe_addr);
		vtable++;
	}
}

void video_mapping_write_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		buf[i] = i;
	}
}

void video_mapping_read_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		assert(buf[i] == i);
	}
}

void video_mapping_clear() {
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

