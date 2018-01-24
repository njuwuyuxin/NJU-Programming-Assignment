#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"


void raise_intr(uint8_t intr_no) {
#ifdef IA32_INTR
	//printf("Please implement raise_intr()");
	//assert(0);
	
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,cpu.eflags.val);
	cpu.esp-=2;
	vaddr_write(cpu.esp,2,2,cpu.cs.val);
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,cpu.eip);


	printf("idtr=%x\n",cpu.idtr.base);
	uint32_t base_addr=cpu.idtr.base;
	uint32_t final_addr=base_addr+intr_no*8;
	GateDesc Desc;
	memcpy((void*)(&Desc),(void*)(final_addr),8);
#endif
}

void raise_sw_intr(uint8_t intr_no) {
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
