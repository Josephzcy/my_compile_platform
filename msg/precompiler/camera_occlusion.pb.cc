// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: camera_occlusion.proto

#include "camera_occlusion.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace perception {
class CameraOcclusionDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<CameraOcclusion> _instance;
} _CameraOcclusion_default_instance_;
}  // namespace perception
static void InitDefaultsscc_info_CameraOcclusion_camera_5focclusion_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::perception::_CameraOcclusion_default_instance_;
    new (ptr) ::perception::CameraOcclusion();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::perception::CameraOcclusion::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_CameraOcclusion_camera_5focclusion_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_CameraOcclusion_camera_5focclusion_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_camera_5focclusion_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_camera_5focclusion_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_camera_5focclusion_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_camera_5focclusion_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::perception::CameraOcclusion, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::perception::CameraOcclusion, frame_id_),
  PROTOBUF_FIELD_OFFSET(::perception::CameraOcclusion, timestamp_),
  PROTOBUF_FIELD_OFFSET(::perception::CameraOcclusion, camera_id_),
  PROTOBUF_FIELD_OFFSET(::perception::CameraOcclusion, camera_occlusion_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::perception::CameraOcclusion)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::perception::_CameraOcclusion_default_instance_),
};

const char descriptor_table_protodef_camera_5focclusion_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\026camera_occlusion.proto\022\nperception\"c\n\017"
  "CameraOcclusion\022\020\n\010frame_id\030\001 \001(\004\022\021\n\ttim"
  "estamp\030\002 \001(\004\022\021\n\tcamera_id\030\003 \001(\005\022\030\n\020camer"
  "a_occlusion\030\004 \001(\005b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_camera_5focclusion_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_camera_5focclusion_2eproto_sccs[1] = {
  &scc_info_CameraOcclusion_camera_5focclusion_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_camera_5focclusion_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_camera_5focclusion_2eproto = {
  false, false, descriptor_table_protodef_camera_5focclusion_2eproto, "camera_occlusion.proto", 145,
  &descriptor_table_camera_5focclusion_2eproto_once, descriptor_table_camera_5focclusion_2eproto_sccs, descriptor_table_camera_5focclusion_2eproto_deps, 1, 0,
  schemas, file_default_instances, TableStruct_camera_5focclusion_2eproto::offsets,
  file_level_metadata_camera_5focclusion_2eproto, 1, file_level_enum_descriptors_camera_5focclusion_2eproto, file_level_service_descriptors_camera_5focclusion_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_camera_5focclusion_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_camera_5focclusion_2eproto)), true);
namespace perception {

// ===================================================================

void CameraOcclusion::InitAsDefaultInstance() {
}
class CameraOcclusion::_Internal {
 public:
};

CameraOcclusion::CameraOcclusion(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:perception.CameraOcclusion)
}
CameraOcclusion::CameraOcclusion(const CameraOcclusion& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&frame_id_, &from.frame_id_,
    static_cast<size_t>(reinterpret_cast<char*>(&camera_occlusion_) -
    reinterpret_cast<char*>(&frame_id_)) + sizeof(camera_occlusion_));
  // @@protoc_insertion_point(copy_constructor:perception.CameraOcclusion)
}

void CameraOcclusion::SharedCtor() {
  ::memset(&frame_id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&camera_occlusion_) -
      reinterpret_cast<char*>(&frame_id_)) + sizeof(camera_occlusion_));
}

CameraOcclusion::~CameraOcclusion() {
  // @@protoc_insertion_point(destructor:perception.CameraOcclusion)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void CameraOcclusion::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void CameraOcclusion::ArenaDtor(void* object) {
  CameraOcclusion* _this = reinterpret_cast< CameraOcclusion* >(object);
  (void)_this;
}
void CameraOcclusion::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void CameraOcclusion::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const CameraOcclusion& CameraOcclusion::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_CameraOcclusion_camera_5focclusion_2eproto.base);
  return *internal_default_instance();
}


void CameraOcclusion::Clear() {
// @@protoc_insertion_point(message_clear_start:perception.CameraOcclusion)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  ::memset(&frame_id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&camera_occlusion_) -
      reinterpret_cast<char*>(&frame_id_)) + sizeof(camera_occlusion_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* CameraOcclusion::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // uint64 frame_id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          frame_id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // uint64 timestamp = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          timestamp_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 camera_id = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          camera_id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // int32 camera_occlusion = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 32)) {
          camera_occlusion_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      default: {
      handle_unusual:
        if ((tag & 7) == 4 || tag == 0) {
          ctx->SetLastTag(tag);
          goto success;
        }
        ptr = UnknownFieldParse(tag,
            _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
            ptr, ctx);
        CHK_(ptr != nullptr);
        continue;
      }
    }  // switch
  }  // while
success:
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* CameraOcclusion::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:perception.CameraOcclusion)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // uint64 frame_id = 1;
  if (this->frame_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(1, this->_internal_frame_id(), target);
  }

  // uint64 timestamp = 2;
  if (this->timestamp() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(2, this->_internal_timestamp(), target);
  }

  // int32 camera_id = 3;
  if (this->camera_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(3, this->_internal_camera_id(), target);
  }

  // int32 camera_occlusion = 4;
  if (this->camera_occlusion() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(4, this->_internal_camera_occlusion(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:perception.CameraOcclusion)
  return target;
}

size_t CameraOcclusion::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:perception.CameraOcclusion)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // uint64 frame_id = 1;
  if (this->frame_id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_frame_id());
  }

  // uint64 timestamp = 2;
  if (this->timestamp() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_timestamp());
  }

  // int32 camera_id = 3;
  if (this->camera_id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_camera_id());
  }

  // int32 camera_occlusion = 4;
  if (this->camera_occlusion() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_camera_occlusion());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void CameraOcclusion::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:perception.CameraOcclusion)
  GOOGLE_DCHECK_NE(&from, this);
  const CameraOcclusion* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<CameraOcclusion>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:perception.CameraOcclusion)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:perception.CameraOcclusion)
    MergeFrom(*source);
  }
}

void CameraOcclusion::MergeFrom(const CameraOcclusion& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:perception.CameraOcclusion)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.frame_id() != 0) {
    _internal_set_frame_id(from._internal_frame_id());
  }
  if (from.timestamp() != 0) {
    _internal_set_timestamp(from._internal_timestamp());
  }
  if (from.camera_id() != 0) {
    _internal_set_camera_id(from._internal_camera_id());
  }
  if (from.camera_occlusion() != 0) {
    _internal_set_camera_occlusion(from._internal_camera_occlusion());
  }
}

void CameraOcclusion::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:perception.CameraOcclusion)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void CameraOcclusion::CopyFrom(const CameraOcclusion& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:perception.CameraOcclusion)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool CameraOcclusion::IsInitialized() const {
  return true;
}

void CameraOcclusion::InternalSwap(CameraOcclusion* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(CameraOcclusion, camera_occlusion_)
      + sizeof(CameraOcclusion::camera_occlusion_)
      - PROTOBUF_FIELD_OFFSET(CameraOcclusion, frame_id_)>(
          reinterpret_cast<char*>(&frame_id_),
          reinterpret_cast<char*>(&other->frame_id_));
}

::PROTOBUF_NAMESPACE_ID::Metadata CameraOcclusion::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace perception
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::perception::CameraOcclusion* Arena::CreateMaybeMessage< ::perception::CameraOcclusion >(Arena* arena) {
  return Arena::CreateMessageInternal< ::perception::CameraOcclusion >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
