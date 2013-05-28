#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
using namespace std;
FILE * var;/*变量名表*/ 
FILE * pro;/*过程名表*/ 

char * process[2] = {"main","F"};
int processSelect = 0;
int varKind = 0;
const int MAX=1024;
int i,j,k;//j表示first.dyd内容的行数
int varPos = 0;//变量在变量表中位置
char numberStr[3];//数字转换成的字符串
int level = 0;//变量的层次
int proLevel = 0;//函数的层次

struct dyd//用来存储从first.dyd文件读入的内容
{
	char s[16];
	int zhongbie;//种别
}dyd[MAX];

bool arith ();//算术表达式
bool condition();//条件表达式
bool execute();//执行语句
bool direction_list();//说明语句表


//数字转换成字符串写入token中,不足宽度,前面补零. 
int int2charArray(int number,char * token,int width) {
	if (number < 0) {
		printf("error : negative number");
		return -1;	
	}
		token[width] = '\0';
		int i; 
	for (i = width;i > 0;) {
		int temp = number % 10;
		token[--i] = temp + 48;
		number /= 10;
	}
	return 0;
}

void storeVar(int index,int kind) {
	fwrite("vname: ",sizeof (char),strlen("vname: "),var);
	fwrite(dyd[index].s,sizeof (char),strlen(dyd[index].s),var);//变量名
	fwrite("\n",sizeof (char),1,var);
	
	fwrite("vpro: ",sizeof (char),strlen("vpro: "),var);
	fwrite(process[processSelect],sizeof(char),strlen(process[processSelect]),var);//所属过程
	fwrite("\n",sizeof (char),1,var);
	
	fwrite("vkind: ",sizeof (char),strlen("vkind: "),var);
	int2charArray(kind,numberStr,2);
	fwrite(numberStr,sizeof(char),strlen(numberStr),var);//分类:变量
	fwrite("\n",sizeof (char),1,var);
	
	fwrite("vtype: ",sizeof (char),strlen("vtype: "),var);
	fwrite("integer",sizeof(char),strlen("integer"),var);//变量类型:integer
	fwrite("\n",sizeof (char),1,var);
	
	
	int2charArray(level,numberStr,2);//层次
	fwrite("vlev: ",sizeof (char),strlen("vlev: "),var);
	fwrite(numberStr,sizeof(char),strlen(numberStr),var);
	fwrite("\n",sizeof (char),1,var);
	
	
	int2charArray(varPos++,numberStr,2);//变量名表中的位置
	fwrite("vadr: ",sizeof (char),strlen("vadr: "),var);
	fwrite(numberStr,sizeof(char),strlen(numberStr),var);
	fwrite("\n",sizeof (char),1,var);
	fwrite("\n",sizeof (char),1,var);
}

bool contrast(int a)//对first.dyd中的内容进行匹配对比
{
	i=1;
	while (dyd[j].zhongbie==24)
	{
		j++;
		i++;
	}
	printf("analysing line %d: %s, %d\n",j + 1,dyd[j].s,dyd[j].zhongbie);
	if(dyd[j].zhongbie!=a) {
		return 0;
	}
	j++;
	return 1;

}

bool relation()//关系运算符-><|<=|>=|=|<>
{
	for(int m=12;m!=17;m++)
		if(contrast(m)) {
		printf("relation found: %d\n",m);
			return 1;
		}
		return 0;
}

bool condition()//条件表达式->算术表达式 关系运算符 算术表达式
{	
	printf("condition analysing\n");
	if(!arith())
		return 0;
	if(!relation())
		return 0;
	if(!arith())
		return 0;
	return 1;
}


bool tjyj()//条件语句
{
	printf("tjyj analysing\n");
	
	if(!contrast(4))//contrast(4)=="if"
		return 0;
	if(!condition())
		return 0;
	if(!contrast(5))//contrast(5)=="then"
		return 0;
	if(!execute())
		return 0;
	if(!contrast(6))//contrast(6)=="else"
		return 0;
	if(!execute())
		return 0;
	return 1;
}

bool divisor_() {//因子回溯
	if (contrast(21)) {
		if(!arith()) {
			return 0;
		}
		if(!contrast(22)) {
			return 0;
		}
	}
	return 1;
}

bool divisor() {//因子
	if(contrast(11))/*常数*/
		return 1;
	if(!contrast(10)) /*变量或函数调用*/
		return 0;
	if(!divisor_())
		return 0;
	return 1;
}

