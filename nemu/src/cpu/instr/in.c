#include "cpu/instr.h"
#include "device/port_io.h"
make_instr_func(IN_b)
{
	cpu.gpr[0]._8[0]=pio_read(cpu.gpr[2]._16,1);
	return 1;
}
make_instr_func(IN_l)
{
	cpu.eax=pio_read(cpu.gpr[2]._16,4);
	return 1;
}
