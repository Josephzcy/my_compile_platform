// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: apa_odometry.proto

#include "apa_odometry.pb.h"

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
extern PROTOBUF_INTERNAL_EXPORT_apa_5fodometry_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_Pose_apa_5fodometry_2eproto;
extern PROTOBUF_INTERNAL_EXPORT_apa_5fodometry_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_PoseList_apa_5fodometry_2eproto;
extern PROTOBUF_INTERNAL_EXPORT_geometry_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_Point3f_geometry_2eproto;
namespace apa {
class PoseDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<Pose> _instance;
} _Pose_default_instance_;
class PoseListDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<PoseList> _instance;
} _PoseList_default_instance_;
class ApaOdometryDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<ApaOdometry> _instance;
} _ApaOdometry_default_instance_;
}  // namespace apa
static void InitDefaultsscc_info_ApaOdometry_apa_5fodometry_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::apa::_ApaOdometry_default_instance_;
    new (ptr) ::apa::ApaOdometry();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::apa::ApaOdometry::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<2> scc_info_ApaOdometry_apa_5fodometry_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 2, 0, InitDefaultsscc_info_ApaOdometry_apa_5fodometry_2eproto}, {
      &scc_info_Pose_apa_5fodometry_2eproto.base,
      &scc_info_PoseList_apa_5fodometry_2eproto.base,}};

static void InitDefaultsscc_info_Pose_apa_5fodometry_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::apa::_Pose_default_instance_;
    new (ptr) ::apa::Pose();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::apa::Pose::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_Pose_apa_5fodometry_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 1, 0, InitDefaultsscc_info_Pose_apa_5fodometry_2eproto}, {
      &scc_info_Point3f_geometry_2eproto.base,}};

static void InitDefaultsscc_info_PoseList_apa_5fodometry_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::apa::_PoseList_default_instance_;
    new (ptr) ::apa::PoseList();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::apa::PoseList::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_PoseList_apa_5fodometry_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 1, 0, InitDefaultsscc_info_PoseList_apa_5fodometry_2eproto}, {
      &scc_info_Pose_apa_5fodometry_2eproto.base,}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_apa_5fodometry_2eproto[3];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_apa_5fodometry_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_apa_5fodometry_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_apa_5fodometry_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::apa::Pose, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::apa::Pose, timestamp_us_),
  PROTOBUF_FIELD_OFFSET(::apa::Pose, pose_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::apa::PoseList, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::apa::PoseList, pose_list_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::apa::ApaOdometry, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::apa::ApaOdometry, timestamp_us_),
  PROTOBUF_FIELD_OFFSET(::apa::ApaOdometry, frame_id_),
  PROTOBUF_FIELD_OFFSET(::apa::ApaOdometry, current_pose_),
  PROTOBUF_FIELD_OFFSET(::apa::ApaOdometry, history_pose_),
  PROTOBUF_FIELD_OFFSET(::apa::ApaOdometry, velocity_),
  PROTOBUF_FIELD_OFFSET(::apa::ApaOdometry, distance_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::apa::Pose)},
  { 7, -1, sizeof(::apa::PoseList)},
  { 13, -1, sizeof(::apa::ApaOdometry)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::apa::_Pose_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::apa::_PoseList_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::apa::_ApaOdometry_default_instance_),
};

