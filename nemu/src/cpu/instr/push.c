#include "cpu/instr.h"

static void instr_execute_1op()
{
	operand_read(&opr_src);
	cpu.esp-=4;
	/*
	OPERAND temp;
	temp.type=OPR_MEM;
	temp.data_size=32;
	temp.addr=cpu.esp;
	temp.val=opr_src.val;
	operand_write(&temp);
	*/
	vaddr_write(cpu.esp,2,data_size/8,opr_src.val);
}

make_instr_func(pusha)
{
	uint32_t temp=cpu.esp;
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,cpu.eax);
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,cpu.ecx);
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,cpu.edx);
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,cpu.ebx);
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,temp);
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,cpu.ebp);
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,cpu.esi);
	cpu.esp-=4;
	vaddr_write(cpu.esp,2,4,cpu.edi);
	return 1;
}

make_instr_impl_1op(push,r,l)
make_instr_impl_1op(push,rm,v)
make_instr_impl_1op(push,i,b)
make_instr_impl_1op(push,i,v)
