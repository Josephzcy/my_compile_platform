#include "pslString.h"

#ifdef  _WIN32
#pragma warning(disable: 4267) //warning C4267: 'initializing' : conversion from 'size_t' to 'int', possible loss of data
#endif
///////////////////////////////////////////////////////////////

void  trimR( 
	char* string
) 
{
	int N=strlen(string);
	int n;
	for (n=N-1;n>=0; n--){
		if (string[n]!=' ') break;
	}
	string[n+1]='\0';
}

////////////////////////////////////////////////////////////////////
char* getFilenameFromPath(
	char* path
){
	int L=strlen(path);
	while(path[L] != '\\') L--;

	return &path[L+1];
}

////////////////////////////////////////////////////////////////////
void trimCR(char* string){
	for ( unsigned int i = strlen(string)-1; i >strlen(string)-3 ; i--) 
		if ( string[i] == '\n' || string[i] == '\r' )  string[i] = '\0'; 
};