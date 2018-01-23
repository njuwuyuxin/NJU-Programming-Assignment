#include "cpu/instr.h"
make_instr_func(cli)
{
	cpu.eflags.if=0;
}
