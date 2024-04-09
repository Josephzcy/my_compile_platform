#pragma once
/// @file MP4Writer.cpp
/// @brief A MP4 Writer
/// @author ZhuangMingyu (ZhuangMingyu@minieye.cc) QQ25360199
/// @date 2018-06-05
/// Copyright (C) 2018 - MiniEye INC.

/*
warning:
1.未在大端环境下测试过
2.涉及的输出流，将调用seekp(0, beg)跳转到流开始位置，因此传入的流建议是干净的
3.在输入参数正确的情况下，可能抛出异常（内存不足，存储空间不足）并可能导致流无法正确seek。
本类本身不try异常。异常时，本类自身不会造成内存泄漏。
使用者可以在调用完Begin、Write或End时检查流的状态

usage:使用方法:

void makeMP4()
{
#define GetFileDataToBuf(filename, buf, fileSize)			\
uint32_t fileSize = 0; do {							\
FILE* f = fopen(filename, "rb");				\
if (f == NULL)									\
__debugbreak();								\
fseek(f, 0, SEEK_END);						\
fileSize = (uint32_t)ftell(f);				\
fseek(f, 0, SEEK_SET);						\
buf.resize(fileSize);						\
fread(&buf[0], fileSize, 1, f);				\
fclose(f);									\
} while (0);



std::vector<const char*> jpgs =
{
"E:/jpg2mp4/0.jpg",
"E:/jpg2mp4/1.jpg",
"E:/jpg2mp4/2.jpg",
"E:/jpg2mp4/3.jpg",
"E:/jpg2mp4/4.jpg",
"E:/jpg2mp4/5.jpg",
"E:/jpg2mp4/6.jpg",
"E:/jpg2mp4/7.jpg",
"E:/jpg2mp4/8.jpg",
"E:/jpg2mp4/9.jpg",
"E:/jpg2mp4/10.jpg",
"E:/jpg2mp4/11.jpg",
"E:/jpg2mp4/12.jpg",
"E:/jpg2mp4/13.jpg",
"E:/jpg2mp4/14.jpg",
};

int fps = 12;
int width = 640;
int height = 360;

std::ofstream out("E:/jpg2mp4/out.mp4", std::ios_base::out | std::ios::trunc | std::ios::binary);
std::vector<char> buf;
buf.reserve(1024 * 20);


MP4Writer* writer = new MP4Writer{ out, width, height };

for (const char* jpg : jpgs)
{
GetFileDataToBuf(jpg, buf, fileSize);
writer->WriteFrame(&buf[0], fileSize, MP4Writer::ScalePerSecond / fps);
}

writer->End();
out.close();
}
*/


#ifndef COMMON_MP4_MP4WRITER_H_
#define COMMON_MP4_MP4WRITER_H_

#include <vector>
#include <ostream>

class MP4Writer
{
public:
	MP4Writer() = default;
	MP4Writer(std::ostream& os, int width, int height) { Begin(os, width, height); }

	enum { ScalePerSecond = 60000 }; //每60数值对应1ms
	enum { DefaultFPS = 12 };

	//************************************
	// Method:    Begin					重置成可输出状态
	// Returns:   int					0表示没出错（然而内部代码并不返回非0值）
	// Qualifier:
	// Parameter: std::ostream & os		输出流，由于将调用seekp(0, beg)跳转到流开始位置，因此传入的流建议是干净的
	// Parameter: int width				宽度，直接写入mp4文件而不加判断
	// Parameter: int height			高度，直接写入mp4文件而不加判断
	// Exception: From ostream::write	异常同ostream::write
	//************************************
	int Begin(std::ostream& os, int width, int height);

	//************************************
	// Method:    WriteFrame			写入一帧
	// Returns:   int					0表示没出错（然而内部代码并不返回非0值）
	// Qualifier:
	// Parameter: void * buf			jpg图片文件数据
	// Parameter: int size				jpg图片文件大小
	// Parameter: int durationScaled	放大过的时长(即使用每秒ScalePerSecond的单位) 
	//										durationScaled = ScalePerSecond / fps
	// Exception: From ostream::Write	异常同ostream::write
	//************************************
	int WriteFrame(void* buf, int size, int durationScaled = ScalePerSecond / DefaultFPS);

	//************************************
	// Method:    End					完成文件
	// Returns:   int					0表示没出错（然而内部代码并不返回非0值）
	// Qualifier:
	// Exception: From ostream::write/seekp 异常同ostream::write、seekp
	//************************************
	int End();

	void Clear() noexcept;				//简单重置本类所有成员数据，恢复到无参数构造时的状态
private:
	std::ostream* out = NULL;
	unsigned short mWidth = -1;
	unsigned short mHeight = -1;

	struct SFrameInfo
	{
		int size = 0;
		int duration = 0;
	};
	std::vector<SFrameInfo> mFrameInfos;

	//unsigned int mPosOfMDAT = 0;
	unsigned int mSizeWritedToStream = 0;
};

#endif  // COMMON_MP4_MP4WRITER_H_