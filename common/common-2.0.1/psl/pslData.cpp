
#include "pslData.h"
#ifdef  _WIN32
#pragma warning (disable: 4267 )//warning C4267: 'argument' : conversion from 'size_t' to 'long', possible loss of data
#pragma warning (disable: 4005 )
#endif
//////////////////////////////////////////////////////////////////////////////////////
#ifdef _CHK_MEM_LEAK_

    #include "..\..\Common\pslMemLeak.h"
    #ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
    #endif
#endif

#if defined (_WIN32)
#define _CRT_SECURE_NO_DEPRECATE
#endif
//////////////////////////////////////////////////////////////////////////////////////
void readDataRow(
    char* filename,
    void* &data,
    size_t elemSize,
    int noOfCols,
    int rowNo,
    bool isAllocateMem
)
{
    int size;
    FILE * stream;
#if defined (_WIN32)
    (fopen_s(&stream, filename, "rb"));
#else
    stream = fopen(filename, "rb");
#endif
    if (stream == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
#if defined (_WIN32)
    fscanf_s(stream,"%u ",&size);
#else
    fscanf(stream, "%u ", &size);
#endif
    int ch;
    do { ch=fgetc( stream ); }
    while( ch!=EOF && ch!=0x0a && ch!=0x0a); //CR(0x0a), LF(0x0d)
    //ungetc( ch, stream );

    if (isAllocateMem)  data =(float*)malloc(size*elemSize );
    fseek(stream,rowNo*noOfCols*elemSize, SEEK_CUR);
    fread(data,elemSize,noOfCols,stream);
    fclose(stream);

}
////////////////////////////////////////////////////////////////////////////
void writeData(
    void* data,
    int size,
    size_t elemSize,
    char* filename
)
{
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "wb");
#else
    stream = fopen(filename, "wb");
#endif
    if (stream == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    fprintf(stream,"%u #Size\n", size);
    fwrite( data,elemSize,size,stream);
    fclose(stream);

}
///////////////////////////////////////////////////////////////////////////////
void writeDataToTextFile32i(
    int* data,
    int size1,  //noOfRows
    int size2,  //noOfColums
    char* filename
)
{
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "w");
#else
    stream = fopen(filename, "w");
#endif
    if (stream  == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0;i<size1; i++){
        for(int j=0;j<size2; j++){
            //fprintf(stream,"%0.7lf\t", data[i*size2+j]);
            fprintf(stream,"%5d\t", data[i*size2+j]);
        }
        fprintf(stream,"\n");
    }
    fclose(stream);

}
///////////////////////////////////////////////////////////////////////////////
void writeDataToTextFile32ul(
    unsigned long* data,
    int size1,  //noOfRows
    int size2,  //noOfColums
    char* filename
)
{
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "w");
#else
    stream = fopen(filename, "w");
#endif
    if (stream == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0;i<size1; i++){
        for(int j=0;j<size2; j++){
            //fprintf(stream,"%0.7lf\t", data[i*size2+j]);
            // old version with compilation warning: fprintf(stream,"%d\t", data[i*size2+j]);
            fprintf(stream,"%lu\t", data[i*size2+j]);
        }
        fprintf(stream,"\n");
    }
    fclose(stream);

}
///////////////////////////////////////////////////////////////////////////////
void writeDataToTextFile8uc(
    unsigned char* data,
    int size1,  //noOfRows
    int size2,  //noOfColums
    char* filename
)
{
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "w");
#else
    stream = fopen(filename, "w");
#endif
    if (stream == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0;i<size1; i++){
        for(int j=0;j<size2; j++){
            //fprintf(stream,"%0.7lf\t", data[i*size2+j]);
            fprintf(stream,"%5d\t", (int)(data[i*size2+j]));
        }
        fprintf(stream,"\n");
    }
    fclose(stream);

}
///////////////////////////////////////////////////////////////////////////////
void readDataFromTextFile8uc(
    char* filename,
    unsigned char* data,
    int size1,  //noOfRows
    int size2   //noOfColums
)
{
    int val;
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "r");
#else
    stream = fopen(filename, "r");
