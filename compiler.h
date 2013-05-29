#ifndef COMPILER_H
#define COMPILER_H


int isLetter(char ch);
int isNumber(char ch);

//��ȡ�ļ���С
long getFileSize(char *fileName);

//����ת�����ַ���д��token�� 
int int2charArray(int a,char * token,int width);

/*
����Ԫʽд��ʷ�����������ļ� 
token : ����ĵ��ʷ���
kind : ���ʷ��ŵ��ֱ� 

*/
int store(char * token,int kind,FILE * outFile);

//���������ļ�������Ҫ������ļ���׺���������ļ�ȫ�� 
char * getOutFileName(char * inFileName,char * outFileSuffix);

//�����Ԫʽ�ṹ 
typedef struct {int kind;
				int value;
				} Result;
				
//�ʷ����� ���������� 
Result analyse(long * readIndexPtr,char buf[],long length,FILE * outFile,int line);

//�ַ����������ַ� 
void linkChar(char * str,char ch);

//��ʾ���ͱ����ִ�����
Result identifier(char * token,FILE * outFile);
 
#endif
