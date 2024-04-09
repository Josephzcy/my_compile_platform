#include "pslSort.h"
//////////////////////////////////////////////////////////////////////////////////////
// SORTING FUNCTIONS 
   
 void qSortAscend32f( 
	float* data,  
	int length
)             
{  
    const int bubble_level = 8;                                                                                                               \
	struct {                                                          
        int lb, ub;                                 
    } stack[48];                                           
                                                                        
    int sp = 0;                                                         
                                                                        
    float   temp;                                                          
    float  lb_val;                                                 
                                                                       
    stack[0].lb = 0;                                                   
    stack[0].ub = length - 1;                                           
                                                                     
    while( sp >= 0 ) {                                                                    
        int lb = stack[sp].lb;                                           
        int ub = stack[sp--].ub;                                         
                                                                       
        for(;;)  {                                                               
            int diff = ub - lb;                                         
            if( diff < bubble_level )  {                                                           
                int i, j;                                               
                float* arr = data + lb;                                    
                                                                        
                for( i = diff; i > 0; i-- ) {                                                
                    int f = 0;                                          
                    for( j = 0; j < i; j++ )                            
                        if(  arr[j+1]< arr[j])              
                        {                                               
                            temp = arr[j];                              
                            arr[j] = arr[j+1];                          
                            arr[j+1] = temp;                            
                            f = 1;                                      
                        }                                               
                    if( !f ) break;                                     
                }                                                       
                break;                                                  
            }                                                           
            else  {                                                           
                /* select pivot and exchange with 1st element */        
                int  m = lb + (diff >> 1);                              
                int  i = lb + 1, j = ub;                                
                                                                        
                lb_val = data[m];                                      
                                                                        
                data[m]  = data[lb];                                  
                data[lb] = lb_val;                                     
                                                                        
                /* partition into two segments */                       
                for(;;) {                                                       
                    for( ;i < j &&  data[i]<lb_val; i++ );  
                    for( ;j >= i && lb_val<data[j]; j-- ); 
                                                                        
                    if( i >= j ) break;                                 
                    temp = data[i];                                    
                    data[i++] = data[j];                              
                    data[j--] = temp;                                  
                }                                                       
                                                                        
                /* pivot belongs in A[j] */                             
                data[lb] = data[j];                                   
                data[j]  = lb_val;                                     
                                                                        
                /* keep processing smallest segment, and stack largest*/
                if( j - lb <= ub - j )  {                                                       
                    if( j + 1 < ub ) {                                                  
                        stack[++sp].lb   = j + 1;                       
                        stack[sp].ub = ub;                              
                    }                                                   
                    ub = j - 1;                                         
                }                                                       
                else   {                                                       
                    if( j - 1 > lb)                                     
                    {                                                   
                        stack[++sp].lb = lb;                            
                        stack[sp].ub = j - 1;                           
                    }                                                   
                    lb = j + 1;                                         
                }                                                       
            }                                                           
        }                                                               
    }                                                                   
}
////////////////////////////////////////////////////////////////////////////////////////////
void qSortAscend32i( 
	int* data,  
	int length
)             
{  
    const int bubble_level = 8;                                                                                                               \
	struct {                                                          
        int lb, ub;                                 
    } stack[48];                                           
                                                                        
    int sp = 0;                                                         
                                                                        
    int   temp;                                                          
    int  lb_val;                                                 
                                                                       
    stack[0].lb = 0;                                                   
    stack[0].ub = length - 1;                                           
                                                                     
    while( sp >= 0 ) {                                                                    
        int lb = stack[sp].lb;                                           
        int ub = stack[sp--].ub;                                         
                                                                       
        for(;;)  {                                                               
            int diff = ub - lb;                                         
            if( diff < bubble_level )  {                                                           
                int i, j;                                               
                int* arr = data + lb;                                    
                                                                        
                for( i = diff; i > 0; i-- ) {                                                
                    int f = 0;                                          
                    for( j = 0; j < i; j++ )                            
                        if(  arr[j+1]< arr[j])              
                        {                                               
                            temp = arr[j];                              
                            arr[j] = arr[j+1];                          
                            arr[j+1] = temp;                            
                            f = 1;                                      
                        }                                               
                    if( !f ) break;                                     
                }                                                       
                break;                                                  
            }                                                           
            else  {                                                           
                /* select pivot and exchange with 1st element */        
                int  m = lb + (diff >> 1);                              
                int  i = lb + 1, j = ub;                                
                                                                        
                lb_val = data[m];                                      
                                                                        
                data[m]  = data[lb];                                  
                data[lb] = lb_val;                                     
                                                                        
                /* partition into two segments */                       
                for(;;) {                                                       
                    for( ;i < j &&  data[i]<lb_val; i++ );  
                    for( ;j >= i && lb_val<data[j]; j-- ); 
                                                                        
                    if( i >= j ) break;                                 
                    temp = data[i];                                    
                    data[i++] = data[j];                              
                    data[j--] = temp;                                  
                }                                                       
                                                                        
                /* pivot belongs in A[j] */                             
                data[lb] = data[j];                                   
                data[j]  = lb_val;                                     
                                                                        
                /* keep processing smallest segment, and stack largest*/
                if( j - lb <= ub - j )  {                                                       
                    if( j + 1 < ub ) {                                                  
                        stack[++sp].lb   = j + 1;                       
                        stack[sp].ub = ub;                              
                    }                                                   
                    ub = j - 1;                                         
                }                                                       
                else   {                                                       
                    if( j - 1 > lb)                                     
                    {                                                   
                        stack[++sp].lb = lb;                            
                        stack[sp].ub = j - 1;                           
                    }                                                   
                    lb = j + 1;                                         
                }                                                       
            }                                                           
        }                                                               
    }                                                                   
} 
///////////////////////////////////////////////////////////////////////////////////////
void qSortAscendIndex32i( 
	int* data,  
	int length,  
	int* sortedIdx
)             
{             
	for (int m=0; m<length; m++) 	sortedIdx[m] = m;

    const int bubble_level = 8;                                                                                                               \
	struct {                                                          
        int lb, ub;                                 
    } stack[48];                                           
                                                                        
    int sp = 0;                                                         
                                                                        
    int   temp;                                                          
    int   lb_val;                                                 
                                                                       
    stack[0].lb = 0;                                                   
    stack[0].ub = length - 1;                                           
                                                                     
    while( sp >= 0 ) {                                                                    
        int lb = stack[sp].lb;                                           
        int ub = stack[sp--].ub;                                         
                                                                       
        for(;;)  {                                                               
            int diff = ub - lb;                                         
            if( diff < bubble_level )  {                                                           
                int i, j;                                               
                int* arr = sortedIdx + lb;                                    
                                                                        
                for( i = diff; i > 0; i-- ) {                                                
                    int f = 0;                                          
                    for( j = 0; j < i; j++ )                            
                        if( data[arr[j+1]] < data[arr[j]] ) {                                               
                            temp = arr[j];                              
                            arr[j] = arr[j+1];                          
                            arr[j+1] = temp;                            
                            f = 1;                                      
                        }                                               
                    if( !f ) break;                                     
                }                                                       
                break;                                                  
            }                                                           
            else  {                                                           
                /* select pivot and exchange with 1st element */        
                int  m = lb + (diff >> 1);                              
                int  i = lb + 1, j = ub;                                
                                                                        
                lb_val = sortedIdx[m];                                      
                                                                        
                sortedIdx[m]  = sortedIdx[lb];                                  
                sortedIdx[lb] = lb_val;                                     
                                                                        
                /* partition into two segments */                       
                for(;;) {                                                       
                    for( ;i < j &&  data[ sortedIdx[i]] < data[lb_val] ; i++ );  
                    for( ;j >= i && data[lb_val] < data[sortedIdx[j]]; j-- ); 
                                                                        
                    if( i >= j ) break;                                 
                    temp = sortedIdx[i];                                    
                    sortedIdx[i++] = sortedIdx[j];                              
                    sortedIdx[j--] = temp;                                  
                }                                                       
                                                                        
                /* pivot belongs in A[j] */                             
                sortedIdx[lb] = sortedIdx[j];                                   
                sortedIdx[j]  = lb_val;                                     
                                                                        
                /* keep processing smallest segment, and stack largest*/
                if( j - lb <= ub - j )  {                                                       
                    if( j + 1 < ub ) {                                                  
                        stack[++sp].lb   = j + 1;                       
                        stack[sp].ub = ub;                              
                    }                                                   
                    ub = j - 1;                                         
                }                                                       
                else   {                                                       
                    if( j - 1 > lb)                                     
                    {                                                   
                        stack[++sp].lb = lb;                            
                        stack[sp].ub = j - 1;                           
                    }                                                   
                    lb = j + 1;                                         
                }                                                       
            }                                                           
        }                                                               
    }                                                                   
}
/////////////////////////////////////////////////////////////////////////////// 
void qSortAscendIndex32f( 
	float* data,  
	int length,  
	int* sortedIdx
)             
{             
	for (int m=0; m<length; m++) sortedIdx[m] = m;

    const int bubble_level = 8;                                                                                                               \
	struct {                                                          
        int lb, ub;                                 
    } stack[48];                                           
                                                                        
    int sp = 0;                                                         
                                                                        
    int   temp;                                                          
    int   lb_val;                                                 
                                                                       
    stack[0].lb = 0;                                                   
    stack[0].ub = length - 1;                                           
                                                                     
    while( sp >= 0 ) {                                                                    
        int lb = stack[sp].lb;                                           
        int ub = stack[sp--].ub;                                         
                                                                       
        for(;;)  {                                                               
            int diff = ub - lb;                                         
            if( diff < bubble_level )  {                                                           
                int i, j;                                               
                int* arr = sortedIdx + lb;                                    
                                                                        
                for( i = diff; i > 0; i-- ) {                                                
                    int f = 0;                                          
                    for( j = 0; j < i; j++ )                            
                        if( data[arr[j+1]] < data[arr[j]] ) {                                               
                            temp = arr[j];                              
                            arr[j] = arr[j+1];                          
                            arr[j+1] = temp;                            
                            f = 1;                                      
                        }                                               
                    if( !f ) break;                                     
                }                                                       
                break;                                                  
            }                                                           
            else  {                                                           
                /* select pivot and exchange with 1st element */        
                int  m = lb + (diff >> 1);                              
                int  i = lb + 1, j = ub;                                
                                                                        
                lb_val = sortedIdx[m];                                      
                                                                        
                sortedIdx[m]  = sortedIdx[lb];                                  
                sortedIdx[lb] = lb_val;                                     
                                                                        
                /* partition into two segments */                       
                for(;;) {                                                       
                    for( ;i < j &&  data[ sortedIdx[i]] < data[lb_val] ; i++ );  
                    for( ;j >= i && data[lb_val] < data[sortedIdx[j]]; j-- ); 
                                                                        
                    if( i >= j ) break;                                 
                    temp = sortedIdx[i];                                    
                    sortedIdx[i++] = sortedIdx[j];                              
                    sortedIdx[j--] = temp;                                  
                }                                                       
                                                                        
                /* pivot belongs in A[j] */                             
                sortedIdx[lb] = sortedIdx[j];                                   
                sortedIdx[j]  = lb_val;                                     
                                                                        
                /* keep processing smallest segment, and stack largest*/
                if( j - lb <= ub - j )  {                                                       
                    if( j + 1 < ub ) {                                                  
                        stack[++sp].lb   = j + 1;                       
                        stack[sp].ub = ub;                              
                    }                                                   
                    ub = j - 1;                                         
                }                                                       
                else   {                                                       
                    if( j - 1 > lb)                                     
                    {                                                   
                        stack[++sp].lb = lb;                            
                        stack[sp].ub = j - 1;                           
                    }                                                   
                    lb = j + 1;                                         
                }                                                       
            }                                                           
        }                                                               
    }                                                                   
}

//////////////////////////////////////////////////////////////////////// 
