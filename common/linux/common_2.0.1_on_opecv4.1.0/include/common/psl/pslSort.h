#ifndef __TX_SORT__
#define __TX_SORT__
//////////////////////////////////////////////////////////////////////////////
void qSortAscend32f( 
	float* data,  
	int length
);   

void qSortAscend32i( 
	int* data,  
	int length
); 

void qSortAscendIndex32i( 
	int* data,  
	int length,  
	int* sortedIdx
);

void qSortAscendIndex32f( 
	float* data,  
	int length,  
	int* sortedIdx
);
///////////////////////////////////////////////////////////////////////
#endif /* __TX_SORT__ */