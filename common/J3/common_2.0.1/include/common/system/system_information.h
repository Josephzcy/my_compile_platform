#ifndef COMMON_SYSTEM_SYSTEM_INFORMATION_H
#define COMMON_SYSTEM_SYSTEM_INFORMATION_H

#include <string>

unsigned int GetLogicalCpuNumber();
unsigned long long GetPhysicalMemorySize();
unsigned long long GetTotalPhysicalMemorySize();
bool GetOsKernelInfo(std::string* kernerl_info);
bool GetMachineArchitecture(std::string* arch_info);
std::string GetUserName();

#endif // COMMON_SYSTEM_SYSTEM_INFORMATION_H
