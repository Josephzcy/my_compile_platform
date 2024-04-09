#include "pslError.h"
//////////////////////////////////////////////////////////////////////////////////////
 

void exitOnError(
	int isError,
	char* functionName,
	char* message
){
	if(isError==1){
		printf("%s() error!\n", functionName);
		printf("\t %s\n", message);
		exit(0);
	}
}