const char descriptor_table_protodef_apa_5fodometry_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\022apa_odometry.proto\022\003apa\032\016geometry.prot"
  "o\"F\n\004Pose\022\024\n\014timestamp_us\030\001 \001(\004\022(\n\004pose\030"
  "\002 \001(\0132\032.perception.common.Point3f\"(\n\010Pos"
  "eList\022\034\n\tpose_list\030\001 \003(\0132\t.apa.Pose\"\237\001\n\013"
  "ApaOdometry\022\024\n\014timestamp_us\030\001 \001(\004\022\020\n\010fra"
  "me_id\030\002 \001(\004\022\037\n\014current_pose\030\003 \001(\0132\t.apa."
  "Pose\022#\n\014history_pose\030\004 \001(\0132\r.apa.PoseLis"
  "t\022\020\n\010velocity\030\005 \001(\002\022\020\n\010distance\030\006 \001(\002b\006p"
  "roto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_apa_5fodometry_2eproto_deps[1] = {
  &::descriptor_table_geometry_2eproto,
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_apa_5fodometry_2eproto_sccs[3] = {
  &scc_info_ApaOdometry_apa_5fodometry_2eproto.base,
  &scc_info_Pose_apa_5fodometry_2eproto.base,
  &scc_info_PoseList_apa_5fodometry_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_apa_5fodometry_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_apa_5fodometry_2eproto = {
  false, false, descriptor_table_protodef_apa_5fodometry_2eproto, "apa_odometry.proto", 325,
  &descriptor_table_apa_5fodometry_2eproto_once, descriptor_table_apa_5fodometry_2eproto_sccs, descriptor_table_apa_5fodometry_2eproto_deps, 3, 1,
  schemas, file_default_instances, TableStruct_apa_5fodometry_2eproto::offsets,
  file_level_metadata_apa_5fodometry_2eproto, 3, file_level_enum_descriptors_apa_5fodometry_2eproto, file_level_service_descriptors_apa_5fodometry_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_apa_5fodometry_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_apa_5fodometry_2eproto)), true);
namespace apa {

// ===================================================================

void Pose::InitAsDefaultInstance() {
  ::apa::_Pose_default_instance_._instance.get_mutable()->pose_ = const_cast< ::perception::common::Point3f*>(
      ::perception::common::Point3f::internal_default_instance());
}
class Pose::_Internal {
 public:
  static const ::perception::common::Point3f& pose(const Pose* msg);
};

const ::perception::common::Point3f&
Pose::_Internal::pose(const Pose* msg) {
  return *msg->pose_;
}
void Pose::clear_pose() {
  if (GetArena() == nullptr && pose_ != nullptr) {
    delete pose_;
  }
  pose_ = nullptr;
}
Pose::Pose(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:apa.Pose)
}
Pose::Pose(const Pose& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  if (from._internal_has_pose()) {
    pose_ = new ::perception::common::Point3f(*from.pose_);
  } else {
    pose_ = nullptr;
  }
  timestamp_us_ = from.timestamp_us_;
  // @@protoc_insertion_point(copy_constructor:apa.Pose)
}

void Pose::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_Pose_apa_5fodometry_2eproto.base);
  ::memset(&pose_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&timestamp_us_) -
      reinterpret_cast<char*>(&pose_)) + sizeof(timestamp_us_));
}

Pose::~Pose() {
  // @@protoc_insertion_point(destructor:apa.Pose)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void Pose::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  if (this != internal_default_instance()) delete pose_;
}

void Pose::ArenaDtor(void* object) {
  Pose* _this = reinterpret_cast< Pose* >(object);
  (void)_this;
}
void Pose::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Pose::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Pose& Pose::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_Pose_apa_5fodometry_2eproto.base);
  return *internal_default_instance();
}


void Pose::Clear() {
// @@protoc_insertion_point(message_clear_start:apa.Pose)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArena() == nullptr && pose_ != nullptr) {
    delete pose_;
  }
  pose_ = nullptr;
  timestamp_us_ = PROTOBUF_ULONGLONG(0);
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Pose::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // uint64 timestamp_us = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          timestamp_us_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .perception.common.Point3f pose = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          ptr = ctx->ParseMessage(_internal_mutable_pose(), ptr);
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

::PROTOBUF_NAMESPACE_ID::uint8* Pose::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:apa.Pose)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // uint64 timestamp_us = 1;
  if (this->timestamp_us() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(1, this->_internal_timestamp_us(), target);
  }

  // .perception.common.Point3f pose = 2;
  if (this->has_pose()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        2, _Internal::pose(this), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:apa.Pose)
  return target;
}

