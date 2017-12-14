#include "cpu/instr.h"
make_instr_func(leave)
{
	/*
	OPERAND temp_ebp;
	OPERAND temp_esp;
	temp_ebp.type=OPR_REG;
	temp_ebp.addr=REG_EBP;
	temp_ebp.data_size=32;
	operand_read(&temp_ebp);
	temp_esp.type=OPR_REG;
	temp_esp.addr=REG_ESP;
	temp_esp.data_size=32;
	temp_esp.val=temp_ebp.val;
	operand_write(&temp_esp);
	*/


	int data=laddr_read(cpu.ebp,4);
	cpu.esp=cpu.ebp;
	laddr_write(cpu.esp,4,data);
	// mov ebp to esp
	

	OPERAND temp_esp;
	temp_esp.type=OPR_MEM;
	temp_esp.addr=cpu.esp;
	temp_esp.data_size=data_size;
	operand_read(&temp_esp);
	
	/*
	OPERAND temp_ebp;
	temp_ebp.type=OPR_MEM;
	temp_ebp.addr=cpu.ebp;
	temp_ebp.data_size=32;
	temp_ebp.val=temp.val;
	*/
	cpu.ebp=temp_esp.val;
	cpu.esp+=data_size/8;
	print_asm_0("leave","",1);
	return 1;
	// pop ebp
}
