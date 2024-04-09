/// @file MP4Writer.cpp
/// @brief A MP4 Writer
/// @author ZhuangMingyu (ZhuangMingyu@minieye.cc) QQ25360199
/// @date 2018-06-05
/// Copyright (C) 2018 - MiniEye INC.
#include "MP4Writer.h"
#include <stdint.h>
#if __GNUC__
#include <string.h>
#endif
#include <type_traits>
#include <tuple>
#include <vector>
#include <chrono>
#include <fstream>
#include <numeric>
#include <array>

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

#endif

/* Test for GCC > 3.2.0 */
#if !defined(_MSC_VER) && !defined(GCC_VERSION)
#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)
#endif


namespace _
{
	template<int BITS>
	struct _IBits
	{
		typedef typename std::remove_reference<decltype(std::get < (BITS >> 3) >(std::make_tuple(
			nullptr, uint8_t(0), uint16_t(0), uint32_t(0), uint32_t(0), uint64_t(0), uint64_t(0), uint64_t(0), uint64_t(0)))) >::type UIBits;
		typedef typename std::remove_reference<decltype(std::get < (BITS >> 3) >(std::make_tuple(
			nullptr, int8_t(0), int16_t(0), int32_t(0), int32_t(0), int64_t(0), int64_t(0), int64_t(0), int64_t(0)))) >::type IBits;
	};

	// 	//根据位数获取无符号整数类型
	// 	template<int BITS>
	// 	using UIBits = std::remove_reference_t<decltype(std::get < (BITS >> 3) >(std::make_tuple(
	// 		nullptr, uint8_t(0), uint16_t(0), uint32_t(0), uint32_t(0), uint64_t(0), uint64_t(0), uint64_t(0), uint64_t(0)))) >;
	// 	template<int BITS>
	// 	using IBits = std::remove_reference_t<decltype(std::get < (BITS >> 3) >(std::make_tuple(
	// 		nullptr, int8_t(0), int16_t(0), int32_t(0), int32_t(0), int64_t(0), int64_t(0), int64_t(0), int64_t(0)))) >;
}
//目前假定代码运行在小端机器上

//BytesReverse 网络字节序翻转
static inline uint8_t BytesReverse(uint8_t value)
{
	return value;
}
static inline uint16_t BytesReverse(uint16_t value)
{
#ifdef _WIN32
	return _byteswap_ushort(value);
#elif defined(GCC_VERSION)
	return __builtin_bswap16(value);
#else
	return (((uint16_t)value & 0x00FFu) << 8) | ((uint16_t)value >> 8);
#endif
}
static inline uint32_t BytesReverse(uint32_t value)
{
#ifdef _WIN32
	return _byteswap_ulong(value);
#elif defined(GCC_VERSION)
	return __builtin_bswap32(value);
#else
	return ((((uint32_t)(value) & 0xFF000000u) >> 24) |
		(((uint32_t)(value) & 0x00FF0000u) >> 8) |
		(((uint32_t)(value) & 0x0000FF00u) << 8) |
		(((uint32_t)(value) & 0x000000FFu) << 24));
#endif
}
static inline uint64_t BytesReverse(uint64_t value)
{
#ifdef _WIN32
	return _byteswap_uint64(value);
#elif defined(GCC_VERSION)
	return __builtin_bswap64(value);
#else
	uint32_t h = uint32_t((uint64_t)value >> 32);
	uint32_t l = (uint32_t)value;
	h = (((uint32_t)(h) >> 24) |
		(((uint32_t)(h) & 0x00FF0000u) >> 8) |
		(((uint32_t)(h) & 0x0000FF00u) << 8) |
		((uint32_t)(h) << 24));
	l = (((uint32_t)(l) >> 24) |
		(((uint32_t)(l) & 0x00FF0000u) >> 8) |
		(((uint32_t)(l) & 0x0000FF00u) << 8) |
		((uint32_t)(l) << 24));
	value = ((uint64_t)l << 32) | h;
	return value;
#endif
}

