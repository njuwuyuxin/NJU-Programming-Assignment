#include "cpu/instr.h"
static void instr_execute_1op()
{		
	operand_read(&opr_src);
	//cpu.esp+=4;
	OPERAND temp;
	/*
	temp.type=OPR_REG;
	temp.addr=REG_ESP;
	*/
	temp.type=OPR_MEM;
	temp.addr=cpu.esp;
	temp.data_size=data_size;
	operand_read(&temp);
	opr_src.val=temp.val;
	opr_src.data_size=data_size;
	operand_write(&opr_src);
	cpu.esp+=data_size/8;
}
make_instr_impl_1op(pop,r,l)

make_instr_func(popa)
{
	cpu.edi=vaddr_read(cpu.esp,2,4);
	cpu.esp+=4;
	cpu.esi=vaddr_read(cpu.esp,2,4);
	cpu.esp+=4;
	cpu.ebp=vaddr_read(cpu.esp,2,4);
	cpu.esp+=4;
	//cpu.esp=vaddr_read(cpu.esp,2,4);  throw away
	cpu.esp+=4;
	cpu.ebx=vaddr_read(cpu.esp,2,4);
	cpu.esp+=4;
	cpu.edx=vaddr_read(cpu.esp,2,4);
	cpu.esp+=4;
	cpu.ecx=vaddr_read(cpu.esp,2,4);
	cpu.esp+=4;
	cpu.eax=vaddr_read(cpu.esp,2,4);
	cpu.esp+=4;
}
