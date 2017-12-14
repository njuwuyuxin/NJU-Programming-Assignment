#include "cpu/instr.h"
make_instr_func(clts)
{
	cpu.cr0.ts=0;
	return 2;
}