template<int BITS>
struct BigEndianUInt	//大端类型
{
	using type = typename _::_IBits<BITS>::UIBits;
	union
	{
		type memory_u = 0;
		char memory[sizeof(type)];
	};

	BigEndianUInt() = default;
	BigEndianUInt(const BigEndianUInt& other) = default;
	BigEndianUInt(type other)
	{
#if defined(_WIN32) || __BYTE_ORDER__ ==__ORDER_LITTLE_ENDIAN__
		memory_u = BytesReverse(other);
#else
		memory_u = other;
#endif
	}
	inline type get() const
	{
#if defined(_WIN32) || __BYTE_ORDER__ ==__ORDER_LITTLE_ENDIAN__
		return BytesReverse(memory_u);
#else
		return memory_u;
#endif
	}
	inline void set(type value)
	{
#if defined(_WIN32) || __BYTE_ORDER__ ==__ORDER_LITTLE_ENDIAN__
		memory_u = BytesReverse(value);
#else
		memory_u = value;
#endif
	}
};

template<int BITS>
struct BigEndianInt	//大端类型
{
	using type = typename _::_IBits<BITS>::IBits;
	using utype = typename _::_IBits<BITS>::UIBits;
	union
	{
		type memory_i = 0;
		char memory[sizeof(type)];
	};

	BigEndianInt() = default;
	BigEndianInt(const BigEndianInt& other) = default;
	BigEndianInt(type other)
	{
#if defined(_WIN32) || __BYTE_ORDER__ ==__ORDER_LITTLE_ENDIAN__
		memory_i = (type)BytesReverse((utype)other);
#else
		memory_i = other;
#endif
	}
	inline type get() const
	{
#if defined(_WIN32) || __BYTE_ORDER__ ==__ORDER_LITTLE_ENDIAN__

		return (type)BytesReverse((utype)memory_i);
#else
		return memory_i;
#endif
	}
	inline void set(type value)
	{
#if defined(_WIN32) || __BYTE_ORDER__ ==__ORDER_LITTLE_ENDIAN__

		memory_i = (type)BytesReverse((utype)value);
#else
		memory_i = value;
#endif
	}
};

#define BOXIDDEF(a,b,c,d) BOXID_##a##b##c##d = 0ULL | (*#d << 24) | (*#c << 16) | (*#b << 8) | (*#a << 0),//  CONCAT_(0x)##CONCAT2(V_,d)##CONCAT2(V_,c)##CONCAT2(V_,b)##CONCAT2(V_,a),
enum
{
	BOXIDDEF(f, t, y, p)
	BOXIDDEF(f, r, e, e)
	BOXIDDEF(m, d, a, t)
	BOXIDDEF(m, v, h, d)
	BOXIDDEF(t, k, h, d)
	BOXIDDEF(e, l, s, t)
	BOXIDDEF(m, d, h, d)
	BOXIDDEF(h, d, l, r)
	BOXIDDEF(v, m, h, d)
	BOXIDDEF(e, s, d, s)
	BOXIDDEF(s, t, t, s)
	BOXIDDEF(s, t, s, c)
	BOXIDDEF(s, t, s, z)
	BOXIDDEF(s, t, c, o)
	BOXIDDEF(d, a, t, a)
	BOXIDDEF(c, p, r, t)
	BOXIDDEF(t, r, e, f)
	BOXIDDEF(d, r, e, f)


	BOXIDDEF(m, o, o, v)
	BOXIDDEF(t, r, a, k)
	BOXIDDEF(e, d, t, s)
	BOXIDDEF(m, d, i, a)
	BOXIDDEF(m, i, n, f)
	BOXIDDEF(d, i, n, f)
	BOXIDDEF(s, t, b, l)
	BOXIDDEF(s, t, s, d)
	BOXIDDEF(m, p, 4, v)
	BOXIDDEF(u, d, t, a)
	BOXIDDEF(m, e, t, a)
	BOXIDDEF(t, r, a, f)
	BOXIDDEF(m, o, o, f)
	BOXIDDEF(m, v, e, x)
	BOXIDDEF(m, f, r, a)
};

