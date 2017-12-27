#include "cpu/instr.h"

make_instr_func(jmp_near) {
        OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

	int offset = sign_ext(rel.val, data_size);
	print_asm_1("jmp", "", 2, &rel);

	cpu.eip += offset;

        return 1 + data_size / 8;
}
make_instr_func(jmp_near_b) {
        OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = 8;
        rel.addr = eip + 1;

        operand_read(&rel);

	int offset = sign_ext(rel.val, rel.data_size);
	print_asm_1("jmp", "", 2, &rel);
	cpu.eip += offset;

        return 2;
}

make_instr_func(jmp_rm_v)
{
	int len=1;
	opr_src.data_size=data_size;
	//decode_data_size_v
	//decode_operand_rm
	len+=modrm_rm(eip+1,&opr_src);
	//len+=modrm_rm(eip+1,&opr_src);
	operand_read(&opr_src);

	int offset = sign_ext(opr_src.val, data_size);
	print_asm_1("jmp", "", 7, &opr_src);
	cpu.eip = offset;
	return 0;
	//return 1+data_size/8;
}

make_instr_func(JNB_b)
{
	if(cpu.eflags.CF==0)
	{
        OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = 8;
        rel.addr = eip + 1;
        operand_read(&rel);
	int offset = sign_ext(rel.val, data_size);
	print_asm_1("jmp", "", 2, &rel);

	cpu.eip += offset;

        return 2;
	}
	else
		return 1;
}
make_instr_func(JNLE_b)
{
	if(cpu.eflags.SF==cpu.eflags.OF&&cpu.eflags.ZF==0)
	{
        OPERAND rel;
        rel.type = OPR_IMM;
	rel.sreg = SREG_CS;
        rel.data_size = 8;
        rel.addr = eip + 1;
        operand_read(&rel);
	int offset = sign_ext(rel.val, data_size);
	print_asm_1("jmp", "", 2, &rel);

	cpu.eip += offset;

        return 2;
	}
	else
		return 1;
}
make_instr_func(ljmp)
{
	uint32_t jmp_addr=paddr_read(eip+1,4);
	uint16_t seg=paddr_read(eip+5,2);
	assert(seg==0x8);
	cpu.cs.val=seg;
	load_sreg(1);
	//jmp_addr=jmp_addr+0xc0000000;
	cpu.eip=jmp_addr;
	return 0;
}
