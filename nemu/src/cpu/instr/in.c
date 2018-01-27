#include "cpu/instr.h"
#include "device/port_io.h"
make_instr_func(IN_b)
{
	cpu.gpr[0]._8[1]=pio_read(cpu.gpr[2]._16,1);
	printf("eip=%x\n",cpu.eip);
	return 1;
}