#if GCC_VERSION < 70000
typedef const std::array<char, 5>* chars5;
#else
typedef const char(&chars5)[5];
#endif
struct CompactID32 : BigEndianUInt<32>
{
	CompactID32() = default;
	CompactID32(chars5 str) { memory_u = *(uint32_t*)str; }
	CompactID32& operator=(const char(&str)[5]) { memory_u = *(uint32_t*)str; return *this; }
};
#pragma pack(push, 4)
template<bool isContainer = false, bool isFull = false>
struct BaseBox
{
	enum { IsContainer = isContainer };
	enum { IsFull = isFull };
	enum { LoadWholeMemory = false };	//按文件中的size字段长度载入该box的内容？false表示按sizeof载入
	BigEndianUInt<32> endian_size;		//小端化后值为1时后面还跟着large size（文件大于4G），但是本应用不会出现
	CompactID32 endian_type;

	static constexpr const char DefaultContent[] = "";
	static constexpr const char ParentBox[] = "";
};

template<typename Parent>
struct WithParent	//用于assert
{
#ifdef _MSC_VER
	typedef Parent Parent;
#endif
	static_assert(Parent::IsContainer, "Parent of this box must be ContainerBox");
};

struct SimpleBox : BaseBox<>
{
};

struct ContainerBox : BaseBox<true>
{
};
struct FullBox : BaseBox<false, true>
{
	uint8_t			version = 0;
	uint8_t			flags[3] = {};
};
#if GCC_VERSION
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Waddress"
#pragma GCC diagnostic ignored "-Wnonnull"
#endif
template<typename BaseType/**/, typename ThisType, uint64_t BoxID, typename... Parents>
struct BoxDefine : BaseType
{
#ifdef _MSC_VER
	static constexpr uint64_t BoxID = BoxID;
#endif
	static constexpr void* DefaultContent = NULL;
	static_assert(sizeof(std::tuple<WithParent<Parents>...>) > 0, "");
	BoxDefine()
	{
		ThisType* pThis = (ThisType*)this;
		pThis->endian_size = sizeof(ThisType);
		pThis->endian_type.memory_u = (uint32_t)BoxID;
	}
	inline void InitByDefaultContent()
	{
		if (ThisType::DefaultContent)	//如果ThisType下定义了DefaultContent那么初始化时，覆盖到Box的内容上
		{
			enum
			{
				copySize = sizeof(ThisType::DefaultContent)
				- std::is_same<decltype(ThisType::DefaultContent), const char[sizeof(ThisType::DefaultContent)]>::value
			};
			//如果是字符串数组，那么少复制一个字节（末尾0）
			memcpy(((SimpleBox*)this) + 1, ThisType::DefaultContent, copySize > sizeof(ThisType) ? sizeof(ThisType) : copySize);
			//超出的不复制
		}
	}
};
#if GCC_VERSION
#pragma GCC diagnostic pop
#endif

#define BOXDEFINE(	\
	name		/*etc: ftyp,mdat,moov*/, \
	baseType	/*SimpleBox,FullBox,ContainerBox*/, \
	...			/*Parents*/) \
	BoxDefine<baseType, SBOX_##name, BOXID_##name, ##__VA_ARGS__>

struct SBOX_free : SimpleBox		//FreeTypeBox
{
	static constexpr uint64_t BoxID = BOXID_free;
};

struct SBOX_meta : BOXDEFINE(meta, ContainerBox/*, MP4File, SBOX_moov, SBOX_trak*/)
{
};

