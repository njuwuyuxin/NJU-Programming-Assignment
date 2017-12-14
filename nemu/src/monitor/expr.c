#include "nemu.h"
#include "cpu/reg.h"
#include "memory/memory.h"
#include "elf.h"
#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
extern CPU_STATE cpu;
extern uint32_t look_up_symtab(char* ,bool*);
enum {
	NOTYPE = 256, EQ,Dec,Hex,Dbl,eax,edx,ecx,ebx,ebp,esp,esi,edi,DeRef,NEG,NE,BE,GE,SYM
	/* TODO: Add more token types */

};

int check_parentheses(int p,int q);
uint32_t eval(int p,int q,bool* success);
int char2int(char ch);
int hex2dec(char* str,int len);

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// white space
	{"\\(",'('},
	{"\\)",')'},
	{"\\+", '+'},
	{"\\-",'-'},
	{"\\*",'*'},
	{"\\/",'/'},
	{"[0][xX][0-9a-fA-F]+",Hex},
	//{"[0-9]+\\.[0-9]+",Dbl},
	{"[0-9]+",Dec},
	{"\\$[eE][aA][xX]",eax},
	{"\\$[eE][dD][xX]",edx},
	{"\\$[eE][cC][xX]",ecx},
	{"\\$[eE][bB][xX]",ebx},
	{"\\$[eE][bB][pP]",ebp},
	{"\\$[eE][sS][pP]",esp},
	{"\\$[eE][sS][iI]",esi},
	{"\\$[eE][dD][iI]",edi},
	{"==", EQ},
	{"!=",NE},
	{"<=",BE},
	{">=",GE},
	{"\\!",'!'},
	{"<",'<'},
	{">",'>'},
	{"[0-9a-zA-Z_]+",SYM}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

typedef struct Operator_pos
{
	int type;
	int position;
	int prefer;
} operator_pos;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				//printf("match regex[%d] at position %d with len %d: %.*s\n", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */

				if(rules[i].token_type==NOTYPE)
					continue;
				//printf("%s\n",tokens[nr_token].str);
				switch(rules[i].token_type) {
					
					default: {tokens[nr_token].type = rules[i].token_type; strncpy(tokens[nr_token].str,substr_start,substr_len);nr_token++;}
							 //nr_token ++;
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	
	//ReAnalyse tokens
	for(int i=0;i<nr_token;i++)
	{
		if(tokens[i].type=='*')
		{
			if(i==0)
				tokens[i].type=DeRef;
			else
			{
				//printf("tokens[j].type=%d\n",tokens[j].type);
				if(tokens[i-1].type!=Dec&&tokens[i-1].type!=Hex&&tokens[i-1].type!=Dbl&&tokens[i-1].type!=')'&&tokens[i-1].type!=eax&&tokens[i-1].type!=edx&&tokens[i-1].type!=ecx&&tokens[i-1].type!=ebx&&tokens[i-1].type!=ebp&&tokens[i-1].type!=esp&&tokens[i-1].type!=esi&&tokens[i-1].type!=edi)
					tokens[i].type=DeRef;
			}
		}
		if(tokens[i].type=='-')
		{
			if(i==0)
				tokens[i].type=NEG;
			else
			{
				//printf("tokens[j].type=%d\n",tokens[j].type);
				if(tokens[i-1].type!=Dec&&tokens[i-1].type!=Hex&&tokens[i-1].type!=Dbl&&tokens[i-1].type!=')'&&tokens[i-1].type!=eax&&tokens[i-1].type!=edx&&tokens[i-1].type!=ecx&&tokens[i-1].type!=ebx&&tokens[i-1].type!=ebp&&tokens[i-1].type!=esp&&tokens[i-1].type!=esi&&tokens[i-1].type!=edi)
					tokens[i].type=NEG;
			}
		}

	}
	

	//Test Tokens[]
	/*
	for(int i=0;i<nr_token;i++)
	{
		printf("%s ",tokens[i].str);
	}
	printf("\n");
	for(int i=0;i<nr_token;i++)
	{
		printf("%d ",tokens[i].type);
	}
	printf("\n");
	*/
	//int test=check_parentheses(0,nr_token);
	//printf("check_parentheses=%d",test);
	return true; 
}