size_t Pose::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:apa.Pose)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // .perception.common.Point3f pose = 2;
  if (this->has_pose()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *pose_);
  }

  // uint64 timestamp_us = 1;
  if (this->timestamp_us() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_timestamp_us());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Pose::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:apa.Pose)
  GOOGLE_DCHECK_NE(&from, this);
  const Pose* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Pose>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:apa.Pose)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:apa.Pose)
    MergeFrom(*source);
  }
}

void Pose::MergeFrom(const Pose& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:apa.Pose)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.has_pose()) {
    _internal_mutable_pose()->::perception::common::Point3f::MergeFrom(from._internal_pose());
  }
  if (from.timestamp_us() != 0) {
    _internal_set_timestamp_us(from._internal_timestamp_us());
  }
}

void Pose::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:apa.Pose)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Pose::CopyFrom(const Pose& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:apa.Pose)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Pose::IsInitialized() const {
  return true;
}

void Pose::InternalSwap(Pose* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Pose, timestamp_us_)
      + sizeof(Pose::timestamp_us_)
      - PROTOBUF_FIELD_OFFSET(Pose, pose_)>(
          reinterpret_cast<char*>(&pose_),
          reinterpret_cast<char*>(&other->pose_));
}

::PROTOBUF_NAMESPACE_ID::Metadata Pose::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void PoseList::InitAsDefaultInstance() {
}
class PoseList::_Internal {
 public:
};

PoseList::PoseList(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena),
  pose_list_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:apa.PoseList)
}
PoseList::PoseList(const PoseList& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      pose_list_(from.pose_list_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  // @@protoc_insertion_point(copy_constructor:apa.PoseList)
}

void PoseList::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_PoseList_apa_5fodometry_2eproto.base);
}

PoseList::~PoseList() {
  // @@protoc_insertion_point(destructor:apa.PoseList)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void PoseList::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void PoseList::ArenaDtor(void* object) {
  PoseList* _this = reinterpret_cast< PoseList* >(object);
  (void)_this;
}
void PoseList::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void PoseList::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const PoseList& PoseList::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_PoseList_apa_5fodometry_2eproto.base);
  return *internal_default_instance();
}


void PoseList::Clear() {
// @@protoc_insertion_point(message_clear_start:apa.PoseList)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  pose_list_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* PoseList::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // repeated .apa.Pose pose_list = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(_internal_add_pose_list(), ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<10>(ptr));
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

::PROTOBUF_NAMESPACE_ID::uint8* PoseList::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:apa.PoseList)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated .apa.Pose pose_list = 1;
  for (unsigned int i = 0,
      n = static_cast<unsigned int>(this->_internal_pose_list_size()); i < n; i++) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(1, this->_internal_pose_list(i), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:apa.PoseList)
  return target;
}

size_t PoseList::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:apa.PoseList)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated .apa.Pose pose_list = 1;
  total_size += 1UL * this->_internal_pose_list_size();
  for (const auto& msg : this->pose_list_) {
    total_size +=
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(msg);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void PoseList::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:apa.PoseList)
  GOOGLE_DCHECK_NE(&from, this);
  const PoseList* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<PoseList>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:apa.PoseList)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:apa.PoseList)
    MergeFrom(*source);
  }
}

void PoseList::MergeFrom(const PoseList& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:apa.PoseList)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  pose_list_.MergeFrom(from.pose_list_);
}

void PoseList::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:apa.PoseList)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PoseList::CopyFrom(const PoseList& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:apa.PoseList)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PoseList::IsInitialized() const {
  return true;
}

void PoseList::InternalSwap(PoseList* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  pose_list_.InternalSwap(&other->pose_list_);
}