struct SBOX_hdlr : BOXDEFINE(hdlr, FullBox/*, SBOX_mdia, SBOX_meta*/)
{
	const uint32_t pre_defined = {};
	CompactID32 handler_type = {};
	const uint32_t _reserved[3] = {};
	char nameUTF8[16] = "VideoHandler";
};

struct SBOX_udta : BOXDEFINE(udta, SimpleBox/*, SBOX_moov, SBOX_trak*/)
{};
struct MP4File
{
	enum { IsContainer = true };
	template<int VariableCount = 1>
	struct SBOX_ftyp : BoxDefine<SimpleBox, SBOX_ftyp<VariableCount>, BOXID_ftyp, MP4File>		//FileTypeBox
	{
		CompactID32 endian_major_brand = (chars5)"isom";
		BigEndianUInt<32> endian_minor_version;
		CompactID32 endian_compatible_brands[VariableCount];

		enum { LoadWholeMemory = true };
		static const char DefaultContent[21]/* = "isom\0\0\2\0isomiso2mp41"*/; //调用InitByDefaultContent函数会自动用这个覆盖
	};

	struct SBOX_mdat : BOXDEFINE(mdat, SimpleBox, MP4File)
	{
		//很多jpeg文件在这里,连续存放
	};

	struct SBOX_moov : BOXDEFINE(moov, ContainerBox, MP4File)
	{
		struct SBOX_mvhd : BOXDEFINE(mvhd, FullBox, SBOX_moov)
		{
			///////////////////////////////////
#define MVHD_VERSION 0
#if MVHD_VERSION
			BigEndianUInt<64> creation_time;
			BigEndianUInt<64> modification_time;
			BigEndianUInt<32> timescale;
			BigEndianUInt<64> duration;
#else
			BigEndianUInt<32> creation_time;
			BigEndianUInt<32> modification_time;
			BigEndianUInt<32> timescale;
			BigEndianUInt<32> duration;
#endif
			BigEndianUInt<32> preferred_rate = 0x00010000;	//16.16定点数
			BigEndianUInt<16> preferred_volume = 0x0100;	//8.8定点数
			const uint16_t _reserved1 = {};
			const uint32_t _reserved2[2] = {};
			BigEndianInt<32> matrix[9] = { 0x00010000,0,0,
				0,0x00010000,0,
				0, 0,0x40000000 };
			const uint32_t pre_defined[6] = {};
			BigEndianUInt<32> next_track_id;
			///////////////////////////////////
		};
		struct _SBOX_meta;
		struct SBOX_trak : BOXDEFINE(trak, ContainerBox, SBOX_moov)
		{
			struct SBOX_tkhd : BOXDEFINE(tkhd, FullBox, SBOX_trak)
			{
#define TKND_VERSION 0
#if TKND_VERSION
				BigEndianUInt<64>	creation_time;
				BigEndianUInt<64>	modification_time;
				BigEndianUInt<32>	track_id;
				const uint32_t	_reserved1 = 0;
				BigEndianUInt<64>	duration;
#else
				BigEndianUInt<32>	creation_time;
				BigEndianUInt<32>	modification_time;
				BigEndianUInt<32>	track_id;
				const uint32_t	_reserved1 = 0;
				BigEndianUInt<32>	duration;
#endif
				const uint32_t	_reserved2[2] = {};
				BigEndianInt<16>	layer;
				BigEndianInt<16>	alternate_group;
				BigEndianInt<16>	volume;
				const uint16_t	_reserved3 = 0;
				BigEndianInt<32>	matrix[9] = { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 };
				BigEndianInt<16>	width;
				BigEndianInt<16>	width_fractional_part;	//小数部分
				BigEndianInt<16>	height;
				BigEndianInt<16>	height_fractional_part;
			};
			struct SBOX_edts : BOXDEFINE(edts, ContainerBox, SBOX_trak)
			{
				struct SBOX_elst : BOXDEFINE(elst, FullBox, SBOX_edts)
				{};
			};
			struct SBOX_tref : BOXDEFINE(tref, SimpleBox, SBOX_trak)
			{};
			struct _SBOX_meta;
			struct SBOX_mdia : BOXDEFINE(mdia, ContainerBox, SBOX_trak)
			{
				struct SBOX_mdhd : BOXDEFINE(mdhd, FullBox, SBOX_mdia)
				{
					///////////////////////////////////
#define MDHD_VERSION 0
#if MDHD_VERSION
					BigEndianUInt<64>	creation_time;
					BigEndianUInt<64>	modification_time;
					BigEndianUInt<32>	timescale;
					BigEndianUInt<64>	duration;
#else
					BigEndianUInt<32>	creation_time;
					BigEndianUInt<32>	modification_time;
					BigEndianUInt<32>	timescale;
					BigEndianUInt<32>	duration;
#endif
					struct Language
					{
						uint16_t a : 5;
						uint16_t b : 5;
						uint16_t c : 5;
					} language = {};
					static_assert(sizeof(Language) == 2, "");
					const uint16_t pre_defined = 0;
					///////////////////////////////////
				};
				using SBOX_hdlr = ::SBOX_hdlr;
				struct SBOX_minf : BOXDEFINE(minf, ContainerBox, SBOX_mdia)
				{
					struct SBOX_vmhd : BOXDEFINE(vmhd, FullBox, SBOX_minf)
					{
						BigEndianUInt<16> graphicsmode = {};
						BigEndianUInt<16> opcolor[3] = {};
					};
					struct SBOX_dinf : BOXDEFINE(dinf, ContainerBox, SBOX_minf)
					{
						struct SBOX_dref : BOXDEFINE(dref, FullBox, SBOX_dinf)
						{
							BigEndianUInt<32> entry_count = 1;
							struct Entry
							{
								BigEndianUInt<32> size = sizeof(Entry);
								CompactID32 type = (chars5)"url ";
								BigEndianUInt<32> flag = 1;
							} entry[1];
						};
					};
					struct SBOX_stbl : BOXDEFINE(stbl, ContainerBox, SBOX_minf)
					{
						struct SBOX_stsd : BOXDEFINE(stsd, FullBox, SBOX_stbl)
						{
							BigEndianUInt<32> entry_count = 1;
							struct VisualEntry
							{
								BigEndianUInt<32> size = sizeof(VisualEntry);
								CompactID32 type = (chars5)"mp4v";
								const uint16_t _reserved[3] = {};
								BigEndianUInt<16> data_reference_index = 1;

