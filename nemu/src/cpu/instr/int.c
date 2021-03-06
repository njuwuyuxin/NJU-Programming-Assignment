#include "cpu/instr.h"
#include "cpu/intr.h"

make_instr_func(INT)
{
	uint8_t intr_no = instr_fetch(eip+1,1);
	cpu.eip+=2;
	raise_intr(intr_no);
	print_asm_0("int"," ",2);
	return 0;
}
