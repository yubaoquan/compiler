#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
using namespace std;
FILE * var;/*��������*/ 
FILE * pro;/*��������*/ 

char * process[2] = {"main","F"};
int processSelect = 0;
int varKind = 0;
const int MAX=1024;
int i,j,k;//j��ʾfirst.dyd���ݵ�����
int varPos = 0;//�����ڱ�������λ��
char numberStr[3];//����ת���ɵ��ַ���
int level = 0;//�����Ĳ��
int proLevel = 0;//�����Ĳ��

struct dyd//�����洢��first.dyd�ļ����������
{
	char s[16];
	int zhongbie;//�ֱ�
}dyd[MAX];

bool arith ();//�������ʽ
bool condition();//�������ʽ
bool execute();//ִ�����
bool direction_list();//˵������


//����ת�����ַ���д��token��,������,ǰ�油��. 
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
	fwrite(dyd[index].s,sizeof (char),strlen(dyd[index].s),var);//������
	fwrite("\n",sizeof (char),1,var);
	
	fwrite("vpro: ",sizeof (char),strlen("vpro: "),var);
	fwrite(process[processSelect],sizeof(char),strlen(process[processSelect]),var);//��������
	fwrite("\n",sizeof (char),1,var);
	
	fwrite("vkind: ",sizeof (char),strlen("vkind: "),var);
	int2charArray(kind,numberStr,2);
	fwrite(numberStr,sizeof(char),strlen(numberStr),var);//����:����
	fwrite("\n",sizeof (char),1,var);
	
	fwrite("vtype: ",sizeof (char),strlen("vtype: "),var);
	fwrite("integer",sizeof(char),strlen("integer"),var);//��������:integer
	fwrite("\n",sizeof (char),1,var);
	
	
	int2charArray(level,numberStr,2);//���
	fwrite("vlev: ",sizeof (char),strlen("vlev: "),var);
	fwrite(numberStr,sizeof(char),strlen(numberStr),var);
	fwrite("\n",sizeof (char),1,var);
	
	
	int2charArray(varPos++,numberStr,2);//���������е�λ��
	fwrite("vadr: ",sizeof (char),strlen("vadr: "),var);
	fwrite(numberStr,sizeof(char),strlen(numberStr),var);
	fwrite("\n",sizeof (char),1,var);
	fwrite("\n",sizeof (char),1,var);
}

bool contrast(int a)//��first.dyd�е����ݽ���ƥ��Ա�
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

bool relation()//��ϵ�����-><|<=|>=|=|<>
{
	for(int m=12;m!=17;m++)
		if(contrast(m)) {
		printf("relation found: %d\n",m);
			return 1;
		}
		return 0;
}

bool condition()//�������ʽ->�������ʽ ��ϵ����� �������ʽ
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


bool tjyj()//�������
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

