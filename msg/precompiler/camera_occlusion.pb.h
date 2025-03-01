// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: camera_occlusion.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_camera_5focclusion_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_camera_5focclusion_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_camera_5focclusion_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_camera_5focclusion_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_camera_5focclusion_2eproto;
namespace perception {
class CameraOcclusion;
class CameraOcclusionDefaultTypeInternal;
extern CameraOcclusionDefaultTypeInternal _CameraOcclusion_default_instance_;
}  // namespace perception
PROTOBUF_NAMESPACE_OPEN
template<> ::perception::CameraOcclusion* Arena::CreateMaybeMessage<::perception::CameraOcclusion>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace perception {

// ===================================================================

class CameraOcclusion PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:perception.CameraOcclusion) */ {
 public:
  inline CameraOcclusion() : CameraOcclusion(nullptr) {};
  virtual ~CameraOcclusion();

  CameraOcclusion(const CameraOcclusion& from);
  CameraOcclusion(CameraOcclusion&& from) noexcept
    : CameraOcclusion() {
    *this = ::std::move(from);
  }

  inline CameraOcclusion& operator=(const CameraOcclusion& from) {
    CopyFrom(from);
    return *this;
  }
  inline CameraOcclusion& operator=(CameraOcclusion&& from) noexcept {
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
  static const CameraOcclusion& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const CameraOcclusion* internal_default_instance() {
    return reinterpret_cast<const CameraOcclusion*>(
               &_CameraOcclusion_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(CameraOcclusion& a, CameraOcclusion& b) {
    a.Swap(&b);
  }
  inline void Swap(CameraOcclusion* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(CameraOcclusion* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline CameraOcclusion* New() const final {
    return CreateMaybeMessage<CameraOcclusion>(nullptr);
  }

  CameraOcclusion* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<CameraOcclusion>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const CameraOcclusion& from);
  void MergeFrom(const CameraOcclusion& from);
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
  void InternalSwap(CameraOcclusion* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "perception.CameraOcclusion";
  }
  protected:
  explicit CameraOcclusion(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_camera_5focclusion_2eproto);
    return ::descriptor_table_camera_5focclusion_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kFrameIdFieldNumber = 1,
    kTimestampFieldNumber = 2,
    kCameraIdFieldNumber = 3,
    kCameraOcclusionFieldNumber = 4,
  };
  // uint64 frame_id = 1;
  void clear_frame_id();
  ::PROTOBUF_NAMESPACE_ID::uint64 frame_id() const;
  void set_frame_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_frame_id() const;
  void _internal_set_frame_id(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // uint64 timestamp = 2;
  void clear_timestamp();
  ::PROTOBUF_NAMESPACE_ID::uint64 timestamp() const;
  void set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_timestamp() const;
  void _internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // int32 camera_id = 3;
  void clear_camera_id();
  ::PROTOBUF_NAMESPACE_ID::int32 camera_id() const;
  void set_camera_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_camera_id() const;
  void _internal_set_camera_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // int32 camera_occlusion = 4;
  void clear_camera_occlusion();
  ::PROTOBUF_NAMESPACE_ID::int32 camera_occlusion() const;
  void set_camera_occlusion(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_camera_occlusion() const;
  void _internal_set_camera_occlusion(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // @@protoc_insertion_point(class_scope:perception.CameraOcclusion)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::uint64 frame_id_;
  ::PROTOBUF_NAMESPACE_ID::uint64 timestamp_;
  ::PROTOBUF_NAMESPACE_ID::int32 camera_id_;
  ::PROTOBUF_NAMESPACE_ID::int32 camera_occlusion_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_camera_5focclusion_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// CameraOcclusion

// uint64 frame_id = 1;
inline void CameraOcclusion::clear_frame_id() {
  frame_id_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 CameraOcclusion::_internal_frame_id() const {
  return frame_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 CameraOcclusion::frame_id() const {
  // @@protoc_insertion_point(field_get:perception.CameraOcclusion.frame_id)
  return _internal_frame_id();
}
inline void CameraOcclusion::_internal_set_frame_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  frame_id_ = value;
}
inline void CameraOcclusion::set_frame_id(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_frame_id(value);
  // @@protoc_insertion_point(field_set:perception.CameraOcclusion.frame_id)
}

// uint64 timestamp = 2;
inline void CameraOcclusion::clear_timestamp() {
  timestamp_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 CameraOcclusion::_internal_timestamp() const {
  return timestamp_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 CameraOcclusion::timestamp() const {
  // @@protoc_insertion_point(field_get:perception.CameraOcclusion.timestamp)
  return _internal_timestamp();
}
inline void CameraOcclusion::_internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  timestamp_ = value;
}
inline void CameraOcclusion::set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_timestamp(value);
  // @@protoc_insertion_point(field_set:perception.CameraOcclusion.timestamp)
}

// int32 camera_id = 3;
inline void CameraOcclusion::clear_camera_id() {
  camera_id_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 CameraOcclusion::_internal_camera_id() const {
  return camera_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 CameraOcclusion::camera_id() const {
  // @@protoc_insertion_point(field_get:perception.CameraOcclusion.camera_id)
  return _internal_camera_id();
}
inline void CameraOcclusion::_internal_set_camera_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  camera_id_ = value;
}
inline void CameraOcclusion::set_camera_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_camera_id(value);
  // @@protoc_insertion_point(field_set:perception.CameraOcclusion.camera_id)
}

// int32 camera_occlusion = 4;
inline void CameraOcclusion::clear_camera_occlusion() {
  camera_occlusion_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 CameraOcclusion::_internal_camera_occlusion() const {
  return camera_occlusion_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 CameraOcclusion::camera_occlusion() const {
  // @@protoc_insertion_point(field_get:perception.CameraOcclusion.camera_occlusion)
  return _internal_camera_occlusion();
}
inline void CameraOcclusion::_internal_set_camera_occlusion(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  camera_occlusion_ = value;
}
inline void CameraOcclusion::set_camera_occlusion(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_camera_occlusion(value);
  // @@protoc_insertion_point(field_set:perception.CameraOcclusion.camera_occlusion)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace perception

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_camera_5focclusion_2eproto
