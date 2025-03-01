// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ultra_radar.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_ultra_5fradar_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_ultra_5fradar_2eproto

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
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_ultra_5fradar_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_ultra_5fradar_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_ultra_5fradar_2eproto;
namespace minieye {
class UltrasonicRadar;
class UltrasonicRadarDefaultTypeInternal;
extern UltrasonicRadarDefaultTypeInternal _UltrasonicRadar_default_instance_;
}  // namespace minieye
PROTOBUF_NAMESPACE_OPEN
template<> ::minieye::UltrasonicRadar* Arena::CreateMaybeMessage<::minieye::UltrasonicRadar>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace minieye {

// ===================================================================

class UltrasonicRadar PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:minieye.UltrasonicRadar) */ {
 public:
  inline UltrasonicRadar() : UltrasonicRadar(nullptr) {};
  virtual ~UltrasonicRadar();

  UltrasonicRadar(const UltrasonicRadar& from);
  UltrasonicRadar(UltrasonicRadar&& from) noexcept
    : UltrasonicRadar() {
    *this = ::std::move(from);
  }

  inline UltrasonicRadar& operator=(const UltrasonicRadar& from) {
    CopyFrom(from);
    return *this;
  }
  inline UltrasonicRadar& operator=(UltrasonicRadar&& from) noexcept {
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
  static const UltrasonicRadar& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const UltrasonicRadar* internal_default_instance() {
    return reinterpret_cast<const UltrasonicRadar*>(
               &_UltrasonicRadar_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(UltrasonicRadar& a, UltrasonicRadar& b) {
    a.Swap(&b);
  }
  inline void Swap(UltrasonicRadar* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(UltrasonicRadar* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline UltrasonicRadar* New() const final {
    return CreateMaybeMessage<UltrasonicRadar>(nullptr);
  }

  UltrasonicRadar* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<UltrasonicRadar>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const UltrasonicRadar& from);
  void MergeFrom(const UltrasonicRadar& from);
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
  void InternalSwap(UltrasonicRadar* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "minieye.UltrasonicRadar";
  }
  protected:
  explicit UltrasonicRadar(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_ultra_5fradar_2eproto);
    return ::descriptor_table_ultra_5fradar_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kShortRadarFlFieldNumber = 1,
    kShortRadarFlmFieldNumber = 2,
    kShortRadarFrmFieldNumber = 3,
    kShortRadarFrFieldNumber = 4,
    kShortRadarBlFieldNumber = 5,
    kShortRadarBlmFieldNumber = 6,
    kShortRadarBrmFieldNumber = 7,
    kShortRadarBrFieldNumber = 8,
    kLongRadarFlFieldNumber = 9,
    kLongRadarFrFieldNumber = 10,
    kLongRadarBlFieldNumber = 11,
    kLongRadarBrFieldNumber = 12,
    kTimestampFieldNumber = 13,
    kTickFieldNumber = 14,
    kFrameIdFieldNumber = 15,
  };
  // float short_radar_fl = 1;
  void clear_short_radar_fl();
  float short_radar_fl() const;
  void set_short_radar_fl(float value);
  private:
  float _internal_short_radar_fl() const;
  void _internal_set_short_radar_fl(float value);
  public:

  // float short_radar_flm = 2;
  void clear_short_radar_flm();
  float short_radar_flm() const;
  void set_short_radar_flm(float value);
  private:
  float _internal_short_radar_flm() const;
  void _internal_set_short_radar_flm(float value);
  public:

  // float short_radar_frm = 3;
  void clear_short_radar_frm();
  float short_radar_frm() const;
  void set_short_radar_frm(float value);
  private:
  float _internal_short_radar_frm() const;
  void _internal_set_short_radar_frm(float value);
  public:

  // float short_radar_fr = 4;
  void clear_short_radar_fr();
  float short_radar_fr() const;
  void set_short_radar_fr(float value);
  private:
  float _internal_short_radar_fr() const;
  void _internal_set_short_radar_fr(float value);
  public:

  // float short_radar_bl = 5;
  void clear_short_radar_bl();
  float short_radar_bl() const;
  void set_short_radar_bl(float value);
  private:
  float _internal_short_radar_bl() const;
  void _internal_set_short_radar_bl(float value);
  public:

  // float short_radar_blm = 6;
  void clear_short_radar_blm();
  float short_radar_blm() const;
  void set_short_radar_blm(float value);
  private:
  float _internal_short_radar_blm() const;
  void _internal_set_short_radar_blm(float value);
  public:

  // float short_radar_brm = 7;
  void clear_short_radar_brm();
  float short_radar_brm() const;
  void set_short_radar_brm(float value);
  private:
  float _internal_short_radar_brm() const;
  void _internal_set_short_radar_brm(float value);
  public:

  // float short_radar_br = 8;
  void clear_short_radar_br();
  float short_radar_br() const;
  void set_short_radar_br(float value);
  private:
  float _internal_short_radar_br() const;
  void _internal_set_short_radar_br(float value);
  public:

  // float long_radar_fl = 9;
  void clear_long_radar_fl();
  float long_radar_fl() const;
  void set_long_radar_fl(float value);
  private:
  float _internal_long_radar_fl() const;
  void _internal_set_long_radar_fl(float value);
  public:

  // float long_radar_fr = 10;
  void clear_long_radar_fr();
  float long_radar_fr() const;
  void set_long_radar_fr(float value);
  private:
  float _internal_long_radar_fr() const;
  void _internal_set_long_radar_fr(float value);
  public:

  // float long_radar_bl = 11;
  void clear_long_radar_bl();
  float long_radar_bl() const;
  void set_long_radar_bl(float value);
  private:
  float _internal_long_radar_bl() const;
  void _internal_set_long_radar_bl(float value);
  public:

  // float long_radar_br = 12;
  void clear_long_radar_br();
  float long_radar_br() const;
  void set_long_radar_br(float value);
  private:
  float _internal_long_radar_br() const;
  void _internal_set_long_radar_br(float value);
  public:

  // uint64 timestamp = 13;
  void clear_timestamp();
  ::PROTOBUF_NAMESPACE_ID::uint64 timestamp() const;
  void set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_timestamp() const;
  void _internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // uint64 tick = 14;
  void clear_tick();
  ::PROTOBUF_NAMESPACE_ID::uint64 tick() const;
  void set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_tick() const;
  void _internal_set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // uint64 frame_id = 15;
  void clear_frame_id();
  ::PROTOBUF_NAMESPACE_ID::uint64 frame_id() const;
  void set_frame_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_frame_id() const;
  void _internal_set_frame_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // @@protoc_insertion_point(class_scope:minieye.UltrasonicRadar)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  float short_radar_fl_;
  float short_radar_flm_;
  float short_radar_frm_;
  float short_radar_fr_;
  float short_radar_bl_;
  float short_radar_blm_;
  float short_radar_brm_;
  float short_radar_br_;
  float long_radar_fl_;
  float long_radar_fr_;
  float long_radar_bl_;
  float long_radar_br_;
  ::PROTOBUF_NAMESPACE_ID::uint64 timestamp_;
  ::PROTOBUF_NAMESPACE_ID::uint64 tick_;
  ::PROTOBUF_NAMESPACE_ID::uint64 frame_id_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_ultra_5fradar_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// UltrasonicRadar

// float short_radar_fl = 1;
inline void UltrasonicRadar::clear_short_radar_fl() {
  short_radar_fl_ = 0;
}
inline float UltrasonicRadar::_internal_short_radar_fl() const {
  return short_radar_fl_;
}
inline float UltrasonicRadar::short_radar_fl() const {
  // @@protoc_insertion_point(field_get:minieye.UltrasonicRadar.short_radar_fl)
  return _internal_short_radar_fl();
}
inline void UltrasonicRadar::_internal_set_short_radar_fl(float value) {
  
  short_radar_fl_ = value;
}
inline void UltrasonicRadar::set_short_radar_fl(float value) {
  _internal_set_short_radar_fl(value);
  // @@protoc_insertion_point(field_set:minieye.UltrasonicRadar.short_radar_fl)
}

// float short_radar_flm = 2;
inline void UltrasonicRadar::clear_short_radar_flm() {
  short_radar_flm_ = 0;
}
inline float UltrasonicRadar::_internal_short_radar_flm() const {
  return short_radar_flm_;
}
inline float UltrasonicRadar::short_radar_flm() const {
  // @@protoc_insertion_point(field_get:minieye.UltrasonicRadar.short_radar_flm)
  return _internal_short_radar_flm();
}
inline void UltrasonicRadar::_internal_set_short_radar_flm(float value) {
  
  short_radar_flm_ = value;
}
inline void UltrasonicRadar::set_short_radar_flm(float value) {
  _internal_set_short_radar_flm(value);
  // @@protoc_insertion_point(field_set:minieye.UltrasonicRadar.short_radar_flm)
}

// float short_radar_frm = 3;
inline void UltrasonicRadar::clear_short_radar_frm() {
  short_radar_frm_ = 0;
}
inline float UltrasonicRadar::_internal_short_radar_frm() const {
  return short_radar_frm_;
}
inline float UltrasonicRadar::short_radar_frm() const {
  // @@protoc_insertion_point(field_get:minieye.UltrasonicRadar.short_radar_frm)
  return _internal_short_radar_frm();
}
inline void UltrasonicRadar::_internal_set_short_radar_frm(float value) {
  
  short_radar_frm_ = value;
}
inline void UltrasonicRadar::set_short_radar_frm(float value) {
  _internal_set_short_radar_frm(value);
  // @@protoc_insertion_point(field_set:minieye.UltrasonicRadar.short_radar_frm)
}

// float short_radar_fr = 4;
inline void UltrasonicRadar::clear_short_radar_fr() {
  short_radar_fr_ = 0;
}
inline float UltrasonicRadar::_internal_short_radar_fr() const {
  return short_radar_fr_;
}
inline float UltrasonicRadar::short_radar_fr() const {
  // @@protoc_insertion_point(field_get:minieye.UltrasonicRadar.short_radar_fr)
  return _internal_short_radar_fr();
}
inline void UltrasonicRadar::_internal_set_short_radar_fr(float value) {
  
  short_radar_fr_ = value;
}
inline void UltrasonicRadar::set_short_radar_fr(float value) {
  _internal_set_short_radar_fr(value);
  // @@protoc_insertion_point(field_set:minieye.UltrasonicRadar.short_radar_fr)
}

// float short_radar_bl = 5;
inline void UltrasonicRadar::clear_short_radar_bl() {
  short_radar_bl_ = 0;
}
inline float UltrasonicRadar::_internal_short_radar_bl() const {
  return short_radar_bl_;
}
inline float UltrasonicRadar::short_radar_bl() const {
  // @@protoc_insertion_point(field_get:minieye.UltrasonicRadar.short_radar_bl)
  return _internal_short_radar_bl();
}
inline void UltrasonicRadar::_internal_set_short_radar_bl(float value) {
  
  short_radar_bl_ = value;
}
inline void UltrasonicRadar::set_short_radar_bl(float value) {
  _internal_set_short_radar_bl(value);
  // @@protoc_insertion_point(field_set:minieye.UltrasonicRadar.short_radar_bl)
}

// float short_radar_blm = 6;
inline void UltrasonicRadar::clear_short_radar_blm() {
  short_radar_blm_ = 0;
}
inline float UltrasonicRadar::_internal_short_radar_blm() const {
  return short_radar_blm_;
}
inline float UltrasonicRadar::short_radar_blm() const {
  // @@protoc_insertion_point(field_get:minieye.UltrasonicRadar.short_radar_blm)
  return _internal_short_radar_blm();
}
inline void UltrasonicRadar::_internal_set_short_radar_blm(float value) {
  
  short_radar_blm_ = value;
}
inline void UltrasonicRadar::set_short_radar_blm(float value) {
  _internal_set_short_radar_blm(value);
  // @@protoc_insertion_point(field_set:minieye.UltrasonicRadar.short_radar_blm)
}

// float short_radar_brm = 7;
inline void UltrasonicRadar::clear_short_radar_brm() {
  short_radar_brm_ = 0;
}
inline float UltrasonicRadar::_internal_short_radar_brm() const {
  return short_radar_brm_;
}
inline float UltrasonicRadar::short_radar_brm() const {
  // @@protoc_insertion_point(field_get:minieye.UltrasonicRadar.short_radar_brm)
  return _internal_short_radar_brm();
}
inline void UltrasonicRadar::_internal_set_short_radar_brm(float value) {
  
  short_radar_brm_ = value;
}
inline void UltrasonicRadar::set_short_radar_brm(float value) {
  _internal_set_short_radar_brm(value);
  // @@protoc_insertion_point(field_set:minieye.UltrasonicRadar.short_radar_brm)
}

// float short_radar_br = 8;
inline void UltrasonicRadar::clear_short_radar_br() {
  short_radar_br_ = 0;
}
inline float UltrasonicRadar::_internal_short_radar_br() const {
  return short_radar_br_;
}
inline float UltrasonicRadar::short_radar_br() const {
  // @@protoc_insertion_point(field_get:minieye.UltrasonicRadar.short_radar_br)
  return _internal_short_radar_br();
}
inline void UltrasonicRadar::_internal_set_short_radar_br(float value) {
  
  short_radar_br_ = value;
}
inline void UltrasonicRadar::set_short_radar_br(float value) {
  _internal_set_short_radar_br(value);
  // @@protoc_insertion_point(field_set:minieye.UltrasonicRadar.short_radar_br)
}

// float long_radar_fl = 9;
inline void UltrasonicRadar::clear_long_radar_fl() {
  long_radar_fl_ = 0;
}
inline float UltrasonicRadar::_internal_long_radar_fl() const {
  return long_radar_fl_;
}
inline float UltrasonicRadar::long_radar_fl() const {
  // @@protoc_insertion_point(field_get:minieye.UltrasonicRadar.long_radar_fl)
  return _internal_long_radar_fl();
}
inline void UltrasonicRadar::_internal_set_long_radar_fl(float value) {
  
  long_radar_fl_ = value;
}
inline void UltrasonicRadar::set_long_radar_fl(float value) {
  _internal_set_long_radar_fl(value);
  // @@protoc_insertion_point(field_set:minieye.UltrasonicRadar.long_radar_fl)
}

// float long_radar_fr = 10;
inline void UltrasonicRadar::clear_long_radar_fr() {
  long_radar_fr_ = 0;
}
inline float UltrasonicRadar::_internal_long_radar_fr() const {
  return long_radar_fr_;
}
inline float UltrasonicRadar::long_radar_fr() const {
  // @@protoc_insertion_point(field_get:minieye.UltrasonicRadar.long_radar_fr)
  return _internal_long_radar_fr();
}
inline void UltrasonicRadar::_internal_set_long_radar_fr(float value) {
  
  long_radar_fr_ = value;
}
inline void UltrasonicRadar::set_long_radar_fr(float value) {
  _internal_set_long_radar_fr(value);
  // @@protoc_insertion_point(field_set:minieye.UltrasonicRadar.long_radar_fr)
}

// float long_radar_bl = 11;
inline void UltrasonicRadar::clear_long_radar_bl() {
  long_radar_bl_ = 0;
}
inline float UltrasonicRadar::_internal_long_radar_bl() const {
  return long_radar_bl_;
}
inline float UltrasonicRadar::long_radar_bl() const {
  // @@protoc_insertion_point(field_get:minieye.UltrasonicRadar.long_radar_bl)
  return _internal_long_radar_bl();
}
inline void UltrasonicRadar::_internal_set_long_radar_bl(float value) {
  
  long_radar_bl_ = value;
}
inline void UltrasonicRadar::set_long_radar_bl(float value) {
  _internal_set_long_radar_bl(value);
  // @@protoc_insertion_point(field_set:minieye.UltrasonicRadar.long_radar_bl)
}

// float long_radar_br = 12;
inline void UltrasonicRadar::clear_long_radar_br() {
  long_radar_br_ = 0;
}
inline float UltrasonicRadar::_internal_long_radar_br() const {
  return long_radar_br_;
}
inline float UltrasonicRadar::long_radar_br() const {
  // @@protoc_insertion_point(field_get:minieye.UltrasonicRadar.long_radar_br)
  return _internal_long_radar_br();
}
inline void UltrasonicRadar::_internal_set_long_radar_br(float value) {
  
  long_radar_br_ = value;
}
inline void UltrasonicRadar::set_long_radar_br(float value) {
  _internal_set_long_radar_br(value);
  // @@protoc_insertion_point(field_set:minieye.UltrasonicRadar.long_radar_br)
}

// uint64 timestamp = 13;
inline void UltrasonicRadar::clear_timestamp() {
  timestamp_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 UltrasonicRadar::_internal_timestamp() const {
  return timestamp_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 UltrasonicRadar::timestamp() const {
  // @@protoc_insertion_point(field_get:minieye.UltrasonicRadar.timestamp)
  return _internal_timestamp();
}
inline void UltrasonicRadar::_internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  timestamp_ = value;
}
inline void UltrasonicRadar::set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_timestamp(value);
  // @@protoc_insertion_point(field_set:minieye.UltrasonicRadar.timestamp)
}

// uint64 tick = 14;
inline void UltrasonicRadar::clear_tick() {
  tick_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 UltrasonicRadar::_internal_tick() const {
  return tick_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 UltrasonicRadar::tick() const {
  // @@protoc_insertion_point(field_get:minieye.UltrasonicRadar.tick)
  return _internal_tick();
}
inline void UltrasonicRadar::_internal_set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  tick_ = value;
}
inline void UltrasonicRadar::set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_tick(value);
  // @@protoc_insertion_point(field_set:minieye.UltrasonicRadar.tick)
}

// uint64 frame_id = 15;
inline void UltrasonicRadar::clear_frame_id() {
  frame_id_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 UltrasonicRadar::_internal_frame_id() const {
  return frame_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 UltrasonicRadar::frame_id() const {
  // @@protoc_insertion_point(field_get:minieye.UltrasonicRadar.frame_id)
  return _internal_frame_id();
}
inline void UltrasonicRadar::_internal_set_frame_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  frame_id_ = value;
}
inline void UltrasonicRadar::set_frame_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_frame_id(value);
  // @@protoc_insertion_point(field_set:minieye.UltrasonicRadar.frame_id)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace minieye

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_ultra_5fradar_2eproto
