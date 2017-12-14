#include "cpu/cpu.h"
uint32_t test_bit(uint32_t  number,uint32_t bit)
{
	if(bit>=0&&bit<=31)
	{
		uint32_t test_number=1<<bit;
		uint32_t result_number=number&test_number;
		return (result_number>>bit)%2; //if 1 return 1; if 0 return 0;
	}
	else
	{
		printf("bit is overflow");
		return 2;
	}
}
uint32_t alu_add(uint32_t src, uint32_t dest) {
	//printf("\e[0;31mPlease implement add at alu.c\e[0m\n");
	uint32_t result=src+dest;
	//printf("result=%d\n",result);
	uint32_t SF= test_bit(result,31);
	//printf("SF=%d\n",SF);
	//set CF
	if(result<src&&result<dest)
		cpu.eflags.CF=1;
	else
		cpu.eflags.CF=0;
	//set PF
	uint32_t sum_of_1=0;
	for(int i=0;i<8;i++)
	{
		sum_of_1+=test_bit(result,i);
	}
	if(sum_of_1%2==0)
		cpu.eflags.PF=1;
	else
		cpu.eflags.PF=0;

	//set SF
	if(SF==1)
		cpu.eflags.SF=1;
	else
		cpu.eflags.SF=0;
	//set ZF
	if(result==0)
		cpu.eflags.ZF=1;
	else
		cpu.eflags.ZF=0;	
	//set OF
	uint32_t src_SF=test_bit(src,31);
	uint32_t dest_SF=test_bit(dest,31);
	if(src_SF==dest_SF&&src_SF!=cpu.eflags.SF)
		cpu.eflags.OF=1;
	else
		cpu.eflags.OF=0;
	//assert(0);
	return result;
}

uint32_t alu_adc(uint32_t src, uint32_t dest) {
	//printf("\e[0;31mPlease implement adc at alu.c\e[0m\n");
	uint32_t result=src+dest+cpu.eflags.CF;
	uint32_t SF= test_bit(result,31);
	//set CF
	if(result<src&&result<dest)
		cpu.eflags.CF=1;
	else if((src==0xFFFFFFFF||dest==0xFFFFFFFF)&&cpu.eflags.CF==1)
		cpu.eflags.CF=1;
	else
		cpu.eflags.CF=0;
	//set PF
	uint32_t sum_of_1=0;
	for(int i=0;i<8;i++)
	{
		sum_of_1+=test_bit(result,i);
	}
	if(sum_of_1%2==0)
		cpu.eflags.PF=1;
	else
		cpu.eflags.PF=0;

	//set SF
	if(SF==1)
		cpu.eflags.SF=1;
	else
		cpu.eflags.SF=0;
	//set ZF
	if(result==0)
		cpu.eflags.ZF=1;
	else
		cpu.eflags.ZF=0;	
	//set OF
	uint32_t src_SF=test_bit(src,31);
	uint32_t dest_SF=test_bit(dest,31);
	if(src_SF==dest_SF&&src_SF!=cpu.eflags.SF)
		cpu.eflags.OF=1;
	else
		cpu.eflags.OF=0;
	//assert(0);
	return result;
}


uint32_t alu_sub(uint32_t src, uint32_t dest) {
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//printf("src=%u\n",src);
	uint32_t new_src=src*(-1);
	//printf("dest=%u\n",dest);
	//printf("New_src=%u\n",src);
	uint32_t result = dest+new_src;
	//set CF
	if(result>dest)
		cpu.eflags.CF=1;
	else
		cpu.eflags.CF=0;
	//set PF
	uint32_t sum_of_1=0;
	for(int i=0;i<8;i++)
	{
		sum_of_1+=test_bit(result,i);
	}
	if(sum_of_1%2==0)
		cpu.eflags.PF=1;
	else
		cpu.eflags.PF=0;

	
	//set SF
	uint32_t SF=test_bit(result,31);
	if(SF==1)
		cpu.eflags.SF=1;
	else
		cpu.eflags.SF=0;
	//set ZF
	if(result==0)
		cpu.eflags.ZF=1;
	else
		cpu.eflags.ZF=0;	
	//set OF
	uint32_t src_SF=test_bit(src,31);
	uint32_t dest_SF=test_bit(dest,31);
	//printf("src_SF=%d\n",src_SF);
	//printf("dest_SF=%d\n",dest_SF);
	if((src_SF==SF)&&(src_SF!=dest_SF))
		cpu.eflags.OF=1;
	else
		cpu.eflags.OF=0;
	//assert(0);
	//printf("OF=%d\n",cpu.eflags.OF);
	return result;
}