bool term_() {//项递归
	if(contrast(19)) {//contrast(19)=='*'
		if(!divisor()) {
			j -= i;
			return 0;
		}
		if(!term_()) {
			j -= i;
			return 0;
		}
	}
	return 1;
}

bool term() {//项
	if(!divisor())
		return 0;
	if(!term_())
		return 0;
	return 1;
}

bool arith_() {//算术表达式递归
	if (contrast(18)) {//contrast(18)=='-'
		if (!term()) {
			j-=i;
			return 1;
		}
		if (!arith_()) {
			j-=i;
			return 1;
		}
	}
	return 1;
}

bool arith() {//算术表达式
	if(!term())
		return 0;
	if(!arith_())
		return 0;
	return 1;
}

bool assign() {//赋值运算
	if(!contrast(10))//contrast(10)为变量
		return 0;
	//非形参的变量
	storeVar(j - 1,0); 
	/*
		fwrite(dyd[j - 1].s,sizeof (char),strlen(dyd[j - 1].s),var);//变量名
		fwrite("\n",sizeof (char),1,var);
		fwrite(process[processSelect],sizeof(char),strlen(process[processSelect]),var);//所属过程
		fwrite("\n",sizeof (char),1,var);
		fwrite("0",sizeof(char),strlen("0"),var);//分类:变量
		fwrite("\n",sizeof (char),1,var);
		fwrite("integer",sizeof(char),strlen("integer"),var);//变量类型:integer
		fwrite("\n",sizeof (char),1,var);
		int2charArray(level,numberStr,2);//层次
		fwrite(numberStr,sizeof(char),strlen(numberStr),var);
		fwrite("\n",sizeof (char),1,var);
		int2charArray(varPos++,numberStr,2);//变量名表中的位置
		fwrite(numberStr,sizeof(char),strlen(numberStr),var);
		fwrite("\n",sizeof (char),1,var);
		fwrite("\n",sizeof (char),1,var);
		*/
	if(!contrast(20))//contrast(20)=":="
		return 0;
	if(!arith())
		return 0;
	return 1;
} 

bool write() {//写语句
	if (!contrast(9))//contrast(9)=="write"
		return 0;
	if (!contrast(21)) //contrast(21)='('
		return 0;
	if (!contrast(10))//contrast(10)为变量
		return 0;
	//非形参的变量
	storeVar(j - 1,0); 
	/*
		fwrite(dyd[j - 1].s,sizeof (char),strlen(dyd[j - 1].s),var);//变量名
		fwrite("\n",sizeof (char),1,var);
		fwrite(process[processSelect],sizeof(char),strlen(process[processSelect]),var);//所属过程
		fwrite("\n",sizeof (char),1,var);
		fwrite("0",sizeof(char),strlen("0"),var);//分类:变量
		fwrite("\n",sizeof (char),1,var);
		fwrite("integer",sizeof(char),strlen("integer"),var);//变量类型:integer
		fwrite("\n",sizeof (char),1,var);
		int2charArray(level,numberStr,2);//层次
		fwrite(numberStr,sizeof(char),strlen(numberStr),var);
		fwrite("\n",sizeof (char),1,var);
		int2charArray(varPos++,numberStr,2);//变量名表中的位置
		fwrite(numberStr,sizeof(char),strlen(numberStr),var);
		fwrite("\n",sizeof (char),1,var);
		fwrite("\n",sizeof (char),1,var);
		*/
	if (!contrast(22))//contrast(22)=')'
		return 0;
	return 1;
}
bool read() {//读语句
	if (!contrast(8))//contrast(8)="read"
		return 0;
	if (!contrast(21)) 
		return 0;
	if (!contrast(10))
		return 0;
	//非形参的变量
	storeVar(j - 1,0); 
	/*
		fwrite(dyd[j - 1].s,sizeof (char),strlen(dyd[j - 1].s),var);//变量名
		fwrite("\n",sizeof (char),1,var);
		fwrite(process[processSelect],sizeof(char),strlen(process[processSelect]),var);//所属过程
		fwrite("\n",sizeof (char),1,var);
		fwrite("0",sizeof(char),strlen("0"),var);//分类:变量
		fwrite("\n",sizeof (char),1,var);
		fwrite("integer",sizeof(char),strlen("integer"),var);//变量类型:integer
		fwrite("\n",sizeof (char),1,var);
		int2charArray(level,numberStr,2);//层次
		fwrite(numberStr,sizeof(char),strlen(numberStr),var);
		fwrite("\n",sizeof (char),1,var);
		int2charArray(varPos++,numberStr,2);//变量名表中的位置
		fwrite(numberStr,sizeof(char),strlen(numberStr),var);
		fwrite("\n",sizeof (char),1,var);
		fwrite("\n",sizeof (char),1,var);
		*/
	if (!contrast(22))
		return 0;
	return 1;
}
bool execute() {//执行语句
	if(read())
		return 1;
	if (write())
		return 1;
	if(assign())
		return 1;
	if(tjyj())
		return 1;
	return 0;
}

