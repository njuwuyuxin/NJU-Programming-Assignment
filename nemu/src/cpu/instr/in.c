#include "cpu/instr.h"

make_instr_func(IN_b)
{
	cpu.eax._8[1]=pio_read(cpu.edx._16,1);
	return 1;
}
