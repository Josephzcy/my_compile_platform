#pragma once

#include <string>

void PrintMessage(const char *str);
//һλʮ������ת��Ϊʮ����
int HexChar(char c);
//��λʮ��������ת��Ϊʮ����
int Str2Hex(const std::string& str);

std::string ToLower(const std::string& str);
bool EqualIgnoreCase(const std::string& str1, const std::string& str2);
double GetBitValue(unsigned char data[], int len, int startBit, int bitLen);