::PROTOBUF_NAMESPACE_ID::Metadata PoseList::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void ApaOdometry::InitAsDefaultInstance() {
  ::apa::_ApaOdometry_default_instance_._instance.get_mutable()->current_pose_ = const_cast< ::apa::Pose*>(
      ::apa::Pose::internal_default_instance());
  ::apa::_ApaOdometry_default_instance_._instance.get_mutable()->history_pose_ = const_cast< ::apa::PoseList*>(
      ::apa::PoseList::internal_default_instance());
}
class ApaOdometry::_Internal {
 public:
  static const ::apa::Pose& current_pose(const ApaOdometry* msg);
  static const ::apa::PoseList& history_pose(const ApaOdometry* msg);
};

const ::apa::Pose&
ApaOdometry::_Internal::current_pose(const ApaOdometry* msg) {
  return *msg->current_pose_;
}
const ::apa::PoseList&
ApaOdometry::_Internal::history_pose(const ApaOdometry* msg) {
  return *msg->history_pose_;
}
ApaOdometry::ApaOdometry(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:apa.ApaOdometry)
}
ApaOdometry::ApaOdometry(const ApaOdometry& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  if (from._internal_has_current_pose()) {
    current_pose_ = new ::apa::Pose(*from.current_pose_);
  } else {
    current_pose_ = nullptr;
  }
  if (from._internal_has_history_pose()) {
    history_pose_ = new ::apa::PoseList(*from.history_pose_);
  } else {
    history_pose_ = nullptr;
  }
  ::memcpy(&timestamp_us_, &from.timestamp_us_,
    static_cast<size_t>(reinterpret_cast<char*>(&distance_) -
    reinterpret_cast<char*>(&timestamp_us_)) + sizeof(distance_));
  // @@protoc_insertion_point(copy_constructor:apa.ApaOdometry)
}

void ApaOdometry::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_ApaOdometry_apa_5fodometry_2eproto.base);
  ::memset(&current_pose_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&distance_) -
      reinterpret_cast<char*>(&current_pose_)) + sizeof(distance_));
}

ApaOdometry::~ApaOdometry() {
  // @@protoc_insertion_point(destructor:apa.ApaOdometry)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void ApaOdometry::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  if (this != internal_default_instance()) delete current_pose_;
  if (this != internal_default_instance()) delete history_pose_;
}

void ApaOdometry::ArenaDtor(void* object) {
  ApaOdometry* _this = reinterpret_cast< ApaOdometry* >(object);
  (void)_this;
}
void ApaOdometry::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void ApaOdometry::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ApaOdometry& ApaOdometry::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_ApaOdometry_apa_5fodometry_2eproto.base);
  return *internal_default_instance();
}


