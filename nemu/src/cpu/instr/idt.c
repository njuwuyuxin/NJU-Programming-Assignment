#include "cpu/instr.h"
make_instr_func(lidt)
{
	int len=1;
	len+=modrm_rm(eip+1,&opr_src);
	//printf("addr=%x\t",opr_src.addr);
	//printf("val=%x\t",opr_src.val);
	opr_src.data_size=16;
	operand_read(&opr_src);
	cpu.idtr.limit=opr_src.val;
	opr_src.addr=opr_src.addr+2;
	opr_src.data_size=32;
	operand_read(&opr_src);
	cpu.idtr.base=opr_src.val;
	//printf("limit=%x\t",cpu.gdtr.limit);
	//printf("base=%x\t",cpu.gdtr.base);
	return len;
	//printf()

}
