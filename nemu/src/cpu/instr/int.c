#include "cpu/instr.h"

make_instr_func(INT)
{
	uint8_t intr_no = paddr_read(eip+1,1);
	raise_instr(instr_no);
	return 2;
}
