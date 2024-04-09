#include "pslFile.h"
#ifdef  _WIN32
#pragma warning(disable: 4267) //warning C4267: 'initializing' : conversion from 'size_t' to 'int', possible loss of data
#endif
//////////////////////////////////////////////////////////////////////////////////////
#ifdef _CHK_MEM_LEAK_

	#include "pslMemLeak.h"
	#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
	#endif	
#endif
//////////////////////////////////////////////////////////////////////////////////////

void skipWhiteSpace(
	FILE* fh
) 
//------------------------------------------------------------
{ 
    int ch; 
	do { 
        ch=fgetc( fh ); 
    } while( ch==' ' || ch=='\t' || ch==CR || ch==LF );
    ungetc( ch, fh );
}
//////////////////////////////////////////////////////////////
void skipRestOfLine(
	FILE* fh
) 
//------------------------------------------------------------
{	
	int ch; 
	do { 
        ch=fgetc( fh ); 
    } while( ch!=EOF && ch!=CR && ch!=LF );
    ungetc( ch, fh );

	skipWhiteSpace(fh);
}
//////////////////////////////////////////////////////////////
void skipComments(	
	FILE* fh
) 
//------------------------------------------------------------	
{
	int ch;
    ch = getc( fh );        
	if ( ch==COMMENT_CHAR ) {
			
		skipRestOfLine(fh);                        
        skipComments(fh);
	} else {
		ungetc( ch, fh );
	}
}
//////////////////////////////////////////////////////////////	
void fsync(
	FILE* fh
)
//------------------------------------------------------------ 
{ 
  skipWhiteSpace(fh); 
  skipComments(fh); 
}

////////////////////////////////////////////////////////////////////////
void fgets_withoutCR(
	char* buffer,
	int n, 
	FILE* fh
) //---------------------------------------------------------
{
	fgets( buffer,n,fh);
	for ( unsigned int i = strlen(buffer)-1; i >strlen(buffer)-3 ; i--) { 
		if ( buffer[i] == '\n' || buffer[i] == '\r' ) 
			 buffer[i] = '\0'; 
	}
}
//////////////////////////////////////////////////////////////////////////

long fcopy(
	char *dest, 
	char *source
)
{
    FILE *d = NULL, *s = NULL;
    char *buffer;
    size_t incount;
    long totcount = 0L;
#if defined (_WIN32)
    fopen_s(&s, source, "rb");
#else
	s = fopen(source, "rb");
#endif
    if(s == NULL)
            return -1L;
#if defined (_WIN32)
    fopen_s(&d, dest, "wb");
#else
	s = fopen(source, "rb");
#endif
    if(d == NULL)
    {
            fclose(s);
            return -1L;
    }

    buffer = (char*) malloc(1024);
    if(buffer == NULL)
    {
            fclose(s);
            fclose(d);
            return -1L;
    }

    incount = fread(buffer, sizeof(char), 1024, s);

    while(!feof(s))
    {
            totcount += (long)incount;
            fwrite(buffer, sizeof(char), incount, d);
            incount = fread(buffer, sizeof(char), 1024, s);
    }

    totcount += (long)incount;
    fwrite(buffer, sizeof(char), incount, d);

    free(buffer);
    fclose(s);
    fclose(d);

    return totcount;
}
//////////////////////////////////////////////////////////////////////////
//int findFiles(
//	char* dir,
//	char* match,
//	std::vector<std::string>& files
//)
//{
//	files.clear();
//	struct _finddata_t c_file;
//    long hFile;
//
//	_chdir(dir);
//
//    /* Find first .c file in current directory */
//    if( (hFile = _findfirst( match, &c_file )) == -1L )
//       return 0;
//    else
//    {
//		files.push_back(c_file.name);
//
//        /* Find the rest of the .c files */
//        while( _findnext( hFile, &c_file ) == 0 ) {
//           files.push_back(c_file.name);
//        }
//
//       _findclose( hFile );
//   }
//	return files.size();
//}
/////////////////////////////////////////////////////////////////////
void getFilename(
	char* path,
	char* filename
){
 
	int N=strlen(path);
	int counter;
	for (counter=N-1; counter>0; counter--) {
		if (path[counter] == '\\' || path[counter] == '/') break;
	}   
#if defined (_WIN32)
	sprintf_s(filename,N-counter, "%s",&path[counter+1]);
#else
	sprintf(filename, "%s", &path[counter + 1]);
#endif
}
/////////////////////////////////////////////////////////////////////
void getFileDirname(
	char* path,
	char* dirname
){
	int N=strlen(path);
	int counter;
	for (counter=N-1; counter>0; counter--) {
		if (path[counter]=='\\') break;
	}   
	sprintf(dirname,"%s",path);	
	dirname[counter]='\0';
}
/////////////////////////////////////////////////////////////////////
char* getFilename(
	char* path
){
 
	int N=strlen(path);
	int counter;
	for (counter=N-1; counter>0; counter--) {
		if (path[counter]=='\\' || path[counter]=='/') break;
	}   
	
	return &path[counter+1];
}

//////////////////////////////////////////////////////////////////////////////
void printf_fprintf( const char *fname, const char *fmt, ... ){
    va_list arg_ptr ;
    va_start( arg_ptr, fmt ) ;

	if(fname){
		FILE* fp=fopen(fname,"a");
		vfprintf(fp,fmt,arg_ptr) ;
		fclose(fp);
	}

	vprintf(fmt,arg_ptr) ;
    va_end(arg_ptr);
     
}
 