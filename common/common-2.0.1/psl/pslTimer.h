#ifdef _WIN32
#include <windows.h>
//////////////////////////////////////////////////////////////////////////////////////
static LARGE_INTEGER ticksPerSecond;
static LARGE_INTEGER t0,t1;

#define __START_TIMER__ \
	QueryPerformanceFrequency(&ticksPerSecond);\
	QueryPerformanceCounter(&t0);\

#define __END_TIMER__ \
	QueryPerformanceCounter(&t1);\
	printf("TIME:t1-t0 (sec) = %7.7lf    (Min) = %0.2lf\n", (double)(t1.QuadPart-t0.QuadPart)/ticksPerSecond.QuadPart, ((double)(t1.QuadPart-t0.QuadPart)/ticksPerSecond.QuadPart)/60.0f);\

#else
#include <sys/time.h>
static timeval t0, t1;

#define __START_TIMER__ \
	gettimeofday(&t0, NULL);\

#define __END_TIMER__ \
	gettimeofday(&t1, NULL); \
	printf("TIME:t1-t0 (sec) = %7.7lf\n", (double)(t1.tv_sec - t0.tv_sec + (t1.tv_usec - t0.tv_usec) / 1000000.0)); \

#endif

