#ifndef __TX_FILE__
#define __TX_FILE__

/*
	Resources: http://www.mrx.net/c/introduction.html
*/

#ifdef  _WIN32
# pragma warning(disable: 4786) // identifier was truncated to 255 characters in the debug information
// BUG: C4786 or C1067 Caused by Long Identifier and /Zi
// Microsoft Knowledge Base Article - 122539 
#endif

///////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <cstdarg>
 
const char LF = 0x0a; //Line Feed(10) '\n'           
const char CR = 0x0d; //Carriage Return  (13) '\r'
const char COMMENT_CHAR = '#';

 
void skipRestOfLine(FILE* fh);
void skipComments(FILE* fh);
void skipWhiteSpace(FILE* fh);
void fsync(FILE* fh);
void fgets_withoutCR(
	char* buffer,
	int n, 
	FILE* fh
);

//int findFiles(
//    char* dir,
//	char* match,
//	std::vector<std::string>& files
//);

long fcopy(
	char *dest, 
	char *source
); 
    
void getFilename(
	char* path,
	char* filename
);

char* getFilename(
	char* path 
);

void getFileDirname(
	char* path,
	char* dirname
);

void printf_fprintf( 
	const char *fname, 
	const char *fmt, 
	... 
);
///////////////////////////////////////////////////////////////////////
#endif /* __TX_FILE__ */