#endif
    if (stream == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0;i<size1; i++){
        for(int j=0;j<size2; j++){
#if defined (_WIN32)
            fscanf_s(stream,"%d\t", &val); data[i*size2+j]=val;
#else
            fscanf(stream, "%d\t", &val); data[i*size2 + j] = val;
#endif
        }
    }
    fclose(stream);
}
///////////////////////////////////////////////////////////////////////////////
void readDataFromTextFile8uc(
    char* filename,
    unsigned char* data,
    int size    //noOfRows
)
{
    int val;
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "r");
#else
    stream = fopen(filename, "r");
#endif
    if (stream  == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0;i<size; i++){
#if defined (_WIN32)
        fscanf_s(stream,"%d\t", &val); data[i]=val;
#else
        fscanf(stream, "%d\t", &val); data[i] = val;
#endif
    }
    fclose(stream);
}
///////////////////////////////////////////////////////////////////////////////
void writeDataToTextFile8uc(
    unsigned char* data,
    int size,   //noOfRows
    char* filename
)
{
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "w");
#else
    stream = fopen(filename, "w");
#endif
    if (stream == NULL)

    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0; i<size; i++){
        fprintf(stream,"%5d\n", (int)(data[i]));
    }
    fclose(stream);
}
////////////////////////////////////////////////////////////////////////////
void writeDataToTextFile32f(
    float* data,
    int size1,  //noOfRows
    int size2,  //noOfColums
    char* filename
)
{
    FILE * stream ;
#if defined (_WIN32)
    fopen_s(&stream, filename, "w");
#else
    stream = fopen(filename, "w");
#endif
    if (stream == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    //fprintf(stream,"#size1 size2\n%d %d\n", size1, size2);
    for(int i=0;i<size1; i++){
        for(int j=0;j<size2; j++){
            fprintf(stream,"%0.012f\t", data[i*size2+j]);
            //fprintf(stream,"%5d\t", data[i*size2+j]);
        }
        fprintf(stream,"\n");
    }
    fclose(stream);

}
////////////////////////////////////////////////////////////////////////////
void writeDataToTextFile8sc(
    signed char* data,
    int size1,  //noOfRows
    int size2,  //noOfColums
    char* filename
)
{
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "w");
#else
    stream = fopen(filename, "w");
#endif
    if (stream  == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    //fprintf(stream,"#size1 size2\n%d %d\n", size1, size2);
    for(int i=0;i<size1; i++){
        for(int j=0;j<size2; j++){
            fprintf(stream,"%+d\t", int(data[i*size2+j]));
        }
        fprintf(stream,"\n");
    }
    fclose(stream);

}
///////////////////////////////////////////////////////////////////////////////////////////////
void writeDataToTextFile32f(
    float** data,
    int size1,  //noOfRows
    int size2,  //noOfColums
    char* filename
)
{
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "w");
#else
    stream = fopen(filename, "w");
#endif
    if (stream == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0;i<size1; i++){
        //data_ptr=data[i];
        for(int j=0;j<size2; j++){
            fprintf(stream,"%0.012f\t", data[i][j]);
            //fprintf(stream,"%0.07f\t", data_ptr[j]);
            //fprintf(stream,"%5d\t", data[i*size2+j]);
        }
        fprintf(stream,"\n");
    }
    fclose(stream);

}
///////////////////////////////////////////////////////////////////////////////////////////////
void writeDataToTextFile32i(
    int** data,
    int size1,  //noOfRows
    int size2,  //noOfColums
    char* filename
)
{
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "w");
#else
    stream = fopen(filename, "w");
#endif
    if (stream == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    int* data_ptr;
    for(int i=0;i<size1; i++){
        data_ptr=data[i];
        for(int j=0;j<size2; j++){
            fprintf(stream,"%d\t", data_ptr[j]);
        }
        fprintf(stream,"\n");
    }
    fclose(stream);

}
////////////////////////////////////////////////////////////////////////////
void writeDataToTextFile32f_e(
    float** data,
    int size1,  //noOfRows
    int size2,  //noOfColums
    char* filename
)
{
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "w");
#else
    stream = fopen(filename, "w");
#endif
    if (stream == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    float* data_ptr;
    for(int i=0;i<size1; i++){
        data_ptr=data[i];
        for(int j=0;j<size2; j++){
            fprintf(stream,"%e\t", data_ptr[j]);
            //fprintf(stream,"%5d\t", data[i*size2+j]);
        }
        fprintf(stream,"\n");
    }
    fclose(stream);

}
////////////////////////////////////////////////////////////////////////////
void writeDataToTextFile64f(
    double** data,
    int size1,  //noOfRows
    int size2,  //noOfColums
    char* filename
)
{
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "w");
#else
    stream = fopen(filename, "w");