uint32_t expr(char *e, bool *success) {
	*success=true;
	for(int i=0;i<32;i++)
	{
		tokens[i].type=0;
		for(int j=0;j<32;j++)
			tokens[i].str[j]='\0';
	}
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	//printf("\nPlease implement expr at expr.c\n");
	//assert(0);
	int result=eval(0,nr_token-1,success);
	//*success=true;
	return result;
}
int check_parentheses(int p,int q)
{
	int flag=0;
	int left_num=0;
	if(tokens[p].type!='('||tokens[q-1].type!=')')
	{
		for(int i=p;i<q;i=i+1)
		{
			if(left_num<0)
				return -1;
			if(tokens[i].type=='(')
				left_num=left_num+1;
			if(tokens[i].type==')')
				left_num=left_num-1;
		}
		if(left_num!=0)
			return -1;
	}
	else
	{
		int temp_num=0;
		int temp_flag=0;
		for(int i=p+1;i<q-1;i=i+1)
		{
			if(temp_num<0)
				temp_flag=-1;
			if(tokens[i].type=='(')
				temp_num=temp_num+1;
			if(tokens[i].type==')')
				temp_num=temp_num-1;
		}
		if(temp_num!=0)
			temp_flag=-1;

		if(temp_flag==0)
			return 1;
		else
		{
			flag=0;
			for(int i=p;i<q;i=i+1)
			{

				if(left_num<0)
					return -1;
				if(tokens[i].type=='(')
					left_num=left_num+1;
				if(tokens[i].type==')')
					left_num=left_num-1;
			}
			if(left_num!=0)
				return -1;
			

		}
	}
	return flag;
}

