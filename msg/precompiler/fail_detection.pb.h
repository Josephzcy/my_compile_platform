// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: fail_detection.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_fail_5fdetection_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_fail_5fdetection_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3012000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3012003 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include "geometry.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_fail_5fdetection_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_fail_5fdetection_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_fail_5fdetection_2eproto;
namespace minieye {
class ImageFail;
class ImageFailDefaultTypeInternal;
extern ImageFailDefaultTypeInternal _ImageFail_default_instance_;
class ImageFailStatus;
class ImageFailStatusDefaultTypeInternal;
extern ImageFailStatusDefaultTypeInternal _ImageFailStatus_default_instance_;
}  // namespace minieye
PROTOBUF_NAMESPACE_OPEN
template<> ::minieye::ImageFail* Arena::CreateMaybeMessage<::minieye::ImageFail>(Arena*);
template<> ::minieye::ImageFailStatus* Arena::CreateMaybeMessage<::minieye::ImageFailStatus>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace minieye {

enum ImageFailStatus_ImgFailTypes : int {
  ImageFailStatus_ImgFailTypes_IMG_NORMAL = 0,
  ImageFailStatus_ImgFailTypes_IMG_BLUR = 1,
  ImageFailStatus_ImgFailTypes_IMG_SHELTER = 2,
  ImageFailStatus_ImgFailTypes_IMG_FREEZE = 4,
  ImageFailStatus_ImgFailTypes_IMG_GLARE = 8,
  ImageFailStatus_ImgFailTypes_ImageFailStatus_ImgFailTypes_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  ImageFailStatus_ImgFailTypes_ImageFailStatus_ImgFailTypes_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool ImageFailStatus_ImgFailTypes_IsValid(int value);
constexpr ImageFailStatus_ImgFailTypes ImageFailStatus_ImgFailTypes_ImgFailTypes_MIN = ImageFailStatus_ImgFailTypes_IMG_NORMAL;
constexpr ImageFailStatus_ImgFailTypes ImageFailStatus_ImgFailTypes_ImgFailTypes_MAX = ImageFailStatus_ImgFailTypes_IMG_GLARE;
constexpr int ImageFailStatus_ImgFailTypes_ImgFailTypes_ARRAYSIZE = ImageFailStatus_ImgFailTypes_ImgFailTypes_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ImageFailStatus_ImgFailTypes_descriptor();
template<typename T>
inline const std::string& ImageFailStatus_ImgFailTypes_Name(T enum_t_value) {
  static_assert(::std::is_same<T, ImageFailStatus_ImgFailTypes>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function ImageFailStatus_ImgFailTypes_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    ImageFailStatus_ImgFailTypes_descriptor(), enum_t_value);
}
inline bool ImageFailStatus_ImgFailTypes_Parse(
    const std::string& name, ImageFailStatus_ImgFailTypes* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<ImageFailStatus_ImgFailTypes>(
    ImageFailStatus_ImgFailTypes_descriptor(), name, value);
}
enum ImageFailStatus_ImgFailLevel : int {
  ImageFailStatus_ImgFailLevel_FAIL_NONE = 0,
  ImageFailStatus_ImgFailLevel_FAIL_MILD = 1,
  ImageFailStatus_ImgFailLevel_FAIL_MODERATE = 2,
  ImageFailStatus_ImgFailLevel_FAIL_SEVERE = 3,
  ImageFailStatus_ImgFailLevel_ImageFailStatus_ImgFailLevel_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  ImageFailStatus_ImgFailLevel_ImageFailStatus_ImgFailLevel_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool ImageFailStatus_ImgFailLevel_IsValid(int value);
constexpr ImageFailStatus_ImgFailLevel ImageFailStatus_ImgFailLevel_ImgFailLevel_MIN = ImageFailStatus_ImgFailLevel_FAIL_NONE;
constexpr ImageFailStatus_ImgFailLevel ImageFailStatus_ImgFailLevel_ImgFailLevel_MAX = ImageFailStatus_ImgFailLevel_FAIL_SEVERE;
constexpr int ImageFailStatus_ImgFailLevel_ImgFailLevel_ARRAYSIZE = ImageFailStatus_ImgFailLevel_ImgFailLevel_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* ImageFailStatus_ImgFailLevel_descriptor();
template<typename T>
inline const std::string& ImageFailStatus_ImgFailLevel_Name(T enum_t_value) {
  static_assert(::std::is_same<T, ImageFailStatus_ImgFailLevel>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function ImageFailStatus_ImgFailLevel_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    ImageFailStatus_ImgFailLevel_descriptor(), enum_t_value);
}
inline bool ImageFailStatus_ImgFailLevel_Parse(
    const std::string& name, ImageFailStatus_ImgFailLevel* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<ImageFailStatus_ImgFailLevel>(
    ImageFailStatus_ImgFailLevel_descriptor(), name, value);
}
// ===================================================================

class ImageFailStatus PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:minieye.ImageFailStatus) */ {
 public:
  inline ImageFailStatus() : ImageFailStatus(nullptr) {};
  virtual ~ImageFailStatus();

  ImageFailStatus(const ImageFailStatus& from);
  ImageFailStatus(ImageFailStatus&& from) noexcept
    : ImageFailStatus() {
    *this = ::std::move(from);
  }

  inline ImageFailStatus& operator=(const ImageFailStatus& from) {
    CopyFrom(from);
    return *this;
  }
  inline ImageFailStatus& operator=(ImageFailStatus&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const ImageFailStatus& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ImageFailStatus* internal_default_instance() {
    return reinterpret_cast<const ImageFailStatus*>(
               &_ImageFailStatus_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(ImageFailStatus& a, ImageFailStatus& b) {
    a.Swap(&b);
  }
  inline void Swap(ImageFailStatus* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(ImageFailStatus* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline ImageFailStatus* New() const final {
    return CreateMaybeMessage<ImageFailStatus>(nullptr);
  }

  ImageFailStatus* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<ImageFailStatus>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const ImageFailStatus& from);
  void MergeFrom(const ImageFailStatus& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ImageFailStatus* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "minieye.ImageFailStatus";
  }
  protected:
  explicit ImageFailStatus(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_fail_5fdetection_2eproto);
    return ::descriptor_table_fail_5fdetection_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef ImageFailStatus_ImgFailTypes ImgFailTypes;
  static constexpr ImgFailTypes IMG_NORMAL =
    ImageFailStatus_ImgFailTypes_IMG_NORMAL;
  static constexpr ImgFailTypes IMG_BLUR =
    ImageFailStatus_ImgFailTypes_IMG_BLUR;
  static constexpr ImgFailTypes IMG_SHELTER =
    ImageFailStatus_ImgFailTypes_IMG_SHELTER;
  static constexpr ImgFailTypes IMG_FREEZE =
    ImageFailStatus_ImgFailTypes_IMG_FREEZE;
  static constexpr ImgFailTypes IMG_GLARE =
    ImageFailStatus_ImgFailTypes_IMG_GLARE;
  static inline bool ImgFailTypes_IsValid(int value) {
    return ImageFailStatus_ImgFailTypes_IsValid(value);
  }
  static constexpr ImgFailTypes ImgFailTypes_MIN =
    ImageFailStatus_ImgFailTypes_ImgFailTypes_MIN;
  static constexpr ImgFailTypes ImgFailTypes_MAX =
    ImageFailStatus_ImgFailTypes_ImgFailTypes_MAX;
  static constexpr int ImgFailTypes_ARRAYSIZE =
    ImageFailStatus_ImgFailTypes_ImgFailTypes_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  ImgFailTypes_descriptor() {
    return ImageFailStatus_ImgFailTypes_descriptor();
  }
  template<typename T>
  static inline const std::string& ImgFailTypes_Name(T enum_t_value) {
    static_assert(::std::is_same<T, ImgFailTypes>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function ImgFailTypes_Name.");
    return ImageFailStatus_ImgFailTypes_Name(enum_t_value);
  }
  static inline bool ImgFailTypes_Parse(const std::string& name,
      ImgFailTypes* value) {
    return ImageFailStatus_ImgFailTypes_Parse(name, value);
  }

  typedef ImageFailStatus_ImgFailLevel ImgFailLevel;
  static constexpr ImgFailLevel FAIL_NONE =
    ImageFailStatus_ImgFailLevel_FAIL_NONE;
  static constexpr ImgFailLevel FAIL_MILD =
    ImageFailStatus_ImgFailLevel_FAIL_MILD;
  static constexpr ImgFailLevel FAIL_MODERATE =
    ImageFailStatus_ImgFailLevel_FAIL_MODERATE;
  static constexpr ImgFailLevel FAIL_SEVERE =
    ImageFailStatus_ImgFailLevel_FAIL_SEVERE;
  static inline bool ImgFailLevel_IsValid(int value) {
    return ImageFailStatus_ImgFailLevel_IsValid(value);
  }
  static constexpr ImgFailLevel ImgFailLevel_MIN =
    ImageFailStatus_ImgFailLevel_ImgFailLevel_MIN;
  static constexpr ImgFailLevel ImgFailLevel_MAX =
    ImageFailStatus_ImgFailLevel_ImgFailLevel_MAX;
  static constexpr int ImgFailLevel_ARRAYSIZE =
    ImageFailStatus_ImgFailLevel_ImgFailLevel_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  ImgFailLevel_descriptor() {
    return ImageFailStatus_ImgFailLevel_descriptor();
  }
  template<typename T>
  static inline const std::string& ImgFailLevel_Name(T enum_t_value) {
    static_assert(::std::is_same<T, ImgFailLevel>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function ImgFailLevel_Name.");
    return ImageFailStatus_ImgFailLevel_Name(enum_t_value);
  }
  static inline bool ImgFailLevel_Parse(const std::string& name,
      ImgFailLevel* value) {
    return ImageFailStatus_ImgFailLevel_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kRegionsFieldNumber = 4,
    kFailTypeFieldNumber = 1,
    kScoreFieldNumber = 2,
    kLevelFieldNumber = 3,
  };
  // repeated .perception.common.Polygon regions = 4;
  int regions_size() const;
  private:
  int _internal_regions_size() const;
  public:
  void clear_regions();
  ::perception::common::Polygon* mutable_regions(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::perception::common::Polygon >*
      mutable_regions();
  private:
  const ::perception::common::Polygon& _internal_regions(int index) const;
  ::perception::common::Polygon* _internal_add_regions();
  public:
  const ::perception::common::Polygon& regions(int index) const;
  ::perception::common::Polygon* add_regions();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::perception::common::Polygon >&
      regions() const;

  // int32 fail_type = 1;
  void clear_fail_type();
  ::PROTOBUF_NAMESPACE_ID::int32 fail_type() const;
  void set_fail_type(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_fail_type() const;
  void _internal_set_fail_type(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // float score = 2;
  void clear_score();
  float score() const;
  void set_score(float value);
  private:
  float _internal_score() const;
  void _internal_set_score(float value);
  public:

  // int32 level = 3;
  void clear_level();
  ::PROTOBUF_NAMESPACE_ID::int32 level() const;
  void set_level(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_level() const;
  void _internal_set_level(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:minieye.ImageFailStatus)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::perception::common::Polygon > regions_;
  ::PROTOBUF_NAMESPACE_ID::int32 fail_type_;
  float score_;
  ::PROTOBUF_NAMESPACE_ID::int32 level_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_fail_5fdetection_2eproto;
};
// -------------------------------------------------------------------

class ImageFail PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:minieye.ImageFail) */ {
 public:
  inline ImageFail() : ImageFail(nullptr) {};
  virtual ~ImageFail();

  ImageFail(const ImageFail& from);
  ImageFail(ImageFail&& from) noexcept
    : ImageFail() {
    *this = ::std::move(from);
  }

  inline ImageFail& operator=(const ImageFail& from) {
    CopyFrom(from);
    return *this;
  }
  inline ImageFail& operator=(ImageFail&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const ImageFail& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ImageFail* internal_default_instance() {
    return reinterpret_cast<const ImageFail*>(
               &_ImageFail_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(ImageFail& a, ImageFail& b) {
    a.Swap(&b);
  }
  inline void Swap(ImageFail* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(ImageFail* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline ImageFail* New() const final {
    return CreateMaybeMessage<ImageFail>(nullptr);
  }

  ImageFail* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<ImageFail>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const ImageFail& from);
  void MergeFrom(const ImageFail& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(ImageFail* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "minieye.ImageFail";
  }
  protected:
  explicit ImageFail(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_fail_5fdetection_2eproto);
    return ::descriptor_table_fail_5fdetection_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kFailStatusFieldNumber = 3,
    kCamIdFieldNumber = 1,
    kFrameIdFieldNumber = 2,
  };
  // repeated .minieye.ImageFailStatus fail_status = 3;
  int fail_status_size() const;
  private:
  int _internal_fail_status_size() const;
  public:
  void clear_fail_status();
  ::minieye::ImageFailStatus* mutable_fail_status(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::minieye::ImageFailStatus >*
      mutable_fail_status();
  private:
  const ::minieye::ImageFailStatus& _internal_fail_status(int index) const;
  ::minieye::ImageFailStatus* _internal_add_fail_status();
  public:
  const ::minieye::ImageFailStatus& fail_status(int index) const;
  ::minieye::ImageFailStatus* add_fail_status();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::minieye::ImageFailStatus >&
      fail_status() const;

  // int32 cam_id = 1;
  void clear_cam_id();
  ::PROTOBUF_NAMESPACE_ID::int32 cam_id() const;
  void set_cam_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_cam_id() const;
  void _internal_set_cam_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 frame_id = 2;
  void clear_frame_id();
  ::PROTOBUF_NAMESPACE_ID::int32 frame_id() const;
  void set_frame_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_frame_id() const;
  void _internal_set_frame_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:minieye.ImageFail)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::minieye::ImageFailStatus > fail_status_;
  ::PROTOBUF_NAMESPACE_ID::int32 cam_id_;
  ::PROTOBUF_NAMESPACE_ID::int32 frame_id_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_fail_5fdetection_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// ImageFailStatus

// int32 fail_type = 1;
inline void ImageFailStatus::clear_fail_type() {
  fail_type_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 ImageFailStatus::_internal_fail_type() const {
  return fail_type_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 ImageFailStatus::fail_type() const {
  // @@protoc_insertion_point(field_get:minieye.ImageFailStatus.fail_type)
  return _internal_fail_type();
}
inline void ImageFailStatus::_internal_set_fail_type(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  fail_type_ = value;
}
inline void ImageFailStatus::set_fail_type(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_fail_type(value);
  // @@protoc_insertion_point(field_set:minieye.ImageFailStatus.fail_type)
}

// float score = 2;
inline void ImageFailStatus::clear_score() {
  score_ = 0;
}
inline float ImageFailStatus::_internal_score() const {
  return score_;
}
inline float ImageFailStatus::score() const {
  // @@protoc_insertion_point(field_get:minieye.ImageFailStatus.score)
  return _internal_score();
}
inline void ImageFailStatus::_internal_set_score(float value) {
  
  score_ = value;
}
inline void ImageFailStatus::set_score(float value) {
  _internal_set_score(value);
  // @@protoc_insertion_point(field_set:minieye.ImageFailStatus.score)
}

// int32 level = 3;
inline void ImageFailStatus::clear_level() {
  level_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 ImageFailStatus::_internal_level() const {
  return level_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 ImageFailStatus::level() const {
  // @@protoc_insertion_point(field_get:minieye.ImageFailStatus.level)
  return _internal_level();
}
inline void ImageFailStatus::_internal_set_level(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  level_ = value;
}
inline void ImageFailStatus::set_level(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_level(value);
  // @@protoc_insertion_point(field_set:minieye.ImageFailStatus.level)
}

// repeated .perception.common.Polygon regions = 4;
inline int ImageFailStatus::_internal_regions_size() const {
  return regions_.size();
}
inline int ImageFailStatus::regions_size() const {
  return _internal_regions_size();
}
inline ::perception::common::Polygon* ImageFailStatus::mutable_regions(int index) {
  // @@protoc_insertion_point(field_mutable:minieye.ImageFailStatus.regions)
  return regions_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::perception::common::Polygon >*
ImageFailStatus::mutable_regions() {
  // @@protoc_insertion_point(field_mutable_list:minieye.ImageFailStatus.regions)
  return &regions_;
}
inline const ::perception::common::Polygon& ImageFailStatus::_internal_regions(int index) const {
  return regions_.Get(index);
}
inline const ::perception::common::Polygon& ImageFailStatus::regions(int index) const {
  // @@protoc_insertion_point(field_get:minieye.ImageFailStatus.regions)
  return _internal_regions(index);
}
inline ::perception::common::Polygon* ImageFailStatus::_internal_add_regions() {
  return regions_.Add();
}
inline ::perception::common::Polygon* ImageFailStatus::add_regions() {
  // @@protoc_insertion_point(field_add:minieye.ImageFailStatus.regions)
  return _internal_add_regions();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::perception::common::Polygon >&
ImageFailStatus::regions() const {
  // @@protoc_insertion_point(field_list:minieye.ImageFailStatus.regions)
  return regions_;
}

// -------------------------------------------------------------------

// ImageFail

// int32 cam_id = 1;
inline void ImageFail::clear_cam_id() {
  cam_id_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 ImageFail::_internal_cam_id() const {
  return cam_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 ImageFail::cam_id() const {
  // @@protoc_insertion_point(field_get:minieye.ImageFail.cam_id)
  return _internal_cam_id();
}
inline void ImageFail::_internal_set_cam_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  cam_id_ = value;
}
inline void ImageFail::set_cam_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_cam_id(value);
  // @@protoc_insertion_point(field_set:minieye.ImageFail.cam_id)
}

// int32 frame_id = 2;
inline void ImageFail::clear_frame_id() {
  frame_id_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 ImageFail::_internal_frame_id() const {
  return frame_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 ImageFail::frame_id() const {
  // @@protoc_insertion_point(field_get:minieye.ImageFail.frame_id)
  return _internal_frame_id();
}
inline void ImageFail::_internal_set_frame_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  frame_id_ = value;
}
inline void ImageFail::set_frame_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_frame_id(value);
  // @@protoc_insertion_point(field_set:minieye.ImageFail.frame_id)
}

// repeated .minieye.ImageFailStatus fail_status = 3;
inline int ImageFail::_internal_fail_status_size() const {
  return fail_status_.size();
}
inline int ImageFail::fail_status_size() const {
  return _internal_fail_status_size();
}
inline void ImageFail::clear_fail_status() {
  fail_status_.Clear();
}
inline ::minieye::ImageFailStatus* ImageFail::mutable_fail_status(int index) {
  // @@protoc_insertion_point(field_mutable:minieye.ImageFail.fail_status)
  return fail_status_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::minieye::ImageFailStatus >*
ImageFail::mutable_fail_status() {
  // @@protoc_insertion_point(field_mutable_list:minieye.ImageFail.fail_status)
  return &fail_status_;
}
inline const ::minieye::ImageFailStatus& ImageFail::_internal_fail_status(int index) const {
  return fail_status_.Get(index);
}
inline const ::minieye::ImageFailStatus& ImageFail::fail_status(int index) const {
  // @@protoc_insertion_point(field_get:minieye.ImageFail.fail_status)
  return _internal_fail_status(index);
}
inline ::minieye::ImageFailStatus* ImageFail::_internal_add_fail_status() {
  return fail_status_.Add();
}
inline ::minieye::ImageFailStatus* ImageFail::add_fail_status() {
  // @@protoc_insertion_point(field_add:minieye.ImageFail.fail_status)
  return _internal_add_fail_status();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::minieye::ImageFailStatus >&
ImageFail::fail_status() const {
  // @@protoc_insertion_point(field_list:minieye.ImageFail.fail_status)
  return fail_status_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace minieye

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::minieye::ImageFailStatus_ImgFailTypes> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::minieye::ImageFailStatus_ImgFailTypes>() {
  return ::minieye::ImageFailStatus_ImgFailTypes_descriptor();
}
template <> struct is_proto_enum< ::minieye::ImageFailStatus_ImgFailLevel> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::minieye::ImageFailStatus_ImgFailLevel>() {
  return ::minieye::ImageFailStatus_ImgFailLevel_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_fail_5fdetection_2eproto