#endif
    if (stream == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    double* data_ptr;
    for(int i=0;i<size1; i++){
        data_ptr=data[i];
        for(int j=0;j<size2; j++){
            //fprintf(stream,"%e\t", data_ptr[j]);
            fprintf(stream,"%lf\t", data_ptr[j]);
        }
        fprintf(stream,"\n");
    }
    fclose(stream);

}
////////////////////////////////////////////////////////////////////////////
void writeDataToTextFile32f(
    float* data,
    int size,
    char* filename
)
{
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "w");
#else
    stream = fopen(filename, "w");
#endif
    if (stream == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0;i<size; i++){
        fprintf(stream,"%0.012f\n", data[i]);
    }
    fclose(stream);

}
////////////////////////////////////////////////////////////////////////////
void writeDataToTextFile64f(
    double* data,
    int size,
    char* filename
)
{
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "w");
#else
    stream = fopen(filename, "w");
#endif
    if (stream == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0;i<size; i++){
        fprintf(stream,"%0.7lf\n", data[i]);
    }
    fclose(stream);
}
//////////////////////////////////////////////////////////////////////////////
void writeDataToTextFile64f(
    double* data,
    int size1,  //noOfRows
    int size2,  //noOfColums
    char* filename
)
{
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "w");
#else
    stream = fopen(filename, "w");
#endif
    if (stream == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0;i<size1; i++){
        for(int j=0;j<size2; j++){
            fprintf(stream,"%0.7lf\t", data[i*size2+j]);
        }
        fprintf(stream,"\n");
    }
    fclose(stream);

}
////////////////////////////////////////////////////////////////////////////
void writeDataToTextFile16i(
    short int* data,
    int size,
    char* filename
)
{
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "w");
#else
    stream = fopen(filename, "w");
#endif
    if (stream == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0;i<size; i++){
        fprintf(stream,"%d\n", data[i]);
    }
    fclose(stream);

}
////////////////////////////////////////////////////////////////////////////
void writeDataToTextFile32i(
    int* data,
    int size,
    char* filename
)
{
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "w");
#else
    stream = fopen(filename, "w");