uint32_t eval(int p,int q,bool* success) {
	//printf("P=%d,Q=%d\n",p,q);
	if(p > q) {
	printf("invalid expression:1\n");
	assert(0);
	/* Bad expression */
	}
	else if(p == q) {
		/* Single token.
		* For now this token should be a number.
		* Return the value of the number.
		*/
			if(tokens[p].type==SYM)
			{
				bool ss;
				int temp=look_up_symtab(tokens[p].str,&ss);
				if(ss==false)
				{
					//printf("can't find symbol %s\n",tokens[p].str);
					//assert(0);
					*success=false;
					return 0;
				}
				else
					return temp;
			}
			if(tokens[p].type==Dec)
			{
				int temp=atoi(tokens[p].str);
				return temp;
			}
			if(tokens[p].type==Hex)
			{
				int temp=hex2dec(tokens[p].str,strlen(tokens[p].str));
				if(temp==-1)
				{
					//printf("invalid HEX number");
					//assert(0);
					*success=false;
					return 0;
				}
				else
					return temp;
			}
			if(tokens[p].type==eax)
				return cpu.eax;
			if(tokens[p].type==edx)
				return cpu.edx;
			if(tokens[p].type==ecx)
				return cpu.ecx;
			if(tokens[p].type==ebx)
				return cpu.ebx;
			if(tokens[p].type==ebp)
				return cpu.ebp;
			if(tokens[p].type==esp)
				return cpu.esp;
			if(tokens[p].type==esi)
				return cpu.esi;
			if(tokens[p].type==edi)
				return cpu.edi;
		}
		//p<q
		else if(check_parentheses(p, q+1) == true) {
			/* The expression is surrounded by a matched pair of parentheses.
			* If that is the case, just throw away the parentheses.
			*/
			return eval(p + 1, q - 1,success);
		}
		else {

			if(check_parentheses(p,q+1)==-1)
			{
				//printf("parentheses not match\n");
				//assert(0);
				*success=false;
				return 0;
			}

			int op=0;
			uint32_t val1,val2,val3;
			operator_pos operators[32];
			int op_count=0;
			int left_number=0;
			for(int i=p;i<=q;i=i+1)
			{
				if(tokens[i].type=='(')
					left_number=left_number+1;
				if(tokens[i].type==')')
					left_number=left_number-1;
				//printf("left_number=%d,tokens[i].type=%d,i=%d\n",left_number,tokens[i].type,i);
				if(left_number==0)
				{
					//printf("Found Me :0");
					if(tokens[i].type=='+'||tokens[i].type=='-'||tokens[i].type=='*'||tokens[i].type=='/'||tokens[i].type==EQ||tokens[i].type==NE||tokens[i].type==BE||tokens[i].type==GE||tokens[i].type=='<'||tokens[i].type=='>')
					{
					//printf("Found Me :1");
					//printf("Now: i=%d,tokens[i].type=%d\n",i,tokens[i].type);
					operators[op_count].type=tokens[i].type;
					operators[op_count].position=i;
					if(tokens[i].type=='+'||tokens[i].type=='-')
						operators[op_count].prefer=1;
					if(tokens[i].type=='*'||tokens[i].type=='/')
						operators[op_count].prefer=2;
					if(tokens[i].type==EQ||tokens[i].type==NE||tokens[i].type==BE||tokens[i].type==GE||tokens[i].type=='<'||tokens[i].type=='>')
						operators[op_count].prefer=0;
					op_count=op_count+1;
					}
				}
				//printf("op_count=%d\n",op_count);
						
			}//find all of the operators in the expression and put down their positions

			if(op_count>0)  //the expression can be departed to two parts
			{
				int temp_prefer=10;
				for(int i=0;i<op_count;i++)
				{
						//printf("Found Me :2");
					if(operators[i].prefer<temp_prefer)
					{
						temp_prefer=operators[i].prefer;
						op=operators[i].position;
					}
				}
				//printf("OP POSITION=%d\n",op);
				//op = the position of dominant operator in the token expression;
				val1 = eval(p, op - 1,success);
				val2 = eval(op + 1, q,success);
				switch(tokens[op].type) {
				case '+': return val1 + val2;
				case '-': return val1 - val2;
				case '*': return val1 * val2;
				case '/': return val1 / val2;
				case EQ: return val1 == val2;
				case NE: return val1 != val2;
				case GE: return val1 >= val2;
				case BE: return val1 <= val2;
				case '<': return val1 < val2;
				case '>': return val1 / val2;
				default: {*success=false; return 0;};
				}
			}
			else  //the expression can't be departed , so we should calculate it
			{
				if(tokens[p].type==DeRef)
				{
					val3=eval(p+1,q,success);
					vaddr_t addr=val3;
					return vaddr_read(addr,SREG_DS,4);
					
				}
				else if(tokens[p].type==NEG)
				{
					return -eval(p+1,q,success);
				}
				else if(tokens[p].type=='!')
				{
					val3=eval(p+1,q,success);
					if(val3==0)
						return 1;
					else
						return 0;
				}
				else
				{
					//printf("invalid expression:2\n");
					//assert(0);
					*success=false;
					return 0;
				}
			}
		}
	
		return 0;
}
int char2int(char ch)
{
	if(ch>='0'&&ch<='9')
		return ch-48;
	else if(ch>='a'&&ch<='f')
		return ch-87;
	else if(ch>='A'&&ch<='F')
		return ch-55;
	else
		return -1;
}

int hex2dec(char* str,int len)
{
	int sum=0;
	for(int i=2;i<len;i++)
	{
		if(char2int(str[i])==-1)
			return -1;
		sum=sum*16;
		sum=sum+char2int(str[i]);
	}
	return sum;

}
