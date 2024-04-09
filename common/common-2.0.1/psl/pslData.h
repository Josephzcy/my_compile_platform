#ifndef __TX_DATA__
#define __TX_DATA__
/////////////////////////////////////////////////////////////////////////////////
 /* C/C++ Libraries */
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <vector>

 
#include "pslFile.h"
//-----------------------------
 
////////////////////////////////////////////////////////////////////////////
void writeData(
	void* data,
	int size,
	size_t elemSize,
	char* filename
);
/*
void writeDataToTextFile32ui(
	unsigned int* data,
	int size1,	//noOfRows
	int size2,	//noOfColums
	char* filename
);
*/

void writeDataToTextFile32ul(
    unsigned long* data,
	int size1,	//noOfRows
	int size2,	//noOfColums
	char* filename
);

void writeDataToTextFile32i(
    int* data,
	int size1,	//noOfRows
	int size2,	//noOfColums
	char* filename
);

void writeDataToTextFile32f(
	float* data,
	int size1,	//noOfRows
	int size2,	//noOfColums
	char* filename
);

void writeDataToTextFile8sc(
	signed char* data,
	int size1,	//noOfRows
	int size2,	//noOfColums
	char* filename
);

void writeDataToTextFile32f(
	float** data,
	int size1,	//noOfRows
	int size2,	//noOfColums
	char* filename
);
void writeDataToTextFile32f_e(
	float** data,
	int size1,	//noOfRows
	int size2,	//noOfColums
	char* filename
);

void writeDataToTextFile64f(
	double* data,
	int size,	
	char* filename
);


void writeDataToTextFile64f(
	double** data,
	int size1,	
	int size2,	
	char* filename
);

void writeDataToTextFile32f(
	float* data,
	int size,	
	char* filename
);
void writeDataToTextFile16i(
	short int* data,
	int size,	
	char* filename
);

void readDataFromTextFile16i(
	char* filename,
	short int* data,
	int size 
);
void writeDataToTextFile32i(
	int* data,
	int size,	
	char* filename
);

void writeDataToTextFile64f(
	double* data,
	int size1,	//noOfRows
	int size2,	//noOfColums
	char* filename
);
void writeDataToTextFile32i(
	int** data,
	int size1,	//noOfRows
	int size2,	//noOfColums
	char* filename
);
//..........................................
void writeDataToTextFile8uc(
	unsigned char* data,
	int size1,	//noOfRows
	int size2,	//noOfColums
	char* filename
);

//..........................................
void writeDataToTextFile8uc(
	unsigned char* data,
	int size, 
	char* filename
);
//..........................................
int readData(
	char* filename,
	void* &data,
	size_t elemSize,
	bool isAllocateMem=true
);

void readDataFromTextFile32f(
	char* filename,
	float* data, 
	int size 
);

void readDataFromTextFile32f_step(
	char* filename,
	float* data, 
	int size,
	int step=2
);

void readDataFromTextFile64f(
	char* filename,
	double* data, 
	int size 
);
void readDataFromTextFile64f(
	char* filename,
	double* data, 
	int size1,
	int size2
);
void readDataFromTextFile64f(
	char* filename,
	double** data, 
	int size1,
	int size2
);
void readDataFromTextFile32i(
	char* filename,
	int* data, 
	int size 
);
void readDataFromTextFile32i(
	char* filename,
	int* data, 
	int size1,
	int size2
);
void readDataFromTextFile32f(
	char* filename,
	float* data, 
	int size1, 
	int size2
);
void readDataFromTextFile8uc(
	char* filename,
	unsigned char* data,
	int size1,	//noOfRows
	int size2	//noOfColums
);
void readDataFromTextFile8uc(
	char* filename,
	unsigned char* data,
	int size 	//noOfRows 
);
void readDataRow(
	char* filename,
	void* &data,
	size_t elemSize,
	int noOfCols,
	int rowNo,	
	bool isAllocateMem=true
); 
//...........................................
 
 
/////////////////////////////////////////////////////////////////////////////////
#endif // `__TX_DATA__