bool divisor_() {//���ӻ���
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

bool divisor() {//����
	if(contrast(11))/*����*/
		return 1;
	if(!contrast(10)) /*������������*/
		return 0;
	if(!divisor_())
		return 0;
	return 1;
}

bool term_() {//��ݹ�
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

bool term() {//��
	if(!divisor())
		return 0;
	if(!term_())
		return 0;
	return 1;
}

bool arith_() {//�������ʽ�ݹ�
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

bool arith() {//�������ʽ
	if(!term())
		return 0;
	if(!arith_())
		return 0;
	return 1;
}

bool assign() {//��ֵ����
	if(!contrast(10))//contrast(10)Ϊ����
		return 0;
	//���βεı���
	storeVar(j - 1,0); 
	/*
		fwrite(dyd[j - 1].s,sizeof (char),strlen(dyd[j - 1].s),var);//������
		fwrite("\n",sizeof (char),1,var);
		fwrite(process[processSelect],sizeof(char),strlen(process[processSelect]),var);//��������
		fwrite("\n",sizeof (char),1,var);
		fwrite("0",sizeof(char),strlen("0"),var);//����:����
		fwrite("\n",sizeof (char),1,var);
		fwrite("integer",sizeof(char),strlen("integer"),var);//��������:integer
		fwrite("\n",sizeof (char),1,var);
		int2charArray(level,numberStr,2);//���
		fwrite(numberStr,sizeof(char),strlen(numberStr),var);
		fwrite("\n",sizeof (char),1,var);
		int2charArray(varPos++,numberStr,2);//���������е�λ��
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

bool write() {//д���
	if (!contrast(9))//contrast(9)=="write"
		return 0;
	if (!contrast(21)) //contrast(21)='('
		return 0;
	if (!contrast(10))//contrast(10)Ϊ����
		return 0;
	//���βεı���
	storeVar(j - 1,0); 
	/*
		fwrite(dyd[j - 1].s,sizeof (char),strlen(dyd[j - 1].s),var);//������
		fwrite("\n",sizeof (char),1,var);
		fwrite(process[processSelect],sizeof(char),strlen(process[processSelect]),var);//��������
		fwrite("\n",sizeof (char),1,var);
		fwrite("0",sizeof(char),strlen("0"),var);//����:����
		fwrite("\n",sizeof (char),1,var);
		fwrite("integer",sizeof(char),strlen("integer"),var);//��������:integer
		fwrite("\n",sizeof (char),1,var);
		int2charArray(level,numberStr,2);//���
		fwrite(numberStr,sizeof(char),strlen(numberStr),var);
		fwrite("\n",sizeof (char),1,var);
		int2charArray(varPos++,numberStr,2);//���������е�λ��
		fwrite(numberStr,sizeof(char),strlen(numberStr),var);
		fwrite("\n",sizeof (char),1,var);
		fwrite("\n",sizeof (char),1,var);
		*/
	if (!contrast(22))//contrast(22)=')'
		return 0;
	return 1;
}
bool read() {//�����
	if (!contrast(8))//contrast(8)="read"
		return 0;
	if (!contrast(21)) 
		return 0;
	if (!contrast(10))
		return 0;
	//���βεı���
	storeVar(j - 1,0); 
	/*
		fwrite(dyd[j - 1].s,sizeof (char),strlen(dyd[j - 1].s),var);//������
		fwrite("\n",sizeof (char),1,var);
		fwrite(process[processSelect],sizeof(char),strlen(process[processSelect]),var);//��������
		fwrite("\n",sizeof (char),1,var);
		fwrite("0",sizeof(char),strlen("0"),var);//����:����
		fwrite("\n",sizeof (char),1,var);
		fwrite("integer",sizeof(char),strlen("integer"),var);//��������:integer
		fwrite("\n",sizeof (char),1,var);
		int2charArray(level,numberStr,2);//���
		fwrite(numberStr,sizeof(char),strlen(numberStr),var);
		fwrite("\n",sizeof (char),1,var);
		int2charArray(varPos++,numberStr,2);//���������е�λ��
		fwrite(numberStr,sizeof(char),strlen(numberStr),var);
		fwrite("\n",sizeof (char),1,var);
		fwrite("\n",sizeof (char),1,var);
		*/
	if (!contrast(22))
		return 0;
	return 1;
}
bool execute() {//ִ�����
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

bool exe_list_() {//ִ������ݹ�
	if(contrast(23)) {//contrast(23)=';'
		if (!execute()) {//����
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

bool exe_list() {//ִ������
	if(!execute())
		return 0;
	if(!exe_list_())
		return 0;
	return 1;
}

bool F() {//������
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

bool direction_() {//˵�����ݹ�
	if(contrast(10)) {
	//���βεı���
	storeVar(j - 1,0); 
	/*
		fwrite("vname: ",sizeof (char),strlen("vname: "),var);
		fwrite(dyd[j - 1].s,sizeof (char),strlen(dyd[j - 1].s),var);//������
		fwrite("\n",sizeof (char),1,var);
		
		fwrite("vpro: ",sizeof (char),strlen("vpro: "),var);
		fwrite(process[processSelect],sizeof(char),strlen(process[processSelect]),var);//��������
		fwrite("\n",sizeof (char),1,var);
		
		fwrite("vkind: ",sizeof (char),strlen("vkind: "),var);
		fwrite("0",sizeof(char),strlen("0"),var);//����:����
		fwrite("\n",sizeof (char),1,var);
		
		fwrite("vtype ",sizeof (char),strlen("vtype: "),var);
		fwrite("integer",sizeof(char),strlen("integer"),var);//��������:integer
		fwrite("\n",sizeof (char),1,var);
		
		
		int2charArray(level,numberStr,2);//���
		fwrite("vlev: ",sizeof (char),strlen("vlev: "),var);
		fwrite(numberStr,sizeof(char),strlen(numberStr),var);
		fwrite("\n",sizeof (char),1,var);
		
		
		int2charArray(varPos++,numberStr,2);//���������е�λ��
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
	//����
	fwrite("pname: ",sizeof (char),strlen("pname: "),pro);
	fwrite(dyd[j - 1].s,sizeof (char),strlen(dyd[j - 1].s),pro);
	fwrite("\n",sizeof (char),1,pro);
	
	fwrite("ptype: ",sizeof (char),strlen("ptype: "),pro);
	fwrite("integer",sizeof(char),strlen("integer"),pro);//��������:integer
	fwrite("\n",sizeof (char),1,pro);
	
	fwrite("plev: ",sizeof (char),strlen("plev: "),pro);
	int2charArray(proLevel,numberStr,2);//���
	fwrite(numberStr,sizeof(char),strlen(numberStr),pro);
	fwrite("\n",sizeof (char),1,pro);
	
	fwrite("fadr: ",sizeof (char),strlen("fadr: "),pro);
	int2charArray(1,numberStr,2);//���
	fwrite(numberStr,sizeof(char),strlen(numberStr),pro);
	fwrite("\n",sizeof (char),1,pro);
	
	fwrite("ladr: ",sizeof (char),strlen("ladr: "),pro);
	int2charArray(1,numberStr,2);//���
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
	
	//�������β�
	storeVar(j - 1,1); 
	/*
	fwrite("vname: ",sizeof (char),strlen("vname: "),var);
	fwrite(dyd[j - 1].s,sizeof (char),strlen(dyd[j - 1].s),var);//������
	fwrite("\n",sizeof (char),1,var);
	fwrite(dyd[j - 3].s,sizeof(char),strlen(dyd[j - 3].s),var);//��������
	fwrite("\n",sizeof (char),1,var);
	fwrite("1",sizeof(char),strlen("1"),var);//����:�β�
	fwrite("\n",sizeof (char),1,var);
	fwrite("integer",sizeof(char),strlen("integer"),var);//��������:integer
	fwrite("\n",sizeof (char),1,var);
	int2charArray(level,numberStr,2);//���
	fwrite(numberStr,sizeof(char),strlen(numberStr),var);
	fwrite("\n",sizeof (char),1,var);
	int2charArray(varPos++,numberStr,2);//���������е�λ��
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
bool direction() {//˵�����
	if(!contrast(3))
		return 0;
	if(!direction_())
		return 0;
	return 1;
}

bool direction_list_() {//˵������ݹ�
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

bool direction_list() {//˵������
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

bool program() {//����
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

