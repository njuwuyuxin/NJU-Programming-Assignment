#include "cpu/instr.h"
#include <stdio.h>
make_instr_func(ret)
{
	OPERAND temp_eip;
	temp_eip.type=OPR_MEM;
	temp_eip.addr=cpu.esp;
	temp_eip.data_size=data_size;
	operand_read(&temp_eip);
	cpu.eip=temp_eip.val;
	if(data_size==16)
	{
		cpu.eip=cpu.eip&0x0000ffff;
		cpu.esp+=2;
	}
	else
	{
	cpu.esp+=4;
	}
	print_asm_0("ret","",2);
	return 0;
}

make_instr_func(ret_i_w)
{
	//int len=1;
	
	OPERAND iw;
	iw.type=OPR_IMM;
	iw.sreg=SREG_CS;
	iw.addr=eip+1;
	iw.data_size=16;
	operand_read(&iw);
	
	//decode_data_size_w
	//decode_operand_i



	OPERAND temp_eip;
	temp_eip.type=OPR_MEM;
	temp_eip.addr=cpu.esp;
	temp_eip.data_size=data_size;
	operand_read(&temp_eip);
	cpu.eip=temp_eip.val;
	if(data_size==16)
	{
		cpu.eip=cpu.eip&0x0000ffff;
		cpu.esp+=2;
	}
	else
	{
		cpu.esp+=4;
	}
	//printf("eip=%x",eip);
	//cpu.esp+=4;
	cpu.esp+=iw.val;
	print_asm_0("retw","",3);
	return 0;
}
