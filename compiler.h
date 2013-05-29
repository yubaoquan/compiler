#ifndef COMPILER_H
#define COMPILER_H


int isLetter(char ch);
int isNumber(char ch);

//获取文件大小
long getFileSize(char *fileName);

//数字转换成字符串写入token中 
int int2charArray(int a,char * token,int width);

/*
将二元式写入词法分析的输出文件 
token : 输入的单词符号
kind : 单词符号的种别 

*/
int store(char * token,int kind,FILE * outFile);

//根据输入文件名和需要输出的文件后缀名获得输出文件全名 
char * getOutFileName(char * inFileName,char * outFileSuffix);

//定义二元式结构 
typedef struct {int kind;
				int value;
				} Result;
				
//词法分析 主分析函数 
Result analyse(long * readIndexPtr,char buf[],long length,FILE * outFile,int line);

//字符串后连接字符 
void linkChar(char * str,char ch);

//标示符和保留字处理函数
Result identifier(char * token,FILE * outFile);
 
#endif
