#include <algorithm>
#include "Util.h"


void PrintMessage(const char *str){
	if (str != NULL)
		printf("%s\n", str);
}

//一位十六进制转换为十进制
int HexChar(char c)
{
	if ((c >= '0') && (c <= '9'))
		return c - 0x30;
	else if ((c >= 'A') && (c <= 'F'))
		return c - 'A' + 10;
	else if ((c >= 'a') && (c <= 'f'))
		return c - 'a' + 10;
	else
		return 0x10;
}

//两位十六进制数转换为十进制
int Str2Hex(const std::string& str)
{
	int len = str.length();
	if (len == 2)
	{
		int a = HexChar(str[0]);
		int b = HexChar(str[1]);
		if (a >= 16 || b >= 16)
		{
			PrintMessage("Format error");
			return 256;
		}
		else
		{
			return a * 16 + b;
		}
	}
	else
	{
		PrintMessage("input length must be 2");
		return 256;
	}
}

std::string ToLower(const std::string& str)
{
	std::string t = str;
	transform(t.begin(), t.end(), t.begin(), tolower);
	return t;
}

bool EqualIgnoreCase(const std::string& str1, const std::string& str2)
{
	return ToLower(str1) == ToLower(str2);
}

double GetBitValue(unsigned char data[], int len, int startBit, int bitLen)
{
	if (startBit + bitLen > len * 8)
		return 0;

	double res = 0.0;
	for (int i = startBit + bitLen - 1; i >= startBit; --i)
	{
		int byte = i / 8;
		int byteIndex = i % 8;
		res *= 2;
		//res += (data[byte] & (1 << (7 - byteIndex))) > 0 ? 1 : 0;
		res += (data[byte] & (1 << byteIndex)) > 0 ? 1 : 0;
	}
	return res;
}
