#include "cpu/cpu.h"
#include "memory/memory.h"
#include <stdlib.h>
// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr) {
#ifndef TLB_ENABLED
	//printf("\nPlease implement page_translate()\n");
	//assert(0);
	printf("cr3=%x\n",cpu.cr3.page_directory_base);
	system("pause");
	uint32_t pdb=cpu.cr3.page_directory_base;
	uint32_t laddr_11_0;
	uint32_t laddr_21_12;
	uint32_t laddr_31_22;
	laddr_11_0=laddr&0xfff;
	laddr=laddr>>12;
	laddr_21_12=laddr&0x3ff;
	laddr=laddr>>10;
	laddr_31_22=laddr&0x3ff;
	PDE page_table_1;
	page_table_1.val=paddr_read(pdb+laddr_31_22*4,4);
	assert(page_table_1.present==1);

	PTE page_table_2;
	page_table_2.val=paddr_read(page_table_1.page_frame+laddr_21_12*4,4);
	assert(page_table_2.present==1);

	uint32_t paddr=page_table_2.page_frame+laddr_11_0;
	return paddr;
#else	
	return tlb_read(laddr) | (laddr & PAGE_MASK);;
#endif
}
