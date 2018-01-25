#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"


void raise_intr(uint8_t intr_no) {
#ifdef IA32_INTR
	//printf("Please implement raise_intr()");
	//assert(0);
	
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,cpu.eflags.val);
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,cpu.cs.val);
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,cpu.eip);
	cpu.eflags.IF=0;
	cpu.eflags.TF=0;

	//printf("idtr=%x\n",cpu.idtr.base);
	uint32_t base_addr=cpu.idtr.base;
	uint32_t final_addr=base_addr+intr_no*8;
	GateDesc Desc;
	Desc.val[0]=laddr_read(final_addr,4);
	Desc.val[1]=laddr_read(final_addr+4,4);
	//printf("Desc.offset=%x\n",Desc.offset_15_0);
	uint32_t offset=Desc.offset_31_16;
	offset=offset<<16;
	offset+=Desc.offset_15_0;
	//printf("Desc.offset=%x\n",offset);
	
	cpu.cs.val=Desc.selector;
	cpu.eip=offset;
	
#endif
}

void raise_sw_intr(uint8_t intr_no) {
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