void ApaOdometry::Clear() {
// @@protoc_insertion_point(message_clear_start:apa.ApaOdometry)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArena() == nullptr && current_pose_ != nullptr) {
    delete current_pose_;
  }
  current_pose_ = nullptr;
  if (GetArena() == nullptr && history_pose_ != nullptr) {
    delete history_pose_;
  }
  history_pose_ = nullptr;
  ::memset(&timestamp_us_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&distance_) -
      reinterpret_cast<char*>(&timestamp_us_)) + sizeof(distance_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* ApaOdometry::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // uint64 timestamp_us = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          timestamp_us_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // uint64 frame_id = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          frame_id_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .apa.Pose current_pose = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          ptr = ctx->ParseMessage(_internal_mutable_current_pose(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .apa.PoseList history_pose = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 34)) {
          ptr = ctx->ParseMessage(_internal_mutable_history_pose(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // float velocity = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 45)) {
          velocity_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // float distance = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 53)) {
          distance_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
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

::PROTOBUF_NAMESPACE_ID::uint8* ApaOdometry::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:apa.ApaOdometry)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // uint64 timestamp_us = 1;
  if (this->timestamp_us() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(1, this->_internal_timestamp_us(), target);
  }

  // uint64 frame_id = 2;
  if (this->frame_id() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(2, this->_internal_frame_id(), target);
  }

  // .apa.Pose current_pose = 3;
  if (this->has_current_pose()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        3, _Internal::current_pose(this), target, stream);
  }

  // .apa.PoseList history_pose = 4;
  if (this->has_history_pose()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        4, _Internal::history_pose(this), target, stream);
  }

  // float velocity = 5;
  if (!(this->velocity() <= 0 && this->velocity() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(5, this->_internal_velocity(), target);
  }

  // float distance = 6;
  if (!(this->distance() <= 0 && this->distance() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(6, this->_internal_distance(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:apa.ApaOdometry)
  return target;
}

size_t ApaOdometry::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:apa.ApaOdometry)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // .apa.Pose current_pose = 3;
  if (this->has_current_pose()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *current_pose_);
  }

  // .apa.PoseList history_pose = 4;
  if (this->has_history_pose()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *history_pose_);
  }

  // uint64 timestamp_us = 1;
  if (this->timestamp_us() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_timestamp_us());
  }

  // uint64 frame_id = 2;
  if (this->frame_id() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_frame_id());
  }

  // float velocity = 5;
  if (!(this->velocity() <= 0 && this->velocity() >= 0)) {
    total_size += 1 + 4;
  }

  // float distance = 6;
  if (!(this->distance() <= 0 && this->distance() >= 0)) {
    total_size += 1 + 4;
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void ApaOdometry::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:apa.ApaOdometry)
  GOOGLE_DCHECK_NE(&from, this);
  const ApaOdometry* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<ApaOdometry>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:apa.ApaOdometry)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:apa.ApaOdometry)
    MergeFrom(*source);
  }
}

void ApaOdometry::MergeFrom(const ApaOdometry& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:apa.ApaOdometry)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.has_current_pose()) {
    _internal_mutable_current_pose()->::apa::Pose::MergeFrom(from._internal_current_pose());
  }
  if (from.has_history_pose()) {
    _internal_mutable_history_pose()->::apa::PoseList::MergeFrom(from._internal_history_pose());
  }
  if (from.timestamp_us() != 0) {
    _internal_set_timestamp_us(from._internal_timestamp_us());
  }
  if (from.frame_id() != 0) {
    _internal_set_frame_id(from._internal_frame_id());
  }
  if (!(from.velocity() <= 0 && from.velocity() >= 0)) {
    _internal_set_velocity(from._internal_velocity());
  }
  if (!(from.distance() <= 0 && from.distance() >= 0)) {
    _internal_set_distance(from._internal_distance());
  }
}

void ApaOdometry::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:apa.ApaOdometry)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ApaOdometry::CopyFrom(const ApaOdometry& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:apa.ApaOdometry)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ApaOdometry::IsInitialized() const {
  return true;
}

void ApaOdometry::InternalSwap(ApaOdometry* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(ApaOdometry, distance_)
      + sizeof(ApaOdometry::distance_)
      - PROTOBUF_FIELD_OFFSET(ApaOdometry, current_pose_)>(
          reinterpret_cast<char*>(&current_pose_),
          reinterpret_cast<char*>(&other->current_pose_));
}

::PROTOBUF_NAMESPACE_ID::Metadata ApaOdometry::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace apa
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::apa::Pose* Arena::CreateMaybeMessage< ::apa::Pose >(Arena* arena) {
  return Arena::CreateMessageInternal< ::apa::Pose >(arena);
}
template<> PROTOBUF_NOINLINE ::apa::PoseList* Arena::CreateMaybeMessage< ::apa::PoseList >(Arena* arena) {
  return Arena::CreateMessageInternal< ::apa::PoseList >(arena);
}
template<> PROTOBUF_NOINLINE ::apa::ApaOdometry* Arena::CreateMaybeMessage< ::apa::ApaOdometry >(Arena* arena) {
  return Arena::CreateMessageInternal< ::apa::ApaOdometry >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