uint32_t alu_sbb(uint32_t src, uint32_t dest) {
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//printf("src=%u\n",src);
	uint32_t new_src=src*(-1);
	//printf("dest=%u\n",dest);
	//printf("New_src=%u\n",src);
	uint32_t result = dest+new_src-cpu.eflags.CF;
	//set CF
	if(result>dest)
		cpu.eflags.CF=1;
	else if(src==0xFFFFFFFF&&cpu.eflags.CF==1)
		cpu.eflags.CF=1;
	else
		cpu.eflags.CF=0;
	//set PF
	uint32_t sum_of_1=0;
	for(int i=0;i<8;i++)
	{
		sum_of_1+=test_bit(result,i);
	}
	if(sum_of_1%2==0)
		cpu.eflags.PF=1;
	else
		cpu.eflags.PF=0;

	
	//set SF
	uint32_t SF=test_bit(result,31);
	if(SF==1)
		cpu.eflags.SF=1;
	else
		cpu.eflags.SF=0;
	//set ZF
	if(result==0)
		cpu.eflags.ZF=1;
	else
		cpu.eflags.ZF=0;	
	//set OF
	uint32_t src_SF=test_bit(src,31);
	uint32_t dest_SF=test_bit(dest,31);
	//printf("src_SF=%d\n",src_SF);
	//printf("dest_SF=%d\n",dest_SF);
	if((src_SF==SF)&&(src_SF!=dest_SF))
		cpu.eflags.OF=1;
	else
		cpu.eflags.OF=0;
	//assert(0);
	//printf("OF=%d\n",cpu.eflags.OF);
	return result;
}


uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size) {
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//assert(0);
	
	uint64_t result=0;
	if(data_size!=32)
	{
		uint32_t test_bit_string=1;
		for(int i=0;i<data_size-1;i++)
		{
			test_bit_string=test_bit_string<<1;
			test_bit_string=test_bit_string+1;
		}
		uint32_t temp_src=test_bit_string&src;
		uint32_t temp_dest=test_bit_string&dest;
		result=temp_src*temp_dest;
		if((result>>data_size)!=0)
			cpu.eflags.CF=cpu.eflags.OF=1;
		else
			cpu.eflags.CF=cpu.eflags.OF=0;
	}
	
	//printf("data_size=%d\n",data_size);
	if(data_size==32)
	{
		//printf("src=%x,dest=%x\n",src,dest);
		result=(uint64_t)src*dest;
		//printf("my_result=%llu\n",result);
		//uint64_t test_length=1;
		//test_length=test_length<<32;
		uint64_t test_result=result>>32;
		if(test_result!=0)
		{
			cpu.eflags.CF=1;
			cpu.eflags.OF=1;
		}
		else
		{
			cpu.eflags.CF=0;
			cpu.eflags.OF=0;
		}
	}
	//set PF
	uint32_t sum_of_1=0;
	for(int i=0;i<8;i++)
	{
		sum_of_1+=test_bit(result,i);
	}
	if(sum_of_1%2==0)
		cpu.eflags.PF=1;
	else
		cpu.eflags.PF=0;

	return result;
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size) {
	return (uint64_t)src*dest;
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	/*
	uint64_t result=0;
	if(data_size!=32)
	{
		uint32_t test_bit_string=1;
		for(int i=0;i<data_size-1;i++)
		{
			test_bit_string=test_bit_string<<1;
			test_bit_string=test_bit_string+1;
		}
		uint32_t temp_src=test_bit_string&src;
		uint32_t temp_dest=test_bit_string&dest;
		result=temp_src*temp_dest;
		if((result>>data_size)!=0)
			cpu.eflags.CF=cpu.eflags.OF=1;
		else
			cpu.eflags.CF=cpu.eflags.OF=0;
	}
	
	//printf("data_size=%d\n",data_size);
	if(data_size==32)
	{
		//printf("src=%x,dest=%x\n",src,dest);
		result=(uint64_t)src*dest;
		//printf("my_result=%llu\n",result);
		//uint64_t test_length=1;
		//test_length=test_length<<32;
		uint64_t test_result=result>>32;
		if(test_result!=0)
		{
			cpu.eflags.CF=1;
			cpu.eflags.OF=1;
		}
		else
		{
			cpu.eflags.CF=0;
			cpu.eflags.OF=0;
		}
	}
	//set PF
	uint32_t sum_of_1=0;
	for(int i=0;i<8;i++)
	{
		sum_of_1+=test_bit(result,i);
	}
	if(sum_of_1%2==0)
		cpu.eflags.PF=1;
	else
		cpu.eflags.PF=0;

	return result;
	*/
}

uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size) {
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//assert(0);
	uint32_t result=0;
	if(src!=0)
		result=dest/src;
	else
	{
		printf("error!src=0");
		return 0;
	}
	return result;
}

int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size) {
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//assert(0);
	return dest/src;
}

uint32_t alu_mod(uint64_t src, uint64_t dest) {
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//assert(0);
	uint32_t result=0;
	if(src!=0)
		result=dest%src;
	else
	{
		printf("error!src=0");
		return 0;
	}
	return result;
}

int32_t alu_imod(int64_t src, int64_t dest) {
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	//assert(0);
	return dest%src;
}

uint32_t alu_and(uint32_t src, uint32_t dest) {
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	uint32_t result=src&dest;
	cpu.eflags.CF=0;
	cpu.eflags.OF=0;
	uint32_t SF=test_bit(result,31);
	cpu.eflags.SF=SF;
	if(result==0)
		cpu.eflags.ZF=1;
	else
		cpu.eflags.ZF=0;
	//set PF
	uint32_t sum_of_1=0;
	for(int i=0;i<8;i++)
	{
		sum_of_1+=test_bit(result,i);
	}
	if(sum_of_1%2==0)
		cpu.eflags.PF=1;
	else
		cpu.eflags.PF=0;
	return result;

	//assert(0);
	//return 0;
}

uint32_t alu_xor(uint32_t src, uint32_t dest) {
	uint32_t result=src^dest;
	cpu.eflags.CF=0;
	cpu.eflags.OF=0;
	uint32_t SF=test_bit(result,31);
	cpu.eflags.SF=SF;
	if(result==0)
		cpu.eflags.ZF=1;
	else
		cpu.eflags.ZF=0;
	//set PF
	uint32_t sum_of_1=0;
	for(int i=0;i<8;i++)
	{
		sum_of_1+=test_bit(result,i);
	}
	if(sum_of_1%2==0)
		cpu.eflags.PF=1;
	else
		cpu.eflags.PF=0;
	return result;
}
uint32_t alu_or(uint32_t src, uint32_t dest) {
	uint32_t result=src|dest;
	cpu.eflags.CF=0;
	cpu.eflags.OF=0;
	uint32_t SF=test_bit(result,31);
	cpu.eflags.SF=SF;
	if(result==0)
		cpu.eflags.ZF=1;
	else
		cpu.eflags.ZF=0;
	//set PF
	uint32_t sum_of_1=0;
	for(int i=0;i<8;i++)
	{
		sum_of_1+=test_bit(result,i);
	}
	if(sum_of_1%2==0)
		cpu.eflags.PF=1;
	else
		cpu.eflags.PF=0;
	return result;
}


uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size) {
	//printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	uint32_t test_bit_string=1;
	if(data_size==0)
		printf("data_size is too short!");
	for(int i=1;i<data_size;i++)
	{
		test_bit_string=test_bit_string<<1;
		test_bit_string=test_bit_string+1;
	}
	//printf("src=%x\n",src);
	//printf("dest=%x\n",dest);
	//printf("data_size=%d\n",data_size);
	//printf("result=%x\n",result);
	
	uint32_t new_dest=dest&test_bit_string;
	//generate a string of bit to get new src_length(data_size)
	uint32_t temp_result=new_dest;
	//printf("temp_result=%x\n",temp_result);
	uint32_t result=0;
	if(data_size==32)
	{
		result=temp_result;
		if(src!=0)
			result=result<<(src-1);
		else
			result=result;
		uint32_t CF=test_bit(result,31);
		if(CF==1)
			cpu.eflags.CF=1;
		else
			cpu.eflags.CF=0;
		if(src!=0)
			result=result<<1;
		else
			result=result;
		uint32_t SF=test_bit(result,31);
		if(SF==1)
			cpu.eflags.SF=1;
		else
			cpu.eflags.SF=0;
	}
	else
	{
		dest=dest>>data_size;
		dest=dest<<data_size;
		//clear low data_size bits
		temp_result=temp_result<<src;
		uint32_t CF=test_bit(temp_result,data_size);
		if(CF==1)
			cpu.eflags.CF=1;
		else
			cpu.eflags.CF=0;
		//printf("CF=%d\n",cpu.eflags.CF);
		uint32_t SF=test_bit(temp_result,data_size-1);
		if(SF==1)
			cpu.eflags.SF=1;
		else
			cpu.eflags.SF=0;
		
		//clear high 32-data_size bits
		temp_result=temp_result<<(32-data_size);
		temp_result=temp_result>>(32-data_size);
		
		result=dest+temp_result;
	}
	//set PF
	uint32_t sum_of_1=0;
	for(int i=0;i<8;i++)
	{
		sum_of_1+=test_bit(result,i);
	}
	if(sum_of_1%2==0)
		cpu.eflags.PF=1;
	else
		cpu.eflags.PF=0;
	//set ZF
	if(data_size<32)
	{
	uint32_t mol=1;
	for(int i=0;i<data_size;i++)
	{
		mol=mol*2;
	}
	if(result%mol==0)
		cpu.eflags.ZF=1;
	else
		cpu.eflags.ZF=0;	
	}
	else
	{
		if(result==0)
			cpu.eflags.ZF=1;
		else
			cpu.eflags.ZF=0;
	}
	return result;
	}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t test_bit_string=1;
	for(int i=1;i<data_size;i++)
	{
		test_bit_string=test_bit_string<<1;
		test_bit_string=test_bit_string+1;
	}
	uint32_t temp_result=dest&test_bit_string;
	uint32_t result=0;

	//set CF
	temp_result=temp_result>>(src-1);
	uint32_t CF=test_bit(temp_result,0);
	if(CF==1)
		cpu.eflags.CF=1;
	else
		cpu.eflags.CF=0;
	temp_result=temp_result>>1;

	//set SF
	uint32_t SF=test_bit(result,data_size-1);
	if(SF==1)
		cpu.eflags.SF=1;
	else
		cpu.eflags.SF=0;

	if(data_size==32)
		result=temp_result;
	else
	{
		dest=dest>>data_size;
		dest=dest<<data_size;
		result=dest+temp_result;
	}
	//set PF
	uint32_t sum_of_1=0;
	for(int i=0;i<8;i++)
	{
		sum_of_1+=test_bit(result,i);
	}
	if(sum_of_1%2==0)
		cpu.eflags.PF=1;
	else
		cpu.eflags.PF=0;
	//set ZF
	if(data_size<32)
	{
	uint32_t mol=1;
	for(int i=0;i<data_size;i++)
	{
		mol=mol*2;
	}
	if(result%mol==0)
		cpu.eflags.ZF=1;
	else
		cpu.eflags.ZF=0;	
	}
	else
	{
		if(result==0)
			cpu.eflags.ZF=1;
		else
			cpu.eflags.ZF=0;
	}
	return result;

}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size) {
	//printf("you see me\n");
	uint32_t test_bit_string=1;
	for(int i=1;i<data_size;i++)
	{
		test_bit_string=test_bit_string<<1;
		test_bit_string=test_bit_string+1;
	}
	uint32_t temp_result=dest&test_bit_string;
	uint32_t result=0;

	//set CF
	//temp_result=temp_result>>(src-1);
	for(int i=0;i<src-1;i++)
	{
		uint32_t temp_SF=test_bit(temp_result,data_size-1);
		temp_result=temp_result>>1;
		if(temp_SF==1)
		{
			uint32_t symbol_bit=1;
			for(int j=0;j<data_size-1;j++)
			{
				symbol_bit=symbol_bit<<1;
			}
			temp_result=temp_result+symbol_bit;
		}
	}
	uint32_t CF=test_bit(temp_result,0);
	if(CF==1)
		cpu.eflags.CF=1;
	else
		cpu.eflags.CF=0;

	uint32_t temp_SF=test_bit(temp_result,data_size-1);
	temp_result=temp_result>>1;
	if(temp_SF==1)
		{
			uint32_t symbol_bit=1;
			for(int j=0;j<data_size-1;j++)
			{
				symbol_bit=symbol_bit<<1;
			}
			temp_result=temp_result+symbol_bit;
		}


	//set SF
	uint32_t SF=test_bit(temp_result,data_size-1);
	if(SF==1)
		cpu.eflags.SF=1;
	else
		cpu.eflags.SF=0;

	if(data_size==32)
		result=temp_result;
	else
	{
		dest=dest>>data_size;
		dest=dest<<data_size;
		result=dest+temp_result;
	}
	//set PF
	uint32_t sum_of_1=0;
	for(int i=0;i<8;i++)
	{
		sum_of_1+=test_bit(result,i);
	}
	if(sum_of_1%2==0)
		cpu.eflags.PF=1;
	else
		cpu.eflags.PF=0;
	//set ZF
	if(data_size<32)
	{
	uint32_t mol=1;
	for(int i=0;i<data_size;i++)
	{
		mol=mol*2;
	}
	if(result%mol==0)
		cpu.eflags.ZF=1;
	else
		cpu.eflags.ZF=0;	
	}
	else
	{
		if(result==0)
			cpu.eflags.ZF=1;
		else
			cpu.eflags.ZF=0;
	}
	return result;

}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size) {
	uint32_t test_bit_string=1;
	if(data_size==0)
		printf("data_size is too short!");
	for(int i=1;i<data_size;i++)
	{
		test_bit_string=test_bit_string<<1;
		test_bit_string=test_bit_string+1;
	}
	uint32_t new_dest=dest&test_bit_string;
	//generate a string of bit to get new src_length(data_size)
	uint32_t temp_result=new_dest;
	uint32_t result=0;
	if(data_size==32)
	{
		result=temp_result;
		result=result<<(src-1);
		uint32_t CF=test_bit(result,31);
		if(CF==1)
			cpu.eflags.CF=1;
		else
			cpu.eflags.CF=0;
		result=result<<1;
		uint32_t SF=test_bit(result,31);
		if(SF==1)
			cpu.eflags.SF=1;
		else
			cpu.eflags.SF=0;
	}
	else
	{
		dest=dest>>data_size;
		dest=dest<<data_size;
		//clear low data_size bits
		temp_result=temp_result<<src;
		uint32_t CF=test_bit(temp_result,data_size);
		if(CF==1)
			cpu.eflags.CF=1;
		else
			cpu.eflags.CF=0;
		uint32_t SF=test_bit(temp_result,data_size-1);
		if(SF==1)
			cpu.eflags.SF=1;
		else
			cpu.eflags.SF=0;
		
		//clear high 32-data_size bits
		temp_result=temp_result<<(32-data_size);
		temp_result=temp_result>>(32-data_size);
		
		result=dest+temp_result;
	}
	//set PF
	uint32_t sum_of_1=0;
	for(int i=0;i<8;i++)
	{
		sum_of_1+=test_bit(result,i);
	}
	if(sum_of_1%2==0)
		cpu.eflags.PF=1;
	else
		cpu.eflags.PF=0;
	//set ZF
	if(data_size<32)
	{
	uint32_t mol=1;
	for(int i=0;i<data_size;i++)
	{
		mol=mol*2;
	}
	if(result%mol==0)
		cpu.eflags.ZF=1;
	else
		cpu.eflags.ZF=0;	
	}
	else
	{
		if(result==0)
			cpu.eflags.ZF=1;
		else
			cpu.eflags.ZF=0;
	}
	return result;
}	
