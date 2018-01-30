#include "common.h"
#include "memory.h"
#include "string.h"
#include <elf.h>
#include <stdlib.h>

#ifdef HAS_DEVICE_IDE
#define ELF_OFFSET_IN_DISK 0
#endif

#define STACK_SIZE (1 << 20)
void ide_read(uint8_t *, uint32_t, uint32_t);
void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {	
	assert(0);
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph, *eph;
#ifdef HAS_DEVICE_IDE
	//uint8_t buf[65536*2];
	//ide_read(buf, ELF_OFFSET_IN_DISK, 65536*2);
	uint8_t buf[4096];
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);


	elf = (void*)buf;
	Log("ELF loading from hard disk.");
#else
	elf = (void *)0x0;
	Log("ELF loading from ram disk.");
#endif

	/* Load each program segment */
	ph = (void *)elf + elf->e_phoff;
	eph = ph + elf->e_phnum;
	for(; ph < eph; ph ++) {
		if(ph->p_type == PT_LOAD) {

			//panic("Please implement the loader");

			/* TODO: copy the segment from the ELF file to its proper memory area */
#ifndef IA32_PAGE
			uint32_t addr=ph->p_vaddr;
			memcpy((void*)(addr),(void*)((void*)elf+ph->p_offset),ph->p_filesz);
			
			if(ph->p_memsz>ph->p_filesz)
				memset((void*)(addr+ph->p_filesz),0,ph->p_memsz-ph->p_filesz);
#endif

#ifdef IA32_PAGE
			uint32_t addr=ph->p_vaddr;
			uint32_t physical_addr=mm_malloc(addr,ph->p_memsz);
			
#ifdef HAS_DEVICE_IDE
			//assert(ph->p_filesz<=4096*32);
			//uint8_t buf2[4096*32];
			//ide_read(buf2,ELF_OFFSET_IN_DISK+ph->p_offset,ph->p_filesz);
			ide_read((uint8_t*)(physical_addr),ELF_OFFSET_IN_DISK+ph->p_offset,ph->p_filesz);
			//memcpy((void*)(physical_addr),(void*)(buf2),ph->p_filesz);
#else
			memcpy((void*)(physical_addr),(void*)((void*)elf+ph->p_offset),ph->p_filesz);
#endif
			if(ph->p_memsz>ph->p_filesz)
				memset((void*)(physical_addr+ph->p_filesz),0,ph->p_memsz-ph->p_filesz);


#endif



			
			/* TODO: zeror the memory area [vaddr + file_sz, vaddr + mem_sz) */

#ifdef IA32_PAGE
			/* Record the program break for future use */
			extern uint32_t brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(brk < new_brk) { brk = new_brk; }
#endif
		}
	}



	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);
#ifdef HAS_DEVICE_VGA
	assert(0);
	create_video_mapping();
#endif
	write_cr3(get_ucr3());
#endif
	return entry;
}
