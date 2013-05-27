#include <stdio.h>
#include <stdlib.h>

#include "compiler.cpp"

int main(int argc, char *argv[]) {
	 
    if( argc < 2 ){ 
        printf("usage: %s %s\n", argv[0], "infile outfile"); 
        exit(1); 
    } 
    
    char * inFileName = argv[1];//看着argv[1]别扭,所以赋值给inFileName 
    char * outFileSuffix = ".dyd";
    char * outFileName = getOutFileName(inFileName,outFileSuffix);
   
    //打开文件的操作 
    FILE *infile,*outfile; 
    infile = fopen(inFileName, "rb"); 
    outfile = fopen(outFileName, "ab" ); 
	long inFileSize = getFileSize(inFileName);
    char buf[inFileSize]; 
	int line = 0;//行号 
    if( infile == NULL){ 
        printf("%s not exists\n",inFileName); 
        exit(1); 
    }
    if( infile == NULL){ 
        printf("open %s failed\n",outFileName); 
        exit(1); 
    }
    int rc = 0;
	int totalRead = 0;
	int status = 0; 
	while ((rc = fread(buf,sizeof(unsigned char),inFileSize,infile)) != 0) {
		totalRead += rc;
	}
	if (totalRead != inFileSize) {
		printf("read error, not enouth\n");
		exit(1);
	}
	printf("read succeed!\n");
	//return 0;
    long readIndex = 0;
    Result result;
	while(readIndex != inFileSize) {
		long temp = readIndex;
		
		result = analyse(&readIndex,buf,inFileSize,outfile);
		if (result.kind == 24) {
			line ++;
		} 
		if (result.kind == 0) {
			printf("analysing error: line %d\n",line);
			return 1;
		}
		if (temp == readIndex) {
			printf("error died loop");
			fclose(infile); 
			fclose(outfile);
			return 1;
		}
	}
	fclose(infile); 
	fclose(outfile);
	system("PAUSE");
	getch();
	return 0; 
} 