								const uint16_t _pre_defined1 = 0;
								const uint16_t _reserved2 = 0;
								const uint32_t _pre_defined2[3] = {};
								BigEndianUInt<16> width;
								BigEndianUInt<16> height;
								const BigEndianUInt<32> horizresolution = 0x00480000;
								const BigEndianUInt<32> vertresolution = 0x00480000;
								const uint32_t _reserved3 = 0;
								BigEndianUInt<16> frame_count = 1;
								char compressorname[32] = {};
								const BigEndianUInt<16> depth = 0x0018;
								const uint16_t pre_defined3 = 0xFFFF;

								//esds官方文档没查到
								uint8_t esdsbox[0x2C] =
								{
									0x00, 0x00, 0x00, 0x2C, 0x65, 0x73, 0x64, 0x73, 0x00, 0x00, 0x00, 0x00, 0x03, 0x80, 0x80, 0x80,
									0x1B, 0x00, 0x01, 0x00, 0x04, 0x80, 0x80, 0x80, 0x0D, 0x6C, 0x11, 0x00, 0x00, 0x00, 0x00, 0x14,
									0x3B, 0xEC, 0x00, 0x14, 0x3B, 0xEC, 0x06, 0x80, 0x80, 0x80, 0x01, 0x02
								};

								const char _reserved4[2] = {}; //zmy非官方，应当保持对齐，避免后续int跨4字节边界
							} entry[1];
						};
						struct SBOX_stts : BOXDEFINE(stts, FullBox, SBOX_stbl)
						{
							BigEndianUInt<32>	entry_count = 1;
							struct Entry
							{
								BigEndianUInt<32> sample_count;
								BigEndianUInt<32> sampe_delta;
							} entry[1];
						};
						struct SBOX_stsc : BOXDEFINE(stsc, FullBox, SBOX_stbl)
						{
							BigEndianUInt<32>	entry_count = 1;
							struct Entry
							{
								BigEndianUInt<32> first_chunk = 1;
								BigEndianUInt<32> samples_per_chunk = 600;
								BigEndianUInt<32> sample_description_index = 1;
							} entry[1];
						};
						struct SBOX_stsz : BOXDEFINE(stsz, FullBox, SBOX_stbl)
						{
							BigEndianUInt<32> sample_size = 0;
							BigEndianUInt<32> sample_count = 1;

							BigEndianUInt<32> sizePerSample[1];
						};
						struct SBOX_stco : BOXDEFINE(stco, FullBox, SBOX_stbl)
						{
							BigEndianUInt<32>	entry_count = 1;
							BigEndianUInt<32>	offsetPerChunk[1];
						};
					};
				};
			};
		};
	};
};


template<int VariableCount>
using SBOX_ftyp = MP4File::SBOX_ftyp<VariableCount>;
using SBOX_mdat = MP4File::SBOX_mdat;
using SBOX_moov = MP4File::SBOX_moov;
using SBOX_mvhd = SBOX_moov::SBOX_mvhd;
using SBOX_trak = SBOX_moov::SBOX_trak;
using SBOX_tkhd = SBOX_trak::SBOX_tkhd;
using SBOX_edts = SBOX_trak::SBOX_edts;
using SBOX_elst = SBOX_edts::SBOX_elst;
using SBOX_tref = SBOX_trak::SBOX_tref;
using SBOX_mdia = SBOX_trak::SBOX_mdia;
using SBOX_mdhd = SBOX_mdia::SBOX_mdhd;
using SBOX_minf = SBOX_mdia::SBOX_minf;
using SBOX_vmhd = SBOX_minf::SBOX_vmhd;
using SBOX_dinf = SBOX_minf::SBOX_dinf;
using SBOX_dref = SBOX_dinf::SBOX_dref;
using SBOX_stbl = SBOX_minf::SBOX_stbl;
using SBOX_stsd = SBOX_stbl::SBOX_stsd;
using SBOX_stts = SBOX_stbl::SBOX_stts;
using SBOX_stsc = SBOX_stbl::SBOX_stsc;
using SBOX_stsz = SBOX_stbl::SBOX_stsz;
using SBOX_stco = SBOX_stbl::SBOX_stco;

template<>
const char MP4File::SBOX_ftyp<3>::DefaultContent[21] = "isom\0\0\2\0isomiso2mp41";

#pragma pack(pop)

template<typename SBox>
struct SizeGuard
{
	SBox& box;
	char*& pBuffer;
	SizeGuard(SBox& box, char*& pBuffer) : box(box), pBuffer(pBuffer) {};
	~SizeGuard()
	{
		box.endian_size = uint32_t(pBuffer - (char*)&box);
	}
};

enum { PosOfMDAT = sizeof(MP4File::SBOX_ftyp<3>) };

int MP4Writer::Begin(std::ostream& os, int width, int height)
{
	out = &os;
	mWidth = (unsigned short)width;
	mHeight = (unsigned short)height;

	mFrameInfos.clear();
	mSizeWritedToStream = 0;

	{
		MP4File::SBOX_ftyp<3> ftyp{}; ftyp.InitByDefaultContent();
		out->write((const char*)&ftyp, sizeof(ftyp));
		mSizeWritedToStream += (unsigned int)sizeof(ftyp);
	}
	{
		//要求PosOfMDAT位于当前写入位置，否则End部分的代码不能正确引用图像数据
		if (PosOfMDAT != mSizeWritedToStream)
			__debugbreak();
		MP4File::SBOX_mdat mdat{}; mdat.InitByDefaultContent();
		out->write((const char*)&mdat, sizeof(mdat));
		mSizeWritedToStream += (unsigned int)sizeof(mdat);
	}
	return 0;
}

