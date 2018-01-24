#include "cpu/instr.h"
#include "cpu/intr.h"

make_instr_func(INT)
{
	uint8_t intr_no = paddr_read(eip+1,1);
	raise_intr(intr_no);
	return 2;
}
