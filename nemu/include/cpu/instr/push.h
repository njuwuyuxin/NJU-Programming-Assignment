#ifndef INSTR_PUSH_H
#define INSTR_PUSH_H

#include "cpu/instr.h"

make_instr_func(push_r_l);
make_instr_func(push_rm_v);
make_instr_func(push_i_b);
make_instr_func(push_i_v);
make_instr_func(pusha);
#endif
