// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: freespace_obstacles.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_freespace_5fobstacles_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_freespace_5fobstacles_2eproto

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
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_freespace_5fobstacles_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_freespace_5fobstacles_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[3]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_freespace_5fobstacles_2eproto;
namespace perception {
class FreespaceObstacle;
class FreespaceObstacleDefaultTypeInternal;
extern FreespaceObstacleDefaultTypeInternal _FreespaceObstacle_default_instance_;
class FreespaceObstacles;
class FreespaceObstaclesDefaultTypeInternal;
extern FreespaceObstaclesDefaultTypeInternal _FreespaceObstacles_default_instance_;
class Point2D;
class Point2DDefaultTypeInternal;
extern Point2DDefaultTypeInternal _Point2D_default_instance_;
}  // namespace perception
PROTOBUF_NAMESPACE_OPEN
template<> ::perception::FreespaceObstacle* Arena::CreateMaybeMessage<::perception::FreespaceObstacle>(Arena*);
template<> ::perception::FreespaceObstacles* Arena::CreateMaybeMessage<::perception::FreespaceObstacles>(Arena*);
template<> ::perception::Point2D* Arena::CreateMaybeMessage<::perception::Point2D>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace perception {

enum FreespaceObstacle_Type : int {
  FreespaceObstacle_Type_UNKNOW = 0,
  FreespaceObstacle_Type_DYNAMIC = 1,
  FreespaceObstacle_Type_STATIC = 2,
  FreespaceObstacle_Type_FreespaceObstacle_Type_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  FreespaceObstacle_Type_FreespaceObstacle_Type_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool FreespaceObstacle_Type_IsValid(int value);
constexpr FreespaceObstacle_Type FreespaceObstacle_Type_Type_MIN = FreespaceObstacle_Type_UNKNOW;
constexpr FreespaceObstacle_Type FreespaceObstacle_Type_Type_MAX = FreespaceObstacle_Type_STATIC;
constexpr int FreespaceObstacle_Type_Type_ARRAYSIZE = FreespaceObstacle_Type_Type_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* FreespaceObstacle_Type_descriptor();
template<typename T>
inline const std::string& FreespaceObstacle_Type_Name(T enum_t_value) {
  static_assert(::std::is_same<T, FreespaceObstacle_Type>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function FreespaceObstacle_Type_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    FreespaceObstacle_Type_descriptor(), enum_t_value);
}
inline bool FreespaceObstacle_Type_Parse(
    const std::string& name, FreespaceObstacle_Type* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<FreespaceObstacle_Type>(
    FreespaceObstacle_Type_descriptor(), name, value);
}
enum Class : int {
  kVision = 0,
  kUss = 1,
  Class_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  Class_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool Class_IsValid(int value);
constexpr Class Class_MIN = kVision;
constexpr Class Class_MAX = kUss;
constexpr int Class_ARRAYSIZE = Class_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Class_descriptor();
template<typename T>
inline const std::string& Class_Name(T enum_t_value) {
  static_assert(::std::is_same<T, Class>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function Class_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    Class_descriptor(), enum_t_value);
}
inline bool Class_Parse(
    const std::string& name, Class* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<Class>(
    Class_descriptor(), name, value);
}
enum Category : int {
  kNormal = 0,
  kNoCrash = 1,
  kRod = 2,
  kVehicle = 3,
  kWheelTwo = 4,
  kRider = 5,
  kPed = 6,
  kCone = 7,
  kWsign = 8,
  kStone = 9,
  kPiles = 10,
  kLock = 11,
  kPillar = 12,
  Category_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  Category_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool Category_IsValid(int value);
constexpr Category Category_MIN = kNormal;
constexpr Category Category_MAX = kPillar;
constexpr int Category_ARRAYSIZE = Category_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Category_descriptor();
template<typename T>
inline const std::string& Category_Name(T enum_t_value) {
  static_assert(::std::is_same<T, Category>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function Category_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    Category_descriptor(), enum_t_value);
}
inline bool Category_Parse(
    const std::string& name, Category* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<Category>(
    Category_descriptor(), name, value);
}
enum UssClass : int {
  kOthers = 0,
  kLongRadarFL = 1,
  kLongRadarFR = 2,
  kLongRadarBL = 3,
  kLongRadarBR = 4,
  kShortRadarFL = 5,
  kShortRadarFLM = 6,
  kShortRadarFRM = 7,
  kShortRadarFR = 8,
  kShortRadarBL = 9,
  kShortRadarBLM = 10,
  kShortRadarBRM = 11,
  kShortRadarBR = 12,
  kFusion = 13,
  UssClass_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  UssClass_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool UssClass_IsValid(int value);
constexpr UssClass UssClass_MIN = kOthers;
constexpr UssClass UssClass_MAX = kFusion;
constexpr int UssClass_ARRAYSIZE = UssClass_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* UssClass_descriptor();
template<typename T>
inline const std::string& UssClass_Name(T enum_t_value) {
  static_assert(::std::is_same<T, UssClass>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function UssClass_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    UssClass_descriptor(), enum_t_value);
}
inline bool UssClass_Parse(
    const std::string& name, UssClass* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<UssClass>(
    UssClass_descriptor(), name, value);
}
// ===================================================================

class Point2D PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:perception.Point2D) */ {
 public:
  inline Point2D() : Point2D(nullptr) {};
  virtual ~Point2D();

  Point2D(const Point2D& from);
  Point2D(Point2D&& from) noexcept
    : Point2D() {
    *this = ::std::move(from);
  }

  inline Point2D& operator=(const Point2D& from) {
    CopyFrom(from);
    return *this;
  }
  inline Point2D& operator=(Point2D&& from) noexcept {
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
  static const Point2D& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const Point2D* internal_default_instance() {
    return reinterpret_cast<const Point2D*>(
               &_Point2D_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(Point2D& a, Point2D& b) {
    a.Swap(&b);
  }
  inline void Swap(Point2D* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(Point2D* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline Point2D* New() const final {
    return CreateMaybeMessage<Point2D>(nullptr);
  }

  Point2D* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<Point2D>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const Point2D& from);
  void MergeFrom(const Point2D& from);
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
  void InternalSwap(Point2D* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "perception.Point2D";
  }
  protected:
  explicit Point2D(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_freespace_5fobstacles_2eproto);
    return ::descriptor_table_freespace_5fobstacles_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kXFieldNumber = 1,
    kYFieldNumber = 2,
  };
  // float x = 1;
  void clear_x();
  float x() const;
  void set_x(float value);
  private:
  float _internal_x() const;
  void _internal_set_x(float value);
  public:

  // float y = 2;
  void clear_y();
  float y() const;
  void set_y(float value);
  private:
  float _internal_y() const;
  void _internal_set_y(float value);
  public:

  // @@protoc_insertion_point(class_scope:perception.Point2D)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  float x_;
  float y_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_freespace_5fobstacles_2eproto;
};
// -------------------------------------------------------------------

class FreespaceObstacle PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:perception.FreespaceObstacle) */ {
 public:
  inline FreespaceObstacle() : FreespaceObstacle(nullptr) {};
  virtual ~FreespaceObstacle();

  FreespaceObstacle(const FreespaceObstacle& from);
  FreespaceObstacle(FreespaceObstacle&& from) noexcept
    : FreespaceObstacle() {
    *this = ::std::move(from);
  }

  inline FreespaceObstacle& operator=(const FreespaceObstacle& from) {
    CopyFrom(from);
    return *this;
  }
  inline FreespaceObstacle& operator=(FreespaceObstacle&& from) noexcept {
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
  static const FreespaceObstacle& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const FreespaceObstacle* internal_default_instance() {
    return reinterpret_cast<const FreespaceObstacle*>(
               &_FreespaceObstacle_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(FreespaceObstacle& a, FreespaceObstacle& b) {
    a.Swap(&b);
  }
  inline void Swap(FreespaceObstacle* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(FreespaceObstacle* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline FreespaceObstacle* New() const final {
    return CreateMaybeMessage<FreespaceObstacle>(nullptr);
  }

  FreespaceObstacle* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<FreespaceObstacle>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const FreespaceObstacle& from);
  void MergeFrom(const FreespaceObstacle& from);
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
  void InternalSwap(FreespaceObstacle* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "perception.FreespaceObstacle";
  }
  protected:
  explicit FreespaceObstacle(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_freespace_5fobstacles_2eproto);
    return ::descriptor_table_freespace_5fobstacles_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef FreespaceObstacle_Type Type;
  static constexpr Type UNKNOW =
    FreespaceObstacle_Type_UNKNOW;
  static constexpr Type DYNAMIC =
    FreespaceObstacle_Type_DYNAMIC;
  static constexpr Type STATIC =
    FreespaceObstacle_Type_STATIC;
  static inline bool Type_IsValid(int value) {
    return FreespaceObstacle_Type_IsValid(value);
  }
  static constexpr Type Type_MIN =
    FreespaceObstacle_Type_Type_MIN;
  static constexpr Type Type_MAX =
    FreespaceObstacle_Type_Type_MAX;
  static constexpr int Type_ARRAYSIZE =
    FreespaceObstacle_Type_Type_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  Type_descriptor() {
    return FreespaceObstacle_Type_descriptor();
  }
  template<typename T>
  static inline const std::string& Type_Name(T enum_t_value) {
    static_assert(::std::is_same<T, Type>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function Type_Name.");
    return FreespaceObstacle_Type_Name(enum_t_value);
  }
  static inline bool Type_Parse(const std::string& name,
      Type* value) {
    return FreespaceObstacle_Type_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kPointsFieldNumber = 2,
    kIdFieldNumber = 1,
    kClassFieldNumber = 3,
    kCategoryFieldNumber = 4,
    kUssClassFieldNumber = 5,
    kUssDistFieldNumber = 6,
  };
  // repeated .perception.Point2D points = 2;
  int points_size() const;
  private:
  int _internal_points_size() const;
  public:
  void clear_points();
  ::perception::Point2D* mutable_points(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::perception::Point2D >*
      mutable_points();
  private:
  const ::perception::Point2D& _internal_points(int index) const;
  ::perception::Point2D* _internal_add_points();
  public:
  const ::perception::Point2D& points(int index) const;
  ::perception::Point2D* add_points();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::perception::Point2D >&
      points() const;

  // uint32 id = 1;
  void clear_id();
  ::PROTOBUF_NAMESPACE_ID::uint32 id() const;
  void set_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_id() const;
  void _internal_set_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // .perception.Class class = 3;
  void clear_class_();
  ::perception::Class class_() const;
  void set_class_(::perception::Class value);
  private:
  ::perception::Class _internal_class_() const;
  void _internal_set_class_(::perception::Class value);
  public:

  // .perception.Category category = 4;
  void clear_category();
  ::perception::Category category() const;
  void set_category(::perception::Category value);
  private:
  ::perception::Category _internal_category() const;
  void _internal_set_category(::perception::Category value);
  public:

  // .perception.UssClass uss_class = 5;
  void clear_uss_class();
  ::perception::UssClass uss_class() const;
  void set_uss_class(::perception::UssClass value);
  private:
  ::perception::UssClass _internal_uss_class() const;
  void _internal_set_uss_class(::perception::UssClass value);
  public:

  // float uss_dist = 6;
  void clear_uss_dist();
  float uss_dist() const;
  void set_uss_dist(float value);
  private:
  float _internal_uss_dist() const;
  void _internal_set_uss_dist(float value);
  public:

  // @@protoc_insertion_point(class_scope:perception.FreespaceObstacle)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::perception::Point2D > points_;
  ::PROTOBUF_NAMESPACE_ID::uint32 id_;
  int class__;
  int category_;
  int uss_class_;
  float uss_dist_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_freespace_5fobstacles_2eproto;
};
// -------------------------------------------------------------------

class FreespaceObstacles PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:perception.FreespaceObstacles) */ {
 public:
  inline FreespaceObstacles() : FreespaceObstacles(nullptr) {};
  virtual ~FreespaceObstacles();

  FreespaceObstacles(const FreespaceObstacles& from);
  FreespaceObstacles(FreespaceObstacles&& from) noexcept
    : FreespaceObstacles() {
    *this = ::std::move(from);
  }

  inline FreespaceObstacles& operator=(const FreespaceObstacles& from) {
    CopyFrom(from);
    return *this;
  }
  inline FreespaceObstacles& operator=(FreespaceObstacles&& from) noexcept {
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
  static const FreespaceObstacles& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const FreespaceObstacles* internal_default_instance() {
    return reinterpret_cast<const FreespaceObstacles*>(
               &_FreespaceObstacles_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(FreespaceObstacles& a, FreespaceObstacles& b) {
    a.Swap(&b);
  }
  inline void Swap(FreespaceObstacles* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(FreespaceObstacles* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline FreespaceObstacles* New() const final {
    return CreateMaybeMessage<FreespaceObstacles>(nullptr);
  }

  FreespaceObstacles* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<FreespaceObstacles>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const FreespaceObstacles& from);
  void MergeFrom(const FreespaceObstacles& from);
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
  void InternalSwap(FreespaceObstacles* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "perception.FreespaceObstacles";
  }
  protected:
  explicit FreespaceObstacles(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_freespace_5fobstacles_2eproto);
    return ::descriptor_table_freespace_5fobstacles_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kObstaclesFieldNumber = 2,
    kTimestampFieldNumber = 1,
    kTickFieldNumber = 3,
    kFrameIdFieldNumber = 4,
  };
  // repeated .perception.FreespaceObstacle obstacles = 2;
  int obstacles_size() const;
  private:
  int _internal_obstacles_size() const;
  public:
  void clear_obstacles();
  ::perception::FreespaceObstacle* mutable_obstacles(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::perception::FreespaceObstacle >*
      mutable_obstacles();
  private:
  const ::perception::FreespaceObstacle& _internal_obstacles(int index) const;
  ::perception::FreespaceObstacle* _internal_add_obstacles();
  public:
  const ::perception::FreespaceObstacle& obstacles(int index) const;
  ::perception::FreespaceObstacle* add_obstacles();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::perception::FreespaceObstacle >&
      obstacles() const;

  // uint64 timestamp = 1;
  void clear_timestamp();
  ::PROTOBUF_NAMESPACE_ID::uint64 timestamp() const;
  void set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_timestamp() const;
  void _internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // uint64 tick = 3;
  void clear_tick();
  ::PROTOBUF_NAMESPACE_ID::uint64 tick() const;
  void set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_tick() const;
  void _internal_set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // uint64 frame_id = 4;
  void clear_frame_id();
  ::PROTOBUF_NAMESPACE_ID::uint64 frame_id() const;
  void set_frame_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_frame_id() const;
  void _internal_set_frame_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // @@protoc_insertion_point(class_scope:perception.FreespaceObstacles)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::perception::FreespaceObstacle > obstacles_;
  ::PROTOBUF_NAMESPACE_ID::uint64 timestamp_;
  ::PROTOBUF_NAMESPACE_ID::uint64 tick_;
  ::PROTOBUF_NAMESPACE_ID::uint64 frame_id_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_freespace_5fobstacles_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// Point2D

// float x = 1;
inline void Point2D::clear_x() {
  x_ = 0;
}
inline float Point2D::_internal_x() const {
  return x_;
}
inline float Point2D::x() const {
  // @@protoc_insertion_point(field_get:perception.Point2D.x)
  return _internal_x();
}
inline void Point2D::_internal_set_x(float value) {
  
  x_ = value;
}
inline void Point2D::set_x(float value) {
  _internal_set_x(value);
  // @@protoc_insertion_point(field_set:perception.Point2D.x)
}

// float y = 2;
inline void Point2D::clear_y() {
  y_ = 0;
}
inline float Point2D::_internal_y() const {
  return y_;
}
inline float Point2D::y() const {
  // @@protoc_insertion_point(field_get:perception.Point2D.y)
  return _internal_y();
}
inline void Point2D::_internal_set_y(float value) {
  
  y_ = value;
}
inline void Point2D::set_y(float value) {
  _internal_set_y(value);
  // @@protoc_insertion_point(field_set:perception.Point2D.y)
}

// -------------------------------------------------------------------

// FreespaceObstacle

// uint32 id = 1;
inline void FreespaceObstacle::clear_id() {
  id_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 FreespaceObstacle::_internal_id() const {
  return id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 FreespaceObstacle::id() const {
  // @@protoc_insertion_point(field_get:perception.FreespaceObstacle.id)
  return _internal_id();
}
inline void FreespaceObstacle::_internal_set_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  id_ = value;
}
inline void FreespaceObstacle::set_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:perception.FreespaceObstacle.id)
}

// repeated .perception.Point2D points = 2;
inline int FreespaceObstacle::_internal_points_size() const {
  return points_.size();
}
inline int FreespaceObstacle::points_size() const {
  return _internal_points_size();
}
inline void FreespaceObstacle::clear_points() {
  points_.Clear();
}
inline ::perception::Point2D* FreespaceObstacle::mutable_points(int index) {
  // @@protoc_insertion_point(field_mutable:perception.FreespaceObstacle.points)
  return points_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::perception::Point2D >*
FreespaceObstacle::mutable_points() {
  // @@protoc_insertion_point(field_mutable_list:perception.FreespaceObstacle.points)
  return &points_;
}
inline const ::perception::Point2D& FreespaceObstacle::_internal_points(int index) const {
  return points_.Get(index);
}
inline const ::perception::Point2D& FreespaceObstacle::points(int index) const {
  // @@protoc_insertion_point(field_get:perception.FreespaceObstacle.points)
  return _internal_points(index);
}
inline ::perception::Point2D* FreespaceObstacle::_internal_add_points() {
  return points_.Add();
}
inline ::perception::Point2D* FreespaceObstacle::add_points() {
  // @@protoc_insertion_point(field_add:perception.FreespaceObstacle.points)
  return _internal_add_points();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::perception::Point2D >&
FreespaceObstacle::points() const {
  // @@protoc_insertion_point(field_list:perception.FreespaceObstacle.points)
  return points_;
}

// .perception.Class class = 3;
inline void FreespaceObstacle::clear_class_() {
  class__ = 0;
}
inline ::perception::Class FreespaceObstacle::_internal_class_() const {
  return static_cast< ::perception::Class >(class__);
}
inline ::perception::Class FreespaceObstacle::class_() const {
  // @@protoc_insertion_point(field_get:perception.FreespaceObstacle.class)
  return _internal_class_();
}
inline void FreespaceObstacle::_internal_set_class_(::perception::Class value) {
  
  class__ = value;
}
inline void FreespaceObstacle::set_class_(::perception::Class value) {
  _internal_set_class_(value);
  // @@protoc_insertion_point(field_set:perception.FreespaceObstacle.class)
}

// .perception.Category category = 4;
inline void FreespaceObstacle::clear_category() {
  category_ = 0;
}
inline ::perception::Category FreespaceObstacle::_internal_category() const {
  return static_cast< ::perception::Category >(category_);
}
inline ::perception::Category FreespaceObstacle::category() const {
  // @@protoc_insertion_point(field_get:perception.FreespaceObstacle.category)
  return _internal_category();
}
inline void FreespaceObstacle::_internal_set_category(::perception::Category value) {
  
  category_ = value;
}
inline void FreespaceObstacle::set_category(::perception::Category value) {
  _internal_set_category(value);
  // @@protoc_insertion_point(field_set:perception.FreespaceObstacle.category)
}

// .perception.UssClass uss_class = 5;
inline void FreespaceObstacle::clear_uss_class() {
  uss_class_ = 0;
}
inline ::perception::UssClass FreespaceObstacle::_internal_uss_class() const {
  return static_cast< ::perception::UssClass >(uss_class_);
}
inline ::perception::UssClass FreespaceObstacle::uss_class() const {
  // @@protoc_insertion_point(field_get:perception.FreespaceObstacle.uss_class)
  return _internal_uss_class();
}
inline void FreespaceObstacle::_internal_set_uss_class(::perception::UssClass value) {
  
  uss_class_ = value;
}
inline void FreespaceObstacle::set_uss_class(::perception::UssClass value) {
  _internal_set_uss_class(value);
  // @@protoc_insertion_point(field_set:perception.FreespaceObstacle.uss_class)
}

// float uss_dist = 6;
inline void FreespaceObstacle::clear_uss_dist() {
  uss_dist_ = 0;
}
inline float FreespaceObstacle::_internal_uss_dist() const {
  return uss_dist_;
}
inline float FreespaceObstacle::uss_dist() const {
  // @@protoc_insertion_point(field_get:perception.FreespaceObstacle.uss_dist)
  return _internal_uss_dist();
}
inline void FreespaceObstacle::_internal_set_uss_dist(float value) {
  
  uss_dist_ = value;
}
inline void FreespaceObstacle::set_uss_dist(float value) {
  _internal_set_uss_dist(value);
  // @@protoc_insertion_point(field_set:perception.FreespaceObstacle.uss_dist)
}

// -------------------------------------------------------------------

// FreespaceObstacles

// uint64 timestamp = 1;
inline void FreespaceObstacles::clear_timestamp() {
  timestamp_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 FreespaceObstacles::_internal_timestamp() const {
  return timestamp_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 FreespaceObstacles::timestamp() const {
  // @@protoc_insertion_point(field_get:perception.FreespaceObstacles.timestamp)
  return _internal_timestamp();
}
inline void FreespaceObstacles::_internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  timestamp_ = value;
}
inline void FreespaceObstacles::set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_timestamp(value);
  // @@protoc_insertion_point(field_set:perception.FreespaceObstacles.timestamp)
}

// repeated .perception.FreespaceObstacle obstacles = 2;
inline int FreespaceObstacles::_internal_obstacles_size() const {
  return obstacles_.size();
}
inline int FreespaceObstacles::obstacles_size() const {
  return _internal_obstacles_size();
}
inline void FreespaceObstacles::clear_obstacles() {
  obstacles_.Clear();
}
inline ::perception::FreespaceObstacle* FreespaceObstacles::mutable_obstacles(int index) {
  // @@protoc_insertion_point(field_mutable:perception.FreespaceObstacles.obstacles)
  return obstacles_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::perception::FreespaceObstacle >*
FreespaceObstacles::mutable_obstacles() {
  // @@protoc_insertion_point(field_mutable_list:perception.FreespaceObstacles.obstacles)
  return &obstacles_;
}
inline const ::perception::FreespaceObstacle& FreespaceObstacles::_internal_obstacles(int index) const {
  return obstacles_.Get(index);
}
inline const ::perception::FreespaceObstacle& FreespaceObstacles::obstacles(int index) const {
  // @@protoc_insertion_point(field_get:perception.FreespaceObstacles.obstacles)
  return _internal_obstacles(index);
}
inline ::perception::FreespaceObstacle* FreespaceObstacles::_internal_add_obstacles() {
  return obstacles_.Add();
}
inline ::perception::FreespaceObstacle* FreespaceObstacles::add_obstacles() {
  // @@protoc_insertion_point(field_add:perception.FreespaceObstacles.obstacles)
  return _internal_add_obstacles();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::perception::FreespaceObstacle >&
FreespaceObstacles::obstacles() const {
  // @@protoc_insertion_point(field_list:perception.FreespaceObstacles.obstacles)
  return obstacles_;
}

// uint64 tick = 3;
inline void FreespaceObstacles::clear_tick() {
  tick_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 FreespaceObstacles::_internal_tick() const {
  return tick_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 FreespaceObstacles::tick() const {
  // @@protoc_insertion_point(field_get:perception.FreespaceObstacles.tick)
  return _internal_tick();
}
inline void FreespaceObstacles::_internal_set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  tick_ = value;
}
inline void FreespaceObstacles::set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_tick(value);
  // @@protoc_insertion_point(field_set:perception.FreespaceObstacles.tick)
}

// uint64 frame_id = 4;
inline void FreespaceObstacles::clear_frame_id() {
  frame_id_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 FreespaceObstacles::_internal_frame_id() const {
  return frame_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 FreespaceObstacles::frame_id() const {
  // @@protoc_insertion_point(field_get:perception.FreespaceObstacles.frame_id)
  return _internal_frame_id();
}
inline void FreespaceObstacles::_internal_set_frame_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  frame_id_ = value;
}
inline void FreespaceObstacles::set_frame_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_frame_id(value);
  // @@protoc_insertion_point(field_set:perception.FreespaceObstacles.frame_id)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace perception

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::perception::FreespaceObstacle_Type> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::perception::FreespaceObstacle_Type>() {
  return ::perception::FreespaceObstacle_Type_descriptor();
}
template <> struct is_proto_enum< ::perception::Class> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::perception::Class>() {
  return ::perception::Class_descriptor();
}
template <> struct is_proto_enum< ::perception::Category> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::perception::Category>() {
  return ::perception::Category_descriptor();
}
template <> struct is_proto_enum< ::perception::UssClass> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::perception::UssClass>() {
  return ::perception::UssClass_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_freespace_5fobstacles_2eproto