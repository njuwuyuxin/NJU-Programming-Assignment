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
	OPERAND temp_op;
	temp_op.type=OPR_REG;
	temp_op.addr=REG_EIP;
	temp_op.data_size=32;
	operand_read(&temp_op);
	*/
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
	print_asm_0("call", "", 10);

	cpu.eip += offset;
	/*
	OPERAND test_esp;
	test_esp.type=OPR_MEM;
	test_esp.addr=cpu.esp;
	test_esp.data_size=32;
	operand_read(&test_esp);
	printf("(esp)=%x  ",test_esp.val);
        */
	return 1 + data_size / 8;
}



