#pragma once

#include <string>

void PrintMessage(const char *str);
//一位十六进制转换为十进制
int HexChar(char c);
//两位十六进制数转换为十进制
int Str2Hex(const std::string& str);

std::string ToLower(const std::string& str);
bool EqualIgnoreCase(const std::string& str1, const std::string& str2);
double GetBitValue(unsigned char data[], int len, int startBit, int bitLen);
