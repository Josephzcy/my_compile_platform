/// @file main.cpp
/// @brief Convert images to mp4 video file.
/// @author ZhuangMingYu <zhuangmingyu@minieye.cc> QQ25360199
/// @date 2018-06-06
/// Copyright (C) 2018 - MiniEye INC.

//////////////////////////////////////////////////////////////////////////
//代码配置

#define __author__ "ZhuangMingYu <zhuangmingyu@minieye.cc> QQ25360199"
#define __version__ "c++0.1 beta "
#define __progname__ "jpg2mp4"
#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "./MP4Writer.h"
#include <deque>
#include <algorithm>

#ifdef __GNUC__
#ifndef __debugbreak_defined
#define __debugbreak_defined
#ifdef __ANDROID__
#include <assert.h>
#define __debugbreak() assert(false)
#else
#define __debugbreak() __asm("int $3")
#endif

#endif

#define stricmp strcasecmp

#endif

#ifdef _WIN32  
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

#ifdef _WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#else
#define ACCESS access
#define MKDIR(a) mkdir((a), 0755)
#endif

int CreateDir(const char *pcDir)
{
	int iRet = -1;
	if (pcDir == NULL)
	{
		__debugbreak();
		return 0;
	}
	std::string dir = pcDir;
	char* pDir = &dir[0];

	int i = 0;
	// 创建中间目录  
	for (; pDir[i]; i++)
	{
		if (pDir[i] == '\\' || pDir[i] == '/')
		{
			pDir[i] = '\0';

			//如果不存在,创建  
			iRet = ACCESS(pDir, 0);
			if (iRet != 0)
			{
				iRet = MKDIR(pDir);
				if (iRet != 0)
					return -1;
			}
			//支持linux,将所有\换成/  
			pDir[i] = '/';
		}
	}
	if (pDir[i - 1] != '/')
		iRet = MKDIR(pDir);
	return iRet;
}


//////////////////////////////////////////////////////////////////////////
/* Test for GCC > 3.2.0 */
#if !defined(_MSC_VER) && !defined(GCC_VERSION)
#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)
#endif

#if defined(GCC_VERSION) && GCC_VERSION < 70000

#ifndef _StringViewDefined
#define _StringViewDefined
namespace std
{
	class string_view
	{
	private:
		const char* m_data = NULL;
		size_t m_size = 0;
	public:
		string_view() = default;
		string_view(const string_view&) = default;
		string_view(const char* data, size_t size) : m_data(data), m_size(size) { }
		inline const char* data() const { return m_data; }
		inline size_t size() const { return m_size; }
	};
}
#endif

#else
#include <string_view>
#ifndef CONSTEXPR_IF
#define CONSTEXPR_IF
#endif
#endif

template<typename T>
struct has_reserve
{
	template<typename C> static char test(decltype(&C::no_destroy));
	template<typename C> static short test(...);
	enum { value = sizeof(test<T>(nullptr)) == 1 };
};

template<typename Container = std::vector<std::string_view>>
Container Split(const char* str, const char* delimiter, std::string_view emptyValue = {}, int maxCount = 0)
{
	Container results;
	if (str == NULL || *str == 0)
		return results;
	size_t sizeOfStr = strlen(str);

	if (delimiter == NULL || *delimiter == 0)
	{
		results.emplace_back(str, sizeOfStr);
		return results;
	}

#ifdef CONSTEXPR_IF
	if constexpr(has_reserve<Container>::value)
		//__if_exists (Container::reserve)
	{
		results.reserve(16);
	}
#endif
	size_t sizeOfDelimiter = strlen(delimiter);

	int n = 0;
	maxCount = maxCount == 0 ? 1 << 30 : maxCount;
	intptr_t lastpos = 0, pos = strstr(str, delimiter) - str;

	for (; n + 1 < maxCount && pos != intptr_t((const char*)0 - str); )
	{
		if (pos - lastpos > 0)
			results.emplace_back(&str[lastpos], size_t(pos - lastpos));
		else
			results.emplace_back(emptyValue);
		++n;

		lastpos = pos + sizeOfDelimiter;
		pos = strstr(str + pos + sizeOfDelimiter, delimiter) - str;
	}
	pos = sizeOfStr;
	{
		if (pos - lastpos > 0)
			results.emplace_back(&str[lastpos], size_t(pos - lastpos));
		else
			results.emplace_back(emptyValue);
		++n;
	}
	return results;
}

