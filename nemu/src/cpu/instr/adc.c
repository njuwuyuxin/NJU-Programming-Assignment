#include "cpu/instr.h"
#include "cpu/alu.h"
static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_src.val=sign_ext(opr_src.val,opr_src.data_size);
	opr_dest.val=alu_adc(opr_src.val,opr_dest.val);
	operand_write(&opr_dest);
}

make_instr_impl_2op(adc, i, r, b)
make_instr_impl_2op(adc,r,rm,b)
make_instr_impl_2op(adc,i,rm,v)
make_instr_impl_2op(adc,r,rm,v)
make_instr_impl_2op(adc,rm,r,b)
make_instr_impl_2op(adc,rm,r,v)
make_instr_impl_2op(adc,i,r,v)
make_instr_impl_2op(adc,i,rm,bv)
make_instr_impl_2op(adc,i,a,b)
make_instr_impl_2op(adc,i,a,v)


/*
make_instr_func(adc_i2eax_v)
{
	OPERAND imm;
	imm.type=OPR_IMM;
	imm.data_size=data_size;
	imm.addr=eip+1;
	operand_read(&imm);

	OPERAND temp_eax;
	temp_eax.type=OPR_REG;
	temp_eax.data_size=32;
	temp_eax.addr=REG_EAX;
	temp_eax.val=alu_adc(imm.val,temp_eax.val);
	operand_write(&temp_eax);
	print_asm_2("adc","",5,&imm,&temp_eax);
	return 1+data_size/8;
}

make_instr_func(adc_i2al_b)
{
	OPERAND imm;
	imm.type=OPR_IMM;
	imm.data_size=8;
	imm.addr=eip+1;
	operand_read(&imm);

	OPERAND temp_eax;
	temp_eax.type=OPR_REG;
	temp_eax.data_size=8;
	temp_eax.addr=REG_EAX;
	temp_eax.val=alu_adc(imm.val,temp_eax.val);
	operand_write(&temp_eax);
	print_asm_2("adc","",5,&imm,&temp_eax);
	return 2;
}
*/
