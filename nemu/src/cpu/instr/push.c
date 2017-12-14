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

make_instr_impl_1op(push,r,l)
make_instr_impl_1op(push,rm,v)
make_instr_impl_1op(push,i,b)
make_instr_impl_1op(push,i,v)
