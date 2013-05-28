#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#include "compiler.h"

#define MAXLEN 1024 

//获取文件大小 
long getFileSize(char *fileName) {
	FILE * fileToEnd = fopen(fileName,"rb");
	if(fileToEnd == NULL){
		printf("open file failed!\n");
		exit(-1);
	}
	fseek(fileToEnd,0,SEEK_END);
	long fileSize = ftell(fileToEnd);
	fclose(fileToEnd);//not tested 
	return fileSize;
}

int isLetter(char ch) {
	return ((ch >= 65 && ch <= 90)||(ch >= 97 && ch <= 122));
}

int isNumber(char ch) {
	return 	(ch >= 48 && ch <= 57);
}

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


/*
将二元式写入词法分析的输出文件 
token : 输入的单词符号
kind : 单词符号的种别 

*/
int store(char * token,int kind,FILE * outFile) {
	int stringToStoreLen = 20;//一行记录的长度 
	int twoYuanShiLen = 19;//二元式的长度 
	int wordLen = 16;//二元式中单词符号的长度 
	int kindLen = 2; //单词符号的种别的长度 
	char * endOfLine = "EOLN";//行尾标记 
	char * endOfFile = "EOF";//文件尾标记 
	
	if (kind == 24) {
		token = endOfLine;
	} 
	else if (kind == 25) {
		token = endOfFile;	
	}
	
	//一行记录的最后一个符号是回车 
	char * stringToStore = (char *)calloc(stringToStoreLen,sizeof (char));//分配了20字节,记住,要注意内存写溢出的问题 
	memset(stringToStore,' ',twoYuanShiLen);//二元式的部分初始化为空格 
	
	if (outFile == NULL) {
		printf("open dydfile fail\n");
		return -1;	
	}
	int tokenLen = strlen(token);
	//输入的单词符号长度超过准许长度,则出错退出 
	if (tokenLen > wordLen) {
		printf("token is too long\n");
		return -1;	
	}
	//单词符号在二元式中开始的位置,前面填充的是空格 
	int beginPoint = wordLen - tokenLen;
	memcpy(stringToStore + beginPoint,token,tokenLen);
	
	char kindStr[2];
	int2charArray(kind,kindStr,2); 
	//种别和单词符号中间有一个空格分隔 
	memcpy(stringToStore + wordLen + 1,kindStr,kindLen);
	//加上行尾标记 
	strcat(stringToStore,"\n");
	//写入文件; 
	printf("writing: %s",stringToStore);
	fwrite(stringToStore,sizeof (char),stringToStoreLen,outFile);
	return 0;
} 

//根据输入文件名和需要输出的文件后缀名获得输出文件全名 
char * getOutFileName(char * inFileName,char * outFileSuffix) {
	int inFileNameLen = strlen(inFileName);
	char point = '.';
	char * inFileSuffix = strrchr(inFileName,point);
	int inFileSuffixLen = strlen(inFileSuffix);
	int outFileSuffixLen = strlen(outFileSuffix);
	int outFileNameLen = inFileNameLen - inFileSuffixLen + outFileSuffixLen;
	char * outFileName = (char *)calloc(outFileNameLen,sizeof (char));
	if (outFileName == NULL) {
		printf("memery alloc error!\n");
		exit(1);	
	}
	memcpy(outFileName,inFileName,inFileNameLen - inFileSuffixLen);
	outFileName = strcat(outFileName,outFileSuffix);
	return outFileName;
}

/*二元式赋值函数.	也可以这样写:result = (Result){a,b}; 要先转型 这是后来才知道的... */
void setResult(Result * result, int kind,int value) {
		result -> kind = kind;
		result -> value = value;
}

//字符串后连接字符的函数 
void linkChar(char * str,char ch) {
	char * ret = str;
	int i = 0;
	for (;str[i] != '\0';i ++){
		;
	}
	str[i++] = ch;
	str[i] = '\0';
	
} 

//标示符和保留字处理函数
Result identifier(char * token,FILE * outFile) {
	int arraySize = 9;
	char * words[9] = {"begin","end","integer","if","then","else","function","read","write"};
	int i;
	for (i = 0; i < arraySize; i ++) {
		if (strcmp(token,words[i]) == 0){
		printf("reserved word is %s, its number is %d\n",token,i + 1);
		store(token,i + 1,outFile);
		
		return (Result){i + 1,0};
		}
	}
	store(token,10,outFile);
	return (Result){10,0};
}

