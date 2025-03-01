// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: apa_hmi_signal.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_apa_5fhmi_5fsignal_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_apa_5fhmi_5fsignal_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_apa_5fhmi_5fsignal_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_apa_5fhmi_5fsignal_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_apa_5fhmi_5fsignal_2eproto;
namespace minieye {
class APAHmiSignal;
class APAHmiSignalDefaultTypeInternal;
extern APAHmiSignalDefaultTypeInternal _APAHmiSignal_default_instance_;
class APAHmiSignalRes;
class APAHmiSignalResDefaultTypeInternal;
extern APAHmiSignalResDefaultTypeInternal _APAHmiSignalRes_default_instance_;
}  // namespace minieye
PROTOBUF_NAMESPACE_OPEN
template<> ::minieye::APAHmiSignal* Arena::CreateMaybeMessage<::minieye::APAHmiSignal>(Arena*);
template<> ::minieye::APAHmiSignalRes* Arena::CreateMaybeMessage<::minieye::APAHmiSignalRes>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace minieye {

enum APAHmiSignal_APAHmiSignalType : int {
  APAHmiSignal_APAHmiSignalType_APA_HMI_SIGNAL_UNKNOWN = 0,
  APAHmiSignal_APAHmiSignalType_APA_HMI_SIGNAL_SWITCH_ON = 1,
  APAHmiSignal_APAHmiSignalType_APA_HMI_SIGNAL_SWITCH_OFF = 2,
  APAHmiSignal_APAHmiSignalType_APAHmiSignal_APAHmiSignalType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  APAHmiSignal_APAHmiSignalType_APAHmiSignal_APAHmiSignalType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool APAHmiSignal_APAHmiSignalType_IsValid(int value);
constexpr APAHmiSignal_APAHmiSignalType APAHmiSignal_APAHmiSignalType_APAHmiSignalType_MIN = APAHmiSignal_APAHmiSignalType_APA_HMI_SIGNAL_UNKNOWN;
constexpr APAHmiSignal_APAHmiSignalType APAHmiSignal_APAHmiSignalType_APAHmiSignalType_MAX = APAHmiSignal_APAHmiSignalType_APA_HMI_SIGNAL_SWITCH_OFF;
constexpr int APAHmiSignal_APAHmiSignalType_APAHmiSignalType_ARRAYSIZE = APAHmiSignal_APAHmiSignalType_APAHmiSignalType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* APAHmiSignal_APAHmiSignalType_descriptor();
template<typename T>
inline const std::string& APAHmiSignal_APAHmiSignalType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, APAHmiSignal_APAHmiSignalType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function APAHmiSignal_APAHmiSignalType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    APAHmiSignal_APAHmiSignalType_descriptor(), enum_t_value);
}
inline bool APAHmiSignal_APAHmiSignalType_Parse(
    const std::string& name, APAHmiSignal_APAHmiSignalType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<APAHmiSignal_APAHmiSignalType>(
    APAHmiSignal_APAHmiSignalType_descriptor(), name, value);
}
enum APAHmiSignalRes_APAHmiSignalResType : int {
  APAHmiSignalRes_APAHmiSignalResType_APA_HMI_CUR_MODE_UNKNOWN = 0,
  APAHmiSignalRes_APAHmiSignalResType_APA_HMI_CUR_MODE_APA = 1,
  APAHmiSignalRes_APAHmiSignalResType_APA_HMI_CUR_MODE_POLIT = 2,
  APAHmiSignalRes_APAHmiSignalResType_APA_HMI_CUR_MODE_IDLE = 3,
  APAHmiSignalRes_APAHmiSignalResType_APA_HMI_CUR_MODE_MANTAINCE = 4,
  APAHmiSignalRes_APAHmiSignalResType_APAHmiSignalRes_APAHmiSignalResType_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  APAHmiSignalRes_APAHmiSignalResType_APAHmiSignalRes_APAHmiSignalResType_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool APAHmiSignalRes_APAHmiSignalResType_IsValid(int value);
constexpr APAHmiSignalRes_APAHmiSignalResType APAHmiSignalRes_APAHmiSignalResType_APAHmiSignalResType_MIN = APAHmiSignalRes_APAHmiSignalResType_APA_HMI_CUR_MODE_UNKNOWN;
constexpr APAHmiSignalRes_APAHmiSignalResType APAHmiSignalRes_APAHmiSignalResType_APAHmiSignalResType_MAX = APAHmiSignalRes_APAHmiSignalResType_APA_HMI_CUR_MODE_MANTAINCE;
constexpr int APAHmiSignalRes_APAHmiSignalResType_APAHmiSignalResType_ARRAYSIZE = APAHmiSignalRes_APAHmiSignalResType_APAHmiSignalResType_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* APAHmiSignalRes_APAHmiSignalResType_descriptor();
template<typename T>
inline const std::string& APAHmiSignalRes_APAHmiSignalResType_Name(T enum_t_value) {
  static_assert(::std::is_same<T, APAHmiSignalRes_APAHmiSignalResType>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function APAHmiSignalRes_APAHmiSignalResType_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    APAHmiSignalRes_APAHmiSignalResType_descriptor(), enum_t_value);
}
inline bool APAHmiSignalRes_APAHmiSignalResType_Parse(
    const std::string& name, APAHmiSignalRes_APAHmiSignalResType* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<APAHmiSignalRes_APAHmiSignalResType>(
    APAHmiSignalRes_APAHmiSignalResType_descriptor(), name, value);
}
// ===================================================================

class APAHmiSignal PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:minieye.APAHmiSignal) */ {
 public:
  inline APAHmiSignal() : APAHmiSignal(nullptr) {};
  virtual ~APAHmiSignal();

