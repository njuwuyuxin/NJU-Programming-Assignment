#include "cpu/instr.h"

static void instr_execute_2op() {
	operand_read(&opr_src);
	operand_read(&opr_dest);
	if(opr_src.data_size==8){
		if(opr_dest.data_size==8)
			opr_dest.val&=opr_src.val;
		else{
			if((opr_src.val>>7)==1){
			uint8_t temp_dest=opr_dest.val&0xff;
			temp_dest&=opr_src.val;
			opr_dest.val &= 0xffffff00;
			opr_dest.val+=temp_dest;
			}
			else
				opr_dest.val&=opr_src.val;
		}
	}
	else
	opr_dest.val &= opr_src.val;
	operand_write(&opr_dest);

	cpu.eflags.OF=0;
	cpu.eflags.CF=0;
	
	if(opr_dest.val==0)
		cpu.eflags.ZF=1;
	else
		cpu.eflags.ZF=0;

	int result=opr_dest.val;
	int a[8];
	a[0]=result%2;
	result=result/2;
	a[1]=result%2;
	result=result/2;
	a[2]=result%2;
	result=result/2;
	a[3]=result%2;
	result=result/2;
	a[4]=result%2;
	result=result/2;
	a[5]=result%2;
	result=result/2;
	a[6]=result%2;
	result=result/2;
	a[7]=result%2;
	int count=0;
	for(int i=0;i<8;i++){
		if(a[i]==1)
			count++;
	}
	if(count%2==0)
		cpu.eflags.PF=1;
	else
		cpu.eflags.PF=0;

	cpu.eflags.SF=sign(opr_dest.val);
}

make_instr_impl_2op(and, i, rm, v)
make_instr_impl_2op(and, i, rm, b)
make_instr_impl_2op(and, i, rm, bv)
make_instr_impl_2op(and, r, rm, b)
make_instr_impl_2op(and, r, rm, v)
make_instr_impl_2op(and, rm,r, b)
make_instr_impl_2op(and, rm,r, v)
make_instr_impl_2op(and, i,a, b)
make_instr_impl_2op(and, i,a, v)