//////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		if (stricmp(argv[1], "--version") == 0
			|| stricmp(argv[1], "-v") == 0)
		{
			puts(__progname__ "(" __author__ ")" " version " __version__);
			return 0;
		}
		else if (stricmp(argv[1], "--help") == 0
			|| stricmp(argv[1], "-h") == 0)
		{
			puts(
R"(Usage: jpg2mp4 -s <width>:<height> -o <OutputFileName> --fps 12 {Group} {Group}...
    Every Group: see -I
example:
    jpg2mp4 -s 360:240 -o xxx/a.mp4 -i /home/zmy/jpgs/%d.jpg(0~58,fps12)
        generate fps12 mp4 with /home/zmy/jpgs/0.jpg ... /home/zmy/jpgs/58.jpg
    jpg2mp4 -s 360:240 -o a.mp4 --fps 30 -i 0.jpg(fps12) -i %d.jpg(1~20) -i %d.jpg(21~30,fps20)
        generate mp4 with 0.jpg(fps12), 1.jpg~20.jpg(fps30), 21.jpg~30.jpg(fps20)
Options:
    -H, --help        Show this message and exit.
    -I        TEXT    Specify a group of jpg file. There cound be multi groups.
                      {FilePath}
                      {FilePath}(fps{fps})
                      {FileFormat}({begin}~{end})
                      {FileFormat}({begin}~{end},fps{fps})

    -O        TEXT    Specify an image file(default: output.mp4)
                      When no {-O, image_dir, output_dir, --output-dir} arg, 
                      try to use second unused arg as output dir.

    -F,--fps  INT     Specify global fps. (default: 12) 
                      Effect any frames which has not specified fps.

    -S,--size INT:INT Specify width and height
)");
			return 0;
		}
	}


	char output_file[1024] = "";
	int global_fps = 12;
	int width = 0;
	int height = 0;
	bool usedOfArg[10000] = {};
	usedOfArg[0] = true;

	std::deque<std::pair<std::string, int /*fps*/>> frames = {};
	auto parseRange = [&](const char* range)
	{
		std::string item = range;
		auto pathAndInfo = Split(item.data(), "(");
		if (pathAndInfo.size() == 1)
			frames.emplace_back(item, -1);
		else if (pathAndInfo.size() == 2)
		{
			((char*)pathAndInfo[1].data())[-1] = 0;

			auto rangeAndFPS = Split(pathAndInfo[1].data(), "fps");
			int fps = -1;
			if (rangeAndFPS.size() == 2)
			{
				((char*)rangeAndFPS[1].data())[-3] = 0;
				fps = atoi(rangeAndFPS[1].data());
			}

			if (rangeAndFPS[0].size() > 0)
			{
				auto range = Split(rangeAndFPS[0].data(), "~");
				int begin = atoi(range[0].data());
				int end = atoi(range[1].data());
				for (int i = begin; i <= end; ++i)
				{
					char filename[1024];
					sprintf(filename, pathAndInfo[0].data(), i);
					frames.emplace_back(filename, fps);
				}
			}
		}
	};
	for (int i = 1; i < argc; ++i)
	{
		if (i + 1 < argc)
		{
			if (stricmp(argv[i], "-i") == 0)
			{
				parseRange(argv[i + 1]);
				usedOfArg[i] = true;
				usedOfArg[++i] = true;
			}
			else if (stricmp(argv[i], "-o") == 0)
			{
				strcpy(output_file, argv[i + 1]);
				usedOfArg[i] = true;
				usedOfArg[++i] = true;
			}
			else if (stricmp(argv[i], "-f") == 0 || stricmp(argv[i], "--fps") == 0)
			{
				global_fps = atoi(argv[i + 1]);
				usedOfArg[i] = true;
				usedOfArg[++i] = true;
			}
			else if (stricmp(argv[i], "-s") == 0 || stricmp(argv[i], "--size") == 0)
			{
				auto ss = Split(argv[i + 1], ":");
				width = atoi(ss[0].data());
				height = atoi(ss[1].data());
				usedOfArg[i] = true;
				usedOfArg[++i] = true;
			}
		}
	}

	if (frames.size() == 0) //当没有输入文件参数时
	{
		int firstUnusedArgIndex = int(std::find(usedOfArg + 1, usedOfArg + argc, false) - usedOfArg);
		if (firstUnusedArgIndex < argc)
		{
			parseRange(argv[firstUnusedArgIndex]); //视频文件使用第一个未使用参数
			usedOfArg[firstUnusedArgIndex] = true;
		}
		else
		{
			puts("No input file arg.");
			return -1;
		}
	}
	if (output_file[0] == '\0') //当没有输出路径参数时
	{
		int firstUnusedArgIndex = int(std::find(usedOfArg + 1, usedOfArg + argc, false) - usedOfArg);
		if (firstUnusedArgIndex < argc)
		{
			strcpy(output_file, argv[firstUnusedArgIndex]); //输出路径使用第一个未使用参数
			usedOfArg[firstUnusedArgIndex] = true;
		}
		else
		{
			strcpy(output_file, "output.mp4");
		}
	}

	if (width == 0) {
		fprintf(stderr, "Error: invalid value of width\n");
		exit(EXIT_FAILURE);
	}

	if (height == 0) {
		fprintf(stderr, "Error: invalid value of height\n");
		exit(EXIT_FAILURE);
	}

#define GetFileDataToBuf(filename, buf, fileSize)			\
		uint32_t fileSize = 0; do {							\
			FILE* f = fopen(filename, "rb");				\
			if (f == NULL)									\
			{	puts(filename);	perror("fopen");			\
				__debugbreak();}							\
			fseek(f, 0, SEEK_END);							\
			fileSize = (uint32_t)ftell(f);					\
			fseek(f, 0, SEEK_SET);							\
			buf.resize(fileSize);							\
			fread(&buf[0], fileSize, 1, f);					\
			fclose(f);										\
		} while (0);

	std::ofstream out(output_file, std::ios_base::out | std::ios::trunc | std::ios::binary);
	std::vector<char> buf;
	buf.reserve(1024 * 20);


	MP4Writer* writer = new MP4Writer{ out, width, height };
	for (auto& frame : frames)
	{
		GetFileDataToBuf(frame.first.c_str(), buf, fileSize);
		writer->WriteFrame(&buf[0], fileSize, MP4Writer::ScalePerSecond / (frame.second == -1 ? global_fps : frame.second));
	}

	writer->End();
	out.close();

	printf("Successfully written to %s\n", output_file);

	return 0;
}
