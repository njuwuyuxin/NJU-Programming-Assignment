#include "cpu/instr.h"
#include <stdio.h>

make_instr_func(call_rm_v)
{
	int len=1;
	decode_data_size_v
	decode_operand_rm
	operand_read(&opr_src);

	cpu.esp-=4;
	OPERAND temp;
	temp.type=OPR_MEM;
	temp.addr=cpu.esp;
	temp.data_size=32;
	temp.val=eip+len;
	operand_write(&temp);

	int offset=sign_ext(opr_src.val,opr_src.data_size);
	cpu.eip=offset;
	return 0;
}
make_instr_func(call)
{
	/*
	cpu.esp-=4;
	OPERAND temp;
	temp.type=OPR_MEM;
	temp.addr=cpu.esp;
	temp.data_size=32;
	temp.val=eip+5;
	operand_write(&temp);
	
	
	
        OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

	int offset = sign_ext(rel.val, data_size);
	print_asm_0("call", "", 1+data_size/8);

	cpu.eip += offset;
	return 1 + data_size / 8;
	*/

	OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

	operand_read(&rel);
	cpu.esp-=data_size/8;
	uint32_t data=cpu.eip+1+data_size/8;
	vaddr_write(cpu.esp,SREG_SS,data_size/8,data);

	int offset=sign_ext(rel.val,data_size);
	print_asm_1("call", "", 1+data_size/8,&rel);
	cpu.eip+=offset;
	return 1+data_size/8;

}