#endif
    if (stream == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0;i<size; i++){
        fprintf(stream,"%d\n", data[i]);
    }
    fclose(stream);

}
//////////////////////////////////////////////////////////////////////
void readDataFromTextFile32f(
    char* filename,
    float* data,
    int size
){
    FILE* fp;
#if defined (_WIN32)
    fopen_s(&fp, filename, "r");
#else
    fp = fopen(filename, "r");
#endif
    if (fp == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0;i<size ; i++)
#if defined (_WIN32)
        fscanf_s(fp,"%f",&data[i]);
#else
        fscanf(fp, "%f", &data[i]);
#endif

    fclose(fp);
}
//////////////////////////////////////////////////////////////////////
void readDataFromTextFile32f_step(
    char* filename,
    float* data,
    int size,
    int step
){
    FILE* fp;
#if defined (_WIN32)
    fopen_s(&fp, filename, "r");
#else
    fp = fopen(filename, "r");
#endif
    if (fp == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    int count=0;
    for(int i=0;i<size ; i++){

        for(count=0;count<step;count++){
#if defined (_WIN32)
            fscanf_s(fp, "%f", &data[i]);
#else
            fscanf(fp, "%f", &data[i]);
#endif
        }

    }
    fclose(fp);
}
//////////////////////////////////////////////////////////////////////
void readDataFromTextFile64f(
    char* filename,
    double* data,
    int size
){
    FILE* fp;
#if defined (_WIN32)
    fopen_s(&fp, filename, "r");
#else
    fp = fopen(filename, "r");
#endif
    if (fp == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0;i<size ; i++)
#if defined (_WIN32)
        fscanf_s(fp, "%f", &data[i]);
#else
        fscanf(fp, "%lf", &data[i]);
#endif

    fclose(fp);
}
//////////////////////////////////////////////////////////////////////
void readDataFromTextFile16i(
    char* filename,
    short int* data,
    int size
){
    FILE* fp;
#if defined (_WIN32)
    fopen_s(&fp, filename, "r");
#else
    fp = fopen(filename, "r");
#endif
    if (fp == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0;i<size ; i++)
#if defined (_WIN32)
        fscanf_s(fp, "%f", &data[i]);
#else
        fscanf(fp, "%d", (int*)(&data[i]));
#endif

    fclose(fp);
}
//////////////////////////////////////////////////////////////////////
void readDataFromTextFile32i(
    char* filename,
    int* data,
    int size
){
    FILE* fp;
#if defined (_WIN32)
    fopen_s(&fp, filename, "r");
#else
    fp = fopen(filename, "r");
#endif
    if (fp == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    for(int i=0;i<size ; i++)
#if defined (_WIN32)
        fscanf_s(fp, "%f", &data[i]);
#else
        fscanf(fp, "%d", &data[i]);
#endif

    fclose(fp);
}
//////////////////////////////////////////////////////////////////////
void readDataFromTextFile64f(
    char* filename,
    double* data,
    int size1,
    int size2
){
    FILE* fp;
#if defined (_WIN32)
    fopen_s(&fp, filename, "r");
#else
    fp = fopen(filename, "r");
#endif
    if (fp == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }

    for(int i=0;i<size1; i++){
        for(int j=0;j<size2; j++){
#if defined (_WIN32)
            fscanf_s(fp,"%lf", &data[i*size2+j]);
#else
            fscanf(fp, "%lf", &data[i*size2 + j]);
#endif

        }
#if defined (_WIN32)
        fscanf_s(fp,"\n");
#else
        fscanf(fp, "\n");
#endif
    }
    fclose(fp);
}
/////////////////////////////////////////////////////////////////////////////////////
void readDataFromTextFile64f(
    char* filename,
    double** data,
    int size1,
    int size2
){
    FILE* fp;
#if defined (_WIN32)
    fopen_s(&fp, filename, "r");
#else
    fp = fopen(filename, "r");
#endif
    if (fp == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }

    for(int i=0;i<size1; i++){
        for(int j=0;j<size2; j++){
            fscanf(fp,"%lf", &(data[i])[j]);

        }
        fscanf(fp,"\n");
    }
    fclose(fp);
}
//////////////////////////////////////////////////////////////////////
void readDataFromTextFile32f(
    char* filename,
    float* data,
    int size1,
    int size2
){
    FILE* fp;
#if defined (_WIN32)
    fopen_s(&fp, filename, "r");
#else
    fp = fopen(filename, "r");
#endif
    if (fp == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }

    for(int i=0;i<size1; i++){
        for(int j=0;j<size2; j++){
            fscanf(fp,"%f", &data[i*size2+j]);

        }
        fscanf(fp,"\n");
    }
    fclose(fp);
}
////////////////////////////////////////////////////////////////////////////
void readDataFromTextFile32i(
    char* filename,
    int* data,
    int size1,
    int size2
){
    FILE* fp;
#if defined (_WIN32)
    fopen_s(&fp, filename, "r");
#else
    fp = fopen(filename, "r");
#endif
    if (fp == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }

    for(int i=0;i<size1; i++){
        for(int j=0;j<size2; j++){
            fscanf(fp,"%d", &data[i*size2+j]);

        }
        fscanf(fp,"\n");
    }
    fclose(fp);
}
////////////////////////////////////////////////////////////////////////////
int readData(
    char* filename,
    void* &data,
    size_t elemSize,
    bool isAllocateMem
)
{
    int size;
    FILE * stream;
#if defined (_WIN32)
    fopen_s(&stream, filename, "rb");
#else
    stream = fopen(filename, "rb");
#endif
    if (stream  == NULL)
    {
        printf("%s is fail to open", filename);
        exit(0);
    }
    fscanf(stream,"%u ",&size);
    int ch;
    do { ch=fgetc( stream ); }
    while( ch!=EOF && ch!=0x0a && ch!=0x0a); //CR(0x0a), LF(0x0d)
    //ungetc( ch, stream );

    if (isAllocateMem)  data =(float*)malloc(size*elemSize );
    fread(data,elemSize,size,stream);
    fclose(stream);

    return size;
}
/////////////////////////////////////////////////////////////////////////////
