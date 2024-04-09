#ifndef COMMON_STRING_SPRINTF_H
#define COMMON_STRING_SPRINTF_H

#include <stdarg.h>
#include <string>

std::string SNPrintf(size_t size, const char* format, ...);
std::string VSNPrintf(size_t size, const char* format, va_list ap);

#endif // COMMON_STRING_SPRINTF_H