//词法分析主分析函数 
Result analyse(long * readIndexPtr,char buf[],long length,FILE * outFile){
	printf("readindex: %ld\n",*readIndexPtr);
	int status = 0;
	char token[16] = "";
	char ch;
	for (;(*readIndexPtr) < length;){ 
		
		ch = buf[(*readIndexPtr)++];
		
		//printf("current char: %ld\n",*readIndexPtr);
		if (ch == EOF) {
			store(token,25,outFile);
			return (Result){25,0};
		} 
		//空格
		else if (ch == 32 && status == 0) {
			status = 0;
		}
		//字母
		else if (isLetter(ch)) {
			if (status == 0 || status == 1) {
				linkChar(token,ch);
				status = 1;
				continue; 
			}
			else if (status == 3) {
				status = 4;
				(*readIndexPtr) --;
				//常数 
				store(token,11,outFile);
				return (Result){11,0};
			}
		}
		//数字
		else if (isNumber(ch)) {
			if (status == 1) {
				linkChar(token,ch);
				status = 1;
			}
			else if (status == 0 ||status == 3) {
				linkChar(token,ch);
				status = 3;
			}
		} 
		//非字母/数字 
		else {
			
			if (status == 1) {
				status = 2;
				(*readIndexPtr) --;
				//标示符/保留字的处理
				return identifier(token,outFile); 
				
			} else if (status == 3) {
				status = 4;
				(*readIndexPtr) --;
				//常数 
				store(token,11,outFile);
				return (Result){11,0};
			} else 
			switch (ch) {
				case '=':
					linkChar(token,ch);
					status = 5;
					store(token,12,outFile);
					return (Result){12,0};
					break;
				case '-':
					linkChar(token,ch);
					status = 6;
					store(token,18,outFile);
					return (Result){18,0};
					break;
				case '*':
					linkChar(token,ch);
					status = 7;
					store(token,19,outFile);
					return (Result){19,0};
					break;
				case '(':
					linkChar(token,ch);
					status = 8;
					store(token,21,outFile);
					return (Result){21,0};
					break;
				case ')':
					linkChar(token,ch);
					status = 9;
					store(token,22,outFile);
					return (Result){22,0};
					break;
				case '<':
					linkChar(token,ch);
					status = 10;
					if ((*readIndexPtr) < length) {
						ch = buf[(*readIndexPtr)++];
						switch (ch) {
							case '=':
								linkChar(token,ch);
								status = 11;
								store(token,14,outFile);
								return (Result){14,0};
								break;
							case '>':
								linkChar(token,ch);
								status = 12;
								store(token,13,outFile);
								return (Result){13,0};
								break;
							default:
								status = 13;
								(*readIndexPtr) --;
								store(token,15,outFile);
								return (Result){15,0};
								break;
						}
					}
					
					break;
				case '>':
					linkChar(token,ch);
					status = 14;
					if ((*readIndexPtr) < length) {
						ch = buf[(*readIndexPtr)++];
						switch (ch) {
							case '=':
								linkChar(token,ch);
								status = 15;
								store(token,16,outFile);
								return (Result){16,0};
								break;
							default:
								status = 16;
								(*readIndexPtr) --;
								store(token,17,outFile);
								return (Result){17,0};
								break;
							}
						}
					break;
				case ':':
					linkChar(token,ch);
					status = 17;
					if ((*readIndexPtr) < length) {
						ch = buf[(*readIndexPtr)++];
						switch (ch) {
							case '=':
								linkChar(token,ch);
								status = 18;
								store(token,20,outFile);
								return (Result){20,0};
								break;
							default:
								status = 19;
								(*readIndexPtr) --;
								printf("error: unexpected letter \"%c\"\n",ch);
								return (Result){0,0}; 
								break;
							}
						}
					break;
				case ';':
					linkChar(token,ch);
					status = 20;
					store(token,23,outFile); 
					return (Result){23,0};
					break;
					//回车 
				case '\r':
					ch = buf[(*readIndexPtr)++];
					if(ch == '\n') {
						store(token,24,outFile);
						if((*readIndexPtr) == length) {
							store(token,25,outFile);
						}
						return (Result){24,0};
					} else {
						printf("error: expect LF ,but no LF\n");
						return (Result){0,0};
					}
					break;
				default:
					printf("wrong character:\"%c\",ascii code is %d\n",ch,ch);
					status = 21;
					(*readIndexPtr) --;
					if (*readIndexPtr < 0) {
						printf("error! status 21,readIndex < 0\n");
						
					}
					return (Result){0,0};
					break;
			}
		}
		
	}
	//store(token,25,outFile);//EOF
}