bool exe_list_() {//执行语句表递归
	if(contrast(23)) {//contrast(23)=';'
		if (!execute()) {//回溯
			j -= i;
			return 1;
		}
		if(!exe_list_()) {
			j -= i;
			return 1;
		}
	}
	return 1;
}

bool exe_list() {//执行语句表
	if(!execute())
		return 0;
	if(!exe_list_())
		return 0;
	return 1;
}

bool F() {//函数体
	processSelect = 1;
	if(!contrast(1)) {//contrast(1)="begin"
		printf("in function analysing : not begin\n");
		return 0;
	}
	if(!direction_list()) {
		printf("in function analysing : not direction_list\n");
		return 0;
	}
	if(!contrast(23)) {
		printf("in function analysing : not 23\n");
		return 0;
	}
	if(!exe_list()) {
		printf("in function analysing : not exe_list\n");
		return 0;
	}
	if(!contrast(2)) {//contrast(2)=="end"
		printf("in function analysing : not end\n");
		return 0;
	}
	processSelect = 0;
	printf("one function\n");
	return 1;
}

bool direction_() {//说明语句递归
	if(contrast(10)) {
	//非形参的变量
	storeVar(j - 1,0); 
	/*
		fwrite("vname: ",sizeof (char),strlen("vname: "),var);
		fwrite(dyd[j - 1].s,sizeof (char),strlen(dyd[j - 1].s),var);//变量名
		fwrite("\n",sizeof (char),1,var);
		
		fwrite("vpro: ",sizeof (char),strlen("vpro: "),var);
		fwrite(process[processSelect],sizeof(char),strlen(process[processSelect]),var);//所属过程
		fwrite("\n",sizeof (char),1,var);
		
		fwrite("vkind: ",sizeof (char),strlen("vkind: "),var);
		fwrite("0",sizeof(char),strlen("0"),var);//分类:变量
		fwrite("\n",sizeof (char),1,var);
		
		fwrite("vtype ",sizeof (char),strlen("vtype: "),var);
		fwrite("integer",sizeof(char),strlen("integer"),var);//变量类型:integer
		fwrite("\n",sizeof (char),1,var);
		
		
		int2charArray(level,numberStr,2);//层次
		fwrite("vlev: ",sizeof (char),strlen("vlev: "),var);
		fwrite(numberStr,sizeof(char),strlen(numberStr),var);
		fwrite("\n",sizeof (char),1,var);
		
		
		int2charArray(varPos++,numberStr,2);//变量名表中的位置
		fwrite("vadr: ",sizeof (char),strlen("vadr: "),var);
		fwrite(numberStr,sizeof(char),strlen(numberStr),var);
		fwrite("\n",sizeof (char),1,var);
		fwrite("\n",sizeof (char),1,var);
		*/
		return 1;	
	} 
	if (!contrast(7)) {
		printf("not 7\n");
		return 0;
	}
	if (!contrast(10)) {
		printf("not 10\n");
		return 0;
	}
	//函数
	fwrite("pname: ",sizeof (char),strlen("pname: "),pro);
	fwrite(dyd[j - 1].s,sizeof (char),strlen(dyd[j - 1].s),pro);
	fwrite("\n",sizeof (char),1,pro);
	
	fwrite("ptype: ",sizeof (char),strlen("ptype: "),pro);
	fwrite("integer",sizeof(char),strlen("integer"),pro);//过程类型:integer
	fwrite("\n",sizeof (char),1,pro);
	
	fwrite("plev: ",sizeof (char),strlen("plev: "),pro);
	int2charArray(proLevel,numberStr,2);//层次
	fwrite(numberStr,sizeof(char),strlen(numberStr),pro);
	fwrite("\n",sizeof (char),1,pro);
	
	fwrite("fadr: ",sizeof (char),strlen("fadr: "),pro);
	int2charArray(1,numberStr,2);//层次
	fwrite(numberStr,sizeof(char),strlen(numberStr),pro);
	fwrite("\n",sizeof (char),1,pro);
	
	fwrite("ladr: ",sizeof (char),strlen("ladr: "),pro);
	int2charArray(1,numberStr,2);//层次
	fwrite(numberStr,sizeof(char),strlen(numberStr),pro);
	fwrite("\n",sizeof (char),1,pro);
	
	fwrite("\n",sizeof (char),1,pro);
	
	if (!contrast(21)) {
		printf("not 21\n");
		return 0;
	}
	
	if (!contrast(10)) {
		printf("not 10\n");
		return 0;
	}
	
	//函数的形参
	storeVar(j - 1,1); 
	/*
	fwrite("vname: ",sizeof (char),strlen("vname: "),var);
	fwrite(dyd[j - 1].s,sizeof (char),strlen(dyd[j - 1].s),var);//变量名
	fwrite("\n",sizeof (char),1,var);
	fwrite(dyd[j - 3].s,sizeof(char),strlen(dyd[j - 3].s),var);//所属过程
	fwrite("\n",sizeof (char),1,var);
	fwrite("1",sizeof(char),strlen("1"),var);//分类:形参
	fwrite("\n",sizeof (char),1,var);
	fwrite("integer",sizeof(char),strlen("integer"),var);//变量类型:integer
	fwrite("\n",sizeof (char),1,var);
	int2charArray(level,numberStr,2);//层次
	fwrite(numberStr,sizeof(char),strlen(numberStr),var);
	fwrite("\n",sizeof (char),1,var);
	int2charArray(varPos++,numberStr,2);//变量名表中的位置
	fwrite(numberStr,sizeof(char),strlen(numberStr),var);
	fwrite("\n",sizeof (char),1,var);
	fwrite("\n",sizeof (char),1,var);
	*/
	
	if (!contrast(22)) {
		printf("not 22\n");
		return 0;
	}
	if (!contrast(23)) {
		printf("not 23\n");
		return 0;
	}
	if (!F()) {
		printf("not f\n");
		return 0;
	}
	return 1;

}
bool direction() {//说明语句
	if(!contrast(3))
		return 0;
	if(!direction_())
		return 0;
	return 1;
}