int MP4Writer::WriteFrame(void* buf, int size, int duration)
{
	out->write((char*)buf, size);
	mSizeWritedToStream += size;

	SFrameInfo fi; fi.size = size; fi.duration = duration;
	mFrameInfos.emplace_back(fi);
	return 0;
}

int MP4Writer::End()
{
	{	//重新写入mdat（刷新了它的大小）
		out->seekp(PosOfMDAT, std::ios::beg);
		MP4File::SBOX_mdat mdat{}; mdat.InitByDefaultContent();
		mdat.endian_size = mSizeWritedToStream - PosOfMDAT;
		out->write((const char*)&mdat, sizeof(mdat));
	}
	//回到文件末尾
	out->seekp(0, std::ios::end);

	unsigned int duration = std::accumulate(mFrameInfos.begin(), mFrameInfos.end(), 0, [](int& s, SFrameInfo& fi) { return s + fi.duration; });

	std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds> tp
		= std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
	uint32_t mp4time = (uint32_t)(tp.time_since_epoch().count() + 2082844800);//Jan-1-1904（系统时间-2082844800）的mp4时间为0

																			  //估算moov缓冲区大小
	std::vector<char> _moovBuffer; _moovBuffer.resize(4096 + mFrameInfos.size() * 20);
	char* pMoovBufferEnd = &_moovBuffer[0];

	//缓冲区指向下一个Box，并且在这个SizeGard变量离开区域时设置它的size
#define NextBox_SetSizeWhenOutOfScope(name) \
		*new (pMoovBufferEnd) typename std::remove_reference<decltype(name)>::type; \
		name.InitByDefaultContent(); \
		pMoovBufferEnd += sizeof(name); \
		if ((intptr_t)pMoovBufferEnd & 3) puts(#name); \
		SizeGuard<typename std::remove_reference<decltype(name)>::type> sg##name{ name, pMoovBufferEnd};

	{
		MP4File::SBOX_moov& moov = NextBox_SetSizeWhenOutOfScope(moov);
		{
			SBOX_moov::SBOX_mvhd& mvhd = NextBox_SetSizeWhenOutOfScope(mvhd);

			mvhd.creation_time = mp4time;
			mvhd.modification_time = mp4time;
			mvhd.timescale = ScalePerSecond;
			mvhd.duration = duration;
			mvhd.next_track_id = 2;
		}
		{
			SBOX_moov::SBOX_trak& trak = NextBox_SetSizeWhenOutOfScope(trak);
			{
				SBOX_trak::SBOX_tkhd& tkhd = NextBox_SetSizeWhenOutOfScope(tkhd);
				tkhd.flags[2] = 3; //enabled & present

				tkhd.creation_time = mp4time;
				tkhd.modification_time = mp4time;
				tkhd.track_id = 1;
				tkhd.duration = duration;
				tkhd.width = (uint16_t)mWidth;
				tkhd.height = (uint16_t)mHeight;

				memcpy(pMoovBufferEnd, "\x00\x00\x00\x24\x65\x64\x74\x73\x00\x00\x00\x1C\x65\x6C\x73\x74"
					"\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x13\x88\x00\x00\x00\x00"
					"\x00\x01\x00\x00", 0x24);
				pMoovBufferEnd += 0x24;
			}
			{
				SBOX_trak::SBOX_mdia& mdia = NextBox_SetSizeWhenOutOfScope(mdia);
				{
					SBOX_mdia::SBOX_mdhd& mdhd = NextBox_SetSizeWhenOutOfScope(mdhd);
					mdhd.creation_time = mp4time;
					mdhd.modification_time = mp4time;
					mdhd.timescale = ScalePerSecond;
					mdhd.duration = duration;
				}
				{
					SBOX_mdia::SBOX_hdlr& hdlr = NextBox_SetSizeWhenOutOfScope(hdlr);
					hdlr.handler_type = "vide";
				}
				{
					SBOX_mdia::SBOX_minf& minf = NextBox_SetSizeWhenOutOfScope(minf);
					{
						SBOX_minf::SBOX_vmhd& vmhd = NextBox_SetSizeWhenOutOfScope(vmhd);
						vmhd.flags[2] = 1;
					}
					{
						SBOX_minf::SBOX_dinf& dinf = NextBox_SetSizeWhenOutOfScope(dinf);
						{
							SBOX_dinf::SBOX_dref& dref = NextBox_SetSizeWhenOutOfScope(dref);
						}
					}
					{
						SBOX_minf::SBOX_stbl& stbl = NextBox_SetSizeWhenOutOfScope(stbl);
						{
							SBOX_stbl::SBOX_stsd& stsd = NextBox_SetSizeWhenOutOfScope(stsd);
							stsd.entry_count = 1;
							stsd.entry[0].width = mWidth;
							stsd.entry[0].height = mHeight;
							//zmy由于上面的数组不是4字节对齐的，所以这边补空（似乎官方没有，但这样也没有问题）
							//zmy现在在结构体定义中直接补齐// (intptr_t&)pBuffer = ((intptr_t)pBuffer + 3) &~3;
						}
						{
							SBOX_stbl::SBOX_stts& stts = NextBox_SetSizeWhenOutOfScope(stts);

							stts.entry_count = 0;
							auto* pEntry = stts.entry - 1;
							int lastDuration = 0;
							for (SFrameInfo& fi : mFrameInfos)
							{
								if (fi.duration != lastDuration)
								{
									++pEntry;
									stts.entry_count.set(stts.entry_count.get() + 1);
									pEntry->sampe_delta = lastDuration = fi.duration;
									pEntry->sample_count = 1;
								}
								else
									pEntry->sample_count.set(pEntry->sample_count.get() + 1);
							}
							pMoovBufferEnd += ((int)stts.entry_count.get() - 1) * sizeof(stts.entry[0]);
						}
						{
							SBOX_stbl::SBOX_stsc& stsc = NextBox_SetSizeWhenOutOfScope(stsc);
							stsc.entry[0].samples_per_chunk = (uint32_t)mFrameInfos.size();
						}
						{
							SBOX_stbl::SBOX_stsz& stsz = NextBox_SetSizeWhenOutOfScope(stsz);
							stsz.sample_count = (uint32_t)mFrameInfos.size();
							pMoovBufferEnd += ((int)mFrameInfos.size() - 1) * sizeof(stsz.sizePerSample[0]);
							BigEndianUInt<32>* pSampleSize = stsz.sizePerSample;
							for (SFrameInfo& fi : mFrameInfos)
								*pSampleSize++ = fi.size;
						}
						{
							SBOX_stbl::SBOX_stco& stco = NextBox_SetSizeWhenOutOfScope(stco);
							stco.entry_count = 1;
							stco.offsetPerChunk[0] = PosOfMDAT + sizeof(MP4File::SBOX_mdat);
						}
					}
				}
			}
		}
	}



	out->write(&_moovBuffer[0], pMoovBufferEnd - &_moovBuffer[0]);
	mSizeWritedToStream += int(pMoovBufferEnd - &_moovBuffer[0]);

	Clear();
	return 0;
}

void MP4Writer::Clear() noexcept
{
	out = NULL;
	mWidth = -1;
	mHeight = -1;
	mFrameInfos.clear();
	mSizeWritedToStream = 0;
}
