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

int main () {
	
	char a[20] = "hello";
	char b[10] = "world";
	char * ba = strcat (a,"this fucking world");
	ba = strcat(ba,"mama");
	//strcat(a,b);
	printf("%s\n",ba);
	//errorRecord(6); 
	system("pause");
	return 0;	
}