bool direction_list_() {//说明语句表递归
	if (contrast(23)) {
		if (!direction()) {
			j -= i ;
			printf("i : %d\n",i);
			return 1;
		}
		if (!direction_())	{
			j -= i ;
			printf("i : %d\n",i);
			return 1;
		}
	}
	return 1;
}

bool direction_list() {//说明语句表
	level ++;
	if(!direction()) {
		printf("not direction\n");
		return 0;
	}
	if (!direction_list_()) {
		printf("not direction_list_\n");
		return 0;
	}
	level --;
	printf("one direction list\n");
	return 1;
}

bool program() {//程序
	if (!contrast(1)) {
		printf("contrast_1\n");
		return 0;
	}
	
	if (!direction_list()) {
		printf("direction_list\n");
		return 0;
	}
	j ++; 
	if (!contrast(23)) {
		printf("contrast_23\n");
		return 0;
	}
	if (!exe_list()) {
		printf("exe_list\n");
		return 0;
	}
	if (!contrast(2)) {
		printf("contrast_2\n");
		return 0;
	}
	if (!contrast(25)) {
		printf("contrast_25\n");
		return 0;
	}
	return 1;
}

int main()
{
	FILE * in=fopen("target.dyd","r");
	var=fopen("target.var","w");
	pro=fopen("target.pro","w");
	int c=0;
	while(~fscanf(in,"%s%d",dyd[c].s,&dyd[c].zhongbie))
		c++;
	fclose(in);
	
	/*
	for(int index = 0; index < 65; index ++) {
		printf("%s, %d\n",dyd[index].s,dyd[index].zhongbie);
	} */
	
	j=0;
	
	if (program())
		cout<<"right!"<<endl;
	else
		cout<<"error! at line: " << j + 1<<endl;
	
	fclose(var);
	fclose(pro);
	
	system("PAUSE"); 
	return 0;

}

