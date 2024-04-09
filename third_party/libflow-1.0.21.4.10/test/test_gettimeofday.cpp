#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <sys/time.h>

double GetCurrentTimeInSecondsDouble() {
  struct timeval tv;
  int rc = gettimeofday(&tv, NULL);
  if (rc == -1) {
    fprintf(stderr, "gettimeofday failed with %d\n", errno);
    return -1.0;
  }

  return tv.tv_sec + tv.tv_usec / 1e6;
}

int64_t GetCurrentTimeInMicroSecondsI64() {
  struct timeval tv;
  int rc = gettimeofday(&tv, NULL);
  if (rc == -1) {
    fprintf(stderr, "gettimeofday failed with %d\n", errno);
    return -1;
  }

  return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main() {
  double now = GetCurrentTimeInSecondsDouble();
  printf("%lf\n", now);

  int64_t now1 = GetCurrentTimeInMicroSecondsI64();
  printf("%lld\n", now1);

  return 0;
}
