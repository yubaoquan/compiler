#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

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


void errorRecord(int line) {
	char errorHead[50] = "***LINE:";
	char * errorTail = "word error\n";
	printf("line: %d\n",line);
	char lineStr[20];
	int2charArray(line,lineStr,2);
	printf("%s\n",lineStr);
	char * temp = strcat(errorHead,lineStr);
	char * errorMsg = strcat(temp,errorTail);
	printf("%s\n",errorMsg);
	//fwrite(errorMsg,sizeof (char),strlen(errorMsg),errorfile);
} 

void CopySomething(int len)
{
	char kbuf[800];
	int size = sizeof(kbuf);
	if(len > size)
		printf("no overflow\n");
	printf("overflow\n");
}


int main () {
	char buf[80] = "123456";
	int a = 6;
	int len;
	len = printf("%s\n",buf);
	printf("%d\n",len);
	printf("%d\n",strlen(buf));
	len = sprintf(buf + strlen(buf) , "%8d%8d",123,4567);  
	printf("%s\n%d\n",buf,len);
	len = sprintf(buf + strlen(buf) , "%8d%8d", 123, 4567);
	printf("%s\n%d\n",buf,len);
	len = sprintf(buf + strlen(buf) , "%s%s%s","hello", " ", "world\n");
	printf("%s\n%d\n",buf,len);
	//CopySomething() 
	/*
	int bytes_formatted=0;
	char buffer[28]= "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	printf("%.20x%n",buffer,&bytes_formatted);
	printf("\nThe number of bytes formatted in the previous printf statement was %d\n",bytes_formatted);
*/

	system("pause");
	return 0;	
}

