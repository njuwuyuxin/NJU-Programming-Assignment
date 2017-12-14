#include "cpu/instr.h"

make_instr_func(lgdt)
{
	int len=1;
	len+=modrm_rm(eip+1,&opr_src);
	//printf("addr=%x\t",opr_src.addr);
	//printf("val=%x\t",opr_src.val);
	opr_src.data_size=16;
	operand_read(&opr_src);
	cpu.gdtr.limit=opr_src.val;
	opr_src.addr=opr_src.addr+2;
	opr_src.data_size=32;
	operand_read(&opr_src);
	cpu.gdtr.base=opr_src.val;
	//printf("limit=%x\t",cpu.gdtr.limit);
	//printf("base=%x\t",cpu.gdtr.base);
	return len;
	//printf()

}

/*
make_instr_func(lgdt)
{
	uint8_t limit=paddr_read(eip+1,1);
	//printf("eip=%u\t",eip);
	printf("limit=%x\t",limit);
	uint32_t base=paddr_read(eip+2,4);
	printf("base=%x\t",base);
	cpu.gdtr.limit=limit;
	cpu.gdtr.base=base;
	printf("eip=%x\t",eip);
	return 6;
}*/

