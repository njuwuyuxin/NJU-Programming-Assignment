#include "common.h"
#include "memory.h"
#include "string.h"
#include <elf.h>

#ifdef HAS_DEVICE_IDE
#define ELF_OFFSET_IN_DISK 0
#endif

#define STACK_SIZE (1 << 20)

void ide_read(uint8_t *, uint32_t, uint32_t);
void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {	
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph, *eph;

#ifdef HAS_DEVICE_IDE
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
			uint32_t addr=ph->p_vaddr;
			memcpy((void*)(addr),(void*)((void*)elf+ph->p_offset),ph->p_filesz);
			
			if(ph->p_memsz>ph->p_filesz)
				memset((void*)(addr+ph->p_filesz),0,ph->p_memsz-ph->p_filesz);



			/*
			char *start;
			start=(char*)elf;
			start+=ph->p_offset;
			uint32_t  dest=ph->p_vaddr;
			
			for(int i=0;i<ph->p_filesz;i++)
			{
				uint32_t temp=(uint32_t)(*start);
				//paddr_write(dest,1,temp);
				memcpy((char*)dest,&temp,1);
				dest=dest+1;
			}
			
			//memcpy(dest,start,ph->p_filesz);
			//dest+=ph->p_filesz;

			for(int i=0;i<ph->p_memsz-ph->p_filesz;i++)
			{
				*(char*)(dest+i)=0;
			}
			*/

			//setmem((char*)dest,ph->p_memsz-ph->p_filesz,0);
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
	create_video_mapping();
#endif
	write_cr3(get_ucr3());
#endif
	return entry;
}
