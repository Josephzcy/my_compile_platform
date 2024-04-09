#include <stdio.h>
#include <string.h>
#include "common/string/sprintf.h"

std::string SNPrintf(size_t size, const char* format, ...) {
  va_list ap;
  va_start(ap, format);
  std::string res = VSNPrintf(size, format, ap);
  va_end(ap);
  return res;
}

std::string VSNPrintf(size_t size, const char* format, va_list ap) {
  char buf[size];
  memset(buf, 0, size);
  vsnprintf(buf, size, format, ap);
  return buf;
}
