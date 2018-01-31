#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include "memory/cache.h"
uint32_t Hit_times=0;
uint32_t Miss_times=0;
double Accuracy=0;

extern uint32_t hw_mem_read(paddr_t paddr,size_t len);
extern void hw_mem_write(paddr_t paddr,size_t len,uint32_t data);
void init_cache()
{
for(int i=0;i<1024;i=i+1)
	{
		L1_dcache[i].valid_bit=0;
	}
}
uint32_t cache_read(paddr_t paddr,size_t len)
{
	uint32_t sign=paddr&0xffffe000;
	sign=sign>>13;
	uint32_t group_No=paddr&0x00001fc0;
	group_No=group_No>>6;
	uint32_t block_addr=paddr&0x0000003f;
	bool if_hit=false;
	uint32_t my_result=0;
	for(int i=0;i<8;i=i+1)
	{
		if(L1_dcache[group_No*8+i].sign==sign)
		{
			if(L1_dcache[group_No*8+i].valid_bit==1)
			{
				Hit_times+=1;
				//printf("Hit!!!\n");
				//printf("correct addr=%u\n",paddr);
				//printf("sign=%u,group_No=%u,block_addr=%u\n",sign,group_No,block_addr);
				if_hit=true;
				if(len==1)
					my_result= L1_dcache[group_No*8+i].content[block_addr];
				if(len==2)
					my_result= *(uint16_t*)(L1_dcache[group_No*8+i].content+block_addr);
				if(len==4)
					my_result= *(uint32_t*)(L1_dcache[group_No*8+i].content+block_addr);
				uint32_t correct_result=hw_mem_read(paddr,len);
				if(correct_result!=my_result)
				{
					//printf("len=%u\n",len);
					//printf("correct addr=%x\n",paddr);
					//printf("sign=%x,group_No=%x,block_addr=%x\n",sign,group_No,block_addr);
					uint32_t my_addr=0;
					my_addr+=sign;
					my_addr=my_addr<<7;
					my_addr+=group_No;
					my_addr=my_addr<<6;
					my_addr+=block_addr;
					my_result=correct_result;
					//printf("my_addr=%x\n",my_addr);
					//printf("correct result is:%x\n",correct_result);
					//printf("my_result is:%x\n\n",my_result);
					//if(paddr==0x3199c)
					//{
						//for(int j=0;j<4;j+=1)
						//{
							//uint8_t t=hw_mem_read(paddr+j,1);
							//t=t+1;
							//printf("0x%x=%x\t",paddr+j,t);
							//printf("content[i]=%x\n",L1_dcache[group_No*8+i].content[block_addr+j]);
						//}
					//}
				}
				return my_result;
				break;
			}
		}
	}
	if(if_hit==false)
	{
		Miss_times+=1;
		//printf("Not Hit!\n");
		//printf("paddr=%x\n",paddr);
		int No=rand()%8;
		paddr_t mem_address=paddr-block_addr;
		for(int i=0;i<64;i=i+1)
		{
			uint8_t temp=hw_mem_read(mem_address+i,1);
			L1_dcache[group_No*8+No].content[i]=temp;
			assert(temp==L1_dcache[group_No*8+No].content[i]);
		}
		//uint32_t sign=paddr&0xffffe000;
		//sign=sign>>13;
		L1_dcache[group_No*8+No].sign=sign;
		L1_dcache[group_No*8+No].valid_bit=1;
					//if(group_No==0x7a)
					//{
						//printf("first miss!\n");
						//printf("paddr=%x\n",paddr);
						//for(int j=0;j<64;j+=1)
						//{
							//uint8_t t=hw_mem_read(paddr+j,1);
							//printf("0x%x=%x\t",paddr+j,t);
							//printf("content[0x%x]=%x\n",block_addr+j,L1_dcache[group_No*8+No].content[j]);
						//}
					//}
		
		return hw_mem_read(paddr,len);
	}
	return hw_mem_read(paddr,len);
}

void cache_write(paddr_t paddr,size_t len,uint32_t data)
{
	hw_mem_write(paddr,len,data);
	uint32_t sign=paddr&0xffffe000;
	sign=sign>>13;
	uint32_t group_No=paddr&0x00001fc0;
	group_No=group_No>>6;
	uint32_t block_addr=paddr&0x0000003f;
	//bool if_hit=false;
	for(int i=0;i<8;i=i+1)
	{
		if(L1_dcache[group_No*8+i].sign==sign)
		{
			if(L1_dcache[group_No*8+i].valid_bit==1)
			{
				//printf("Write Hit!!!\n");
				//printf("correct addr=%x\n",paddr);
				//printf("sign=%x,group_No=%x,block_addr=%x\n",sign,group_No,block_addr);
				//if_hit=true;
				if(len==1)
				{
					L1_dcache[group_No*8+i].content[block_addr]=data;
				}
				if(len==2)
				{
					for(int j=0;j<2;j+=1)
					{
						uint8_t t=hw_mem_read(paddr+j,1);
						L1_dcache[group_No*8+i].content[block_addr+j]=t;
					}
				}
				if(len==4)
				{
					for(int j=0;j<4;j+=1)
					{
						uint8_t t=hw_mem_read(paddr+j,1);
						L1_dcache[group_No*8+i].content[block_addr+j]=t;
					}
				}
				//if(block_addr==0x3d)
				//{
					//printf("found 31ebd\n");
						//for(int j=0;j<4;j+=1)
						//{
							//uint8_t t=hw_mem_read(paddr+j,1);
							//t=t+1;
							//printf("len=%u\n",len);
							//printf("0x%x=%x\t",paddr+j,t);
							//printf("content[0x%x]=%x\n",block_addr+j,L1_dcache[group_No*8+i].content[block_addr+j]);
						//}
				//}
			}
		}
	}
}
