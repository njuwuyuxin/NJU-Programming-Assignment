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