  APAHmiSignal(const APAHmiSignal& from);
  APAHmiSignal(APAHmiSignal&& from) noexcept
    : APAHmiSignal() {
    *this = ::std::move(from);
  }

  inline APAHmiSignal& operator=(const APAHmiSignal& from) {
    CopyFrom(from);
    return *this;
  }
  inline APAHmiSignal& operator=(APAHmiSignal&& from) noexcept {
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
  static const APAHmiSignal& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const APAHmiSignal* internal_default_instance() {
    return reinterpret_cast<const APAHmiSignal*>(
               &_APAHmiSignal_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(APAHmiSignal& a, APAHmiSignal& b) {
    a.Swap(&b);
  }
  inline void Swap(APAHmiSignal* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(APAHmiSignal* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline APAHmiSignal* New() const final {
    return CreateMaybeMessage<APAHmiSignal>(nullptr);
  }

  APAHmiSignal* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<APAHmiSignal>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const APAHmiSignal& from);
  void MergeFrom(const APAHmiSignal& from);
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
  void InternalSwap(APAHmiSignal* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "minieye.APAHmiSignal";
  }
  protected:
  explicit APAHmiSignal(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_apa_5fhmi_5fsignal_2eproto);
    return ::descriptor_table_apa_5fhmi_5fsignal_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef APAHmiSignal_APAHmiSignalType APAHmiSignalType;
  static constexpr APAHmiSignalType APA_HMI_SIGNAL_UNKNOWN =
    APAHmiSignal_APAHmiSignalType_APA_HMI_SIGNAL_UNKNOWN;
  static constexpr APAHmiSignalType APA_HMI_SIGNAL_SWITCH_ON =
    APAHmiSignal_APAHmiSignalType_APA_HMI_SIGNAL_SWITCH_ON;
  static constexpr APAHmiSignalType APA_HMI_SIGNAL_SWITCH_OFF =
    APAHmiSignal_APAHmiSignalType_APA_HMI_SIGNAL_SWITCH_OFF;
  static inline bool APAHmiSignalType_IsValid(int value) {
    return APAHmiSignal_APAHmiSignalType_IsValid(value);
  }
  static constexpr APAHmiSignalType APAHmiSignalType_MIN =
    APAHmiSignal_APAHmiSignalType_APAHmiSignalType_MIN;
  static constexpr APAHmiSignalType APAHmiSignalType_MAX =
    APAHmiSignal_APAHmiSignalType_APAHmiSignalType_MAX;
  static constexpr int APAHmiSignalType_ARRAYSIZE =
    APAHmiSignal_APAHmiSignalType_APAHmiSignalType_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  APAHmiSignalType_descriptor() {
    return APAHmiSignal_APAHmiSignalType_descriptor();
  }
  template<typename T>
  static inline const std::string& APAHmiSignalType_Name(T enum_t_value) {
    static_assert(::std::is_same<T, APAHmiSignalType>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function APAHmiSignalType_Name.");
    return APAHmiSignal_APAHmiSignalType_Name(enum_t_value);
  }
  static inline bool APAHmiSignalType_Parse(const std::string& name,
      APAHmiSignalType* value) {
    return APAHmiSignal_APAHmiSignalType_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kTimestampFieldNumber = 2,
    kTickFieldNumber = 3,
    kSignalFieldNumber = 1,
  };
  // uint64 timestamp = 2;
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

  // uint32 signal = 1;
  void clear_signal();
  ::PROTOBUF_NAMESPACE_ID::uint32 signal() const;
  void set_signal(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_signal() const;
  void _internal_set_signal(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:minieye.APAHmiSignal)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::uint64 timestamp_;
  ::PROTOBUF_NAMESPACE_ID::uint64 tick_;
  ::PROTOBUF_NAMESPACE_ID::uint32 signal_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_apa_5fhmi_5fsignal_2eproto;
};
// -------------------------------------------------------------------

class APAHmiSignalRes PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:minieye.APAHmiSignalRes) */ {
 public:
  inline APAHmiSignalRes() : APAHmiSignalRes(nullptr) {};
  virtual ~APAHmiSignalRes();

  APAHmiSignalRes(const APAHmiSignalRes& from);
  APAHmiSignalRes(APAHmiSignalRes&& from) noexcept
    : APAHmiSignalRes() {
    *this = ::std::move(from);
  }

  inline APAHmiSignalRes& operator=(const APAHmiSignalRes& from) {
    CopyFrom(from);
    return *this;
  }
  inline APAHmiSignalRes& operator=(APAHmiSignalRes&& from) noexcept {
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
  static const APAHmiSignalRes& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const APAHmiSignalRes* internal_default_instance() {
    return reinterpret_cast<const APAHmiSignalRes*>(
               &_APAHmiSignalRes_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(APAHmiSignalRes& a, APAHmiSignalRes& b) {
    a.Swap(&b);
  }
  inline void Swap(APAHmiSignalRes* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(APAHmiSignalRes* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline APAHmiSignalRes* New() const final {
    return CreateMaybeMessage<APAHmiSignalRes>(nullptr);
  }

  APAHmiSignalRes* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<APAHmiSignalRes>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const APAHmiSignalRes& from);
  void MergeFrom(const APAHmiSignalRes& from);
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
  void InternalSwap(APAHmiSignalRes* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "minieye.APAHmiSignalRes";
  }
  protected:
  explicit APAHmiSignalRes(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_apa_5fhmi_5fsignal_2eproto);
    return ::descriptor_table_apa_5fhmi_5fsignal_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  typedef APAHmiSignalRes_APAHmiSignalResType APAHmiSignalResType;
  static constexpr APAHmiSignalResType APA_HMI_CUR_MODE_UNKNOWN =
    APAHmiSignalRes_APAHmiSignalResType_APA_HMI_CUR_MODE_UNKNOWN;
  static constexpr APAHmiSignalResType APA_HMI_CUR_MODE_APA =
    APAHmiSignalRes_APAHmiSignalResType_APA_HMI_CUR_MODE_APA;
  static constexpr APAHmiSignalResType APA_HMI_CUR_MODE_POLIT =
    APAHmiSignalRes_APAHmiSignalResType_APA_HMI_CUR_MODE_POLIT;
  static constexpr APAHmiSignalResType APA_HMI_CUR_MODE_IDLE =
    APAHmiSignalRes_APAHmiSignalResType_APA_HMI_CUR_MODE_IDLE;
  static constexpr APAHmiSignalResType APA_HMI_CUR_MODE_MANTAINCE =
    APAHmiSignalRes_APAHmiSignalResType_APA_HMI_CUR_MODE_MANTAINCE;
  static inline bool APAHmiSignalResType_IsValid(int value) {
    return APAHmiSignalRes_APAHmiSignalResType_IsValid(value);
  }
  static constexpr APAHmiSignalResType APAHmiSignalResType_MIN =
    APAHmiSignalRes_APAHmiSignalResType_APAHmiSignalResType_MIN;
  static constexpr APAHmiSignalResType APAHmiSignalResType_MAX =
    APAHmiSignalRes_APAHmiSignalResType_APAHmiSignalResType_MAX;
  static constexpr int APAHmiSignalResType_ARRAYSIZE =
    APAHmiSignalRes_APAHmiSignalResType_APAHmiSignalResType_ARRAYSIZE;
  static inline const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor*
  APAHmiSignalResType_descriptor() {
    return APAHmiSignalRes_APAHmiSignalResType_descriptor();
  }
  template<typename T>
  static inline const std::string& APAHmiSignalResType_Name(T enum_t_value) {
    static_assert(::std::is_same<T, APAHmiSignalResType>::value ||
      ::std::is_integral<T>::value,
      "Incorrect type passed to function APAHmiSignalResType_Name.");
    return APAHmiSignalRes_APAHmiSignalResType_Name(enum_t_value);
  }
  static inline bool APAHmiSignalResType_Parse(const std::string& name,
      APAHmiSignalResType* value) {
    return APAHmiSignalRes_APAHmiSignalResType_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  enum : int {
    kTimestampFieldNumber = 2,
    kTickFieldNumber = 3,
    kResCurModeFieldNumber = 1,
  };
  // uint64 timestamp = 2;
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

  // uint32 resCurMode = 1;
  void clear_rescurmode();
  ::PROTOBUF_NAMESPACE_ID::uint32 rescurmode() const;
  void set_rescurmode(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_rescurmode() const;
  void _internal_set_rescurmode(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:minieye.APAHmiSignalRes)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::uint64 timestamp_;
  ::PROTOBUF_NAMESPACE_ID::uint64 tick_;
  ::PROTOBUF_NAMESPACE_ID::uint32 rescurmode_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_apa_5fhmi_5fsignal_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// APAHmiSignal

// uint32 signal = 1;
inline void APAHmiSignal::clear_signal() {
  signal_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 APAHmiSignal::_internal_signal() const {
  return signal_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 APAHmiSignal::signal() const {
  // @@protoc_insertion_point(field_get:minieye.APAHmiSignal.signal)
  return _internal_signal();
}
inline void APAHmiSignal::_internal_set_signal(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  signal_ = value;
}
inline void APAHmiSignal::set_signal(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_signal(value);
  // @@protoc_insertion_point(field_set:minieye.APAHmiSignal.signal)
}

// uint64 timestamp = 2;
inline void APAHmiSignal::clear_timestamp() {
  timestamp_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 APAHmiSignal::_internal_timestamp() const {
  return timestamp_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 APAHmiSignal::timestamp() const {
  // @@protoc_insertion_point(field_get:minieye.APAHmiSignal.timestamp)
  return _internal_timestamp();
}
inline void APAHmiSignal::_internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  timestamp_ = value;
}
inline void APAHmiSignal::set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_timestamp(value);
  // @@protoc_insertion_point(field_set:minieye.APAHmiSignal.timestamp)
}

// uint64 tick = 3;
inline void APAHmiSignal::clear_tick() {
  tick_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 APAHmiSignal::_internal_tick() const {
  return tick_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 APAHmiSignal::tick() const {
  // @@protoc_insertion_point(field_get:minieye.APAHmiSignal.tick)
  return _internal_tick();
}
inline void APAHmiSignal::_internal_set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  tick_ = value;
}
inline void APAHmiSignal::set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_tick(value);
  // @@protoc_insertion_point(field_set:minieye.APAHmiSignal.tick)
}

// -------------------------------------------------------------------

// APAHmiSignalRes

// uint32 resCurMode = 1;
inline void APAHmiSignalRes::clear_rescurmode() {
  rescurmode_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 APAHmiSignalRes::_internal_rescurmode() const {
  return rescurmode_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 APAHmiSignalRes::rescurmode() const {
  // @@protoc_insertion_point(field_get:minieye.APAHmiSignalRes.resCurMode)
  return _internal_rescurmode();
}
inline void APAHmiSignalRes::_internal_set_rescurmode(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  rescurmode_ = value;
}
inline void APAHmiSignalRes::set_rescurmode(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_rescurmode(value);
  // @@protoc_insertion_point(field_set:minieye.APAHmiSignalRes.resCurMode)
}

// uint64 timestamp = 2;
inline void APAHmiSignalRes::clear_timestamp() {
  timestamp_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 APAHmiSignalRes::_internal_timestamp() const {
  return timestamp_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 APAHmiSignalRes::timestamp() const {
  // @@protoc_insertion_point(field_get:minieye.APAHmiSignalRes.timestamp)
  return _internal_timestamp();
}
inline void APAHmiSignalRes::_internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  timestamp_ = value;
}
inline void APAHmiSignalRes::set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_timestamp(value);
  // @@protoc_insertion_point(field_set:minieye.APAHmiSignalRes.timestamp)
}

// uint64 tick = 3;
inline void APAHmiSignalRes::clear_tick() {
  tick_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 APAHmiSignalRes::_internal_tick() const {
  return tick_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 APAHmiSignalRes::tick() const {
  // @@protoc_insertion_point(field_get:minieye.APAHmiSignalRes.tick)
  return _internal_tick();
}
inline void APAHmiSignalRes::_internal_set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  tick_ = value;
}
inline void APAHmiSignalRes::set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_tick(value);
  // @@protoc_insertion_point(field_set:minieye.APAHmiSignalRes.tick)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace minieye

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::minieye::APAHmiSignal_APAHmiSignalType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::minieye::APAHmiSignal_APAHmiSignalType>() {
  return ::minieye::APAHmiSignal_APAHmiSignalType_descriptor();
}
template <> struct is_proto_enum< ::minieye::APAHmiSignalRes_APAHmiSignalResType> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::minieye::APAHmiSignalRes_APAHmiSignalResType>() {
  return ::minieye::APAHmiSignalRes_APAHmiSignalResType_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_apa_5fhmi_5fsignal_2eproto
