// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: planningtohmi.proto

#include "planningtohmi.pb.h"

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
extern PROTOBUF_INTERNAL_EXPORT_planningtohmi_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_ParkID_planningtohmi_2eproto;
extern PROTOBUF_INTERNAL_EXPORT_planningtohmi_2eproto ::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_ParkID_IdListEntry_DoNotUse_planningtohmi_2eproto;
namespace minieye {
class ParkID_IdListEntry_DoNotUseDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<ParkID_IdListEntry_DoNotUse> _instance;
} _ParkID_IdListEntry_DoNotUse_default_instance_;
class ParkIDDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<ParkID> _instance;
} _ParkID_default_instance_;
class PlanningToHMIDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<PlanningToHMI> _instance;
} _PlanningToHMI_default_instance_;
}  // namespace minieye
static void InitDefaultsscc_info_ParkID_planningtohmi_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::minieye::_ParkID_default_instance_;
    new (ptr) ::minieye::ParkID();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::minieye::ParkID::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_ParkID_planningtohmi_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 1, 0, InitDefaultsscc_info_ParkID_planningtohmi_2eproto}, {
      &scc_info_ParkID_IdListEntry_DoNotUse_planningtohmi_2eproto.base,}};

static void InitDefaultsscc_info_ParkID_IdListEntry_DoNotUse_planningtohmi_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::minieye::_ParkID_IdListEntry_DoNotUse_default_instance_;
    new (ptr) ::minieye::ParkID_IdListEntry_DoNotUse();
  }
  ::minieye::ParkID_IdListEntry_DoNotUse::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_ParkID_IdListEntry_DoNotUse_planningtohmi_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_ParkID_IdListEntry_DoNotUse_planningtohmi_2eproto}, {}};

static void InitDefaultsscc_info_PlanningToHMI_planningtohmi_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::minieye::_PlanningToHMI_default_instance_;
    new (ptr) ::minieye::PlanningToHMI();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
  ::minieye::PlanningToHMI::InitAsDefaultInstance();
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<1> scc_info_PlanningToHMI_planningtohmi_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 1, 0, InitDefaultsscc_info_PlanningToHMI_planningtohmi_2eproto}, {
      &scc_info_ParkID_planningtohmi_2eproto.base,}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_planningtohmi_2eproto[3];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_planningtohmi_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_planningtohmi_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_planningtohmi_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::minieye::ParkID_IdListEntry_DoNotUse, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::minieye::ParkID_IdListEntry_DoNotUse, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::minieye::ParkID_IdListEntry_DoNotUse, key_),
  PROTOBUF_FIELD_OFFSET(::minieye::ParkID_IdListEntry_DoNotUse, value_),
  0,
  1,
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::minieye::ParkID, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::minieye::ParkID, recommend_id_),
  PROTOBUF_FIELD_OFFSET(::minieye::ParkID, id_list_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::minieye::PlanningToHMI, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::minieye::PlanningToHMI, timestamp_),
  PROTOBUF_FIELD_OFFSET(::minieye::PlanningToHMI, tick_),
  PROTOBUF_FIELD_OFFSET(::minieye::PlanningToHMI, park_id_),
  PROTOBUF_FIELD_OFFSET(::minieye::PlanningToHMI, current_stage_),
  PROTOBUF_FIELD_OFFSET(::minieye::PlanningToHMI, remain_distance_),
  PROTOBUF_FIELD_OFFSET(::minieye::PlanningToHMI, gear_),
  PROTOBUF_FIELD_OFFSET(::minieye::PlanningToHMI, has_obstacle_),
  PROTOBUF_FIELD_OFFSET(::minieye::PlanningToHMI, is_left_park_out_),
  PROTOBUF_FIELD_OFFSET(::minieye::PlanningToHMI, is_right_park_out_),
  PROTOBUF_FIELD_OFFSET(::minieye::PlanningToHMI, park_direction_),
  PROTOBUF_FIELD_OFFSET(::minieye::PlanningToHMI, slot_type_),
  PROTOBUF_FIELD_OFFSET(::minieye::PlanningToHMI, apa_type_),
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 7, sizeof(::minieye::ParkID_IdListEntry_DoNotUse)},
  { 9, -1, sizeof(::minieye::ParkID)},
  { 16, -1, sizeof(::minieye::PlanningToHMI)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::minieye::_ParkID_IdListEntry_DoNotUse_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::minieye::_ParkID_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::minieye::_PlanningToHMI_default_instance_),
};

const char descriptor_table_protodef_planningtohmi_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\023planningtohmi.proto\022\007minieye\"{\n\006ParkID"
  "\022\024\n\014recommend_id\030\001 \003(\005\022,\n\007id_list\030\002 \003(\0132"
  "\033.minieye.ParkID.IdListEntry\032-\n\013IdListEn"
  "try\022\013\n\003key\030\001 \001(\005\022\r\n\005value\030\002 \001(\005:\0028\001\"\230\002\n\r"
  "PlanningToHMI\022\021\n\ttimestamp\030\001 \001(\004\022\014\n\004tick"
  "\030\002 \001(\004\022 \n\007park_id\030\003 \001(\0132\017.minieye.ParkID"
  "\022\025\n\rcurrent_stage\030\004 \001(\002\022\027\n\017remain_distan"
  "ce\030\005 \001(\002\022\014\n\004gear\030\006 \001(\005\022\024\n\014has_obstacle\030\007"
  " \001(\010\022\030\n\020is_left_park_out\030\010 \001(\010\022\031\n\021is_rig"
  "ht_park_out\030\t \001(\010\022\026\n\016park_direction\030\n \001("
  "\r\022\021\n\tslot_type\030\013 \001(\r\022\020\n\010apa_type\030\014 \001(\rb\006"
  "proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_planningtohmi_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_planningtohmi_2eproto_sccs[3] = {
  &scc_info_ParkID_planningtohmi_2eproto.base,
  &scc_info_ParkID_IdListEntry_DoNotUse_planningtohmi_2eproto.base,
  &scc_info_PlanningToHMI_planningtohmi_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_planningtohmi_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_planningtohmi_2eproto = {
  false, false, descriptor_table_protodef_planningtohmi_2eproto, "planningtohmi.proto", 446,
  &descriptor_table_planningtohmi_2eproto_once, descriptor_table_planningtohmi_2eproto_sccs, descriptor_table_planningtohmi_2eproto_deps, 3, 0,
  schemas, file_default_instances, TableStruct_planningtohmi_2eproto::offsets,
  file_level_metadata_planningtohmi_2eproto, 3, file_level_enum_descriptors_planningtohmi_2eproto, file_level_service_descriptors_planningtohmi_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_planningtohmi_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_planningtohmi_2eproto)), true);
namespace minieye {

// ===================================================================

ParkID_IdListEntry_DoNotUse::ParkID_IdListEntry_DoNotUse() {}
ParkID_IdListEntry_DoNotUse::ParkID_IdListEntry_DoNotUse(::PROTOBUF_NAMESPACE_ID::Arena* arena)
    : SuperType(arena) {}
void ParkID_IdListEntry_DoNotUse::MergeFrom(const ParkID_IdListEntry_DoNotUse& other) {
  MergeFromInternal(other);
}
::PROTOBUF_NAMESPACE_ID::Metadata ParkID_IdListEntry_DoNotUse::GetMetadata() const {
  return GetMetadataStatic();
}
void ParkID_IdListEntry_DoNotUse::MergeFrom(
    const ::PROTOBUF_NAMESPACE_ID::Message& other) {
  ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom(other);
}


// ===================================================================

void ParkID::InitAsDefaultInstance() {
}
class ParkID::_Internal {
 public:
};

ParkID::ParkID(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena),
  recommend_id_(arena),
  id_list_(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:minieye.ParkID)
}
ParkID::ParkID(const ParkID& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      recommend_id_(from.recommend_id_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  id_list_.MergeFrom(from.id_list_);
  // @@protoc_insertion_point(copy_constructor:minieye.ParkID)
}

void ParkID::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_ParkID_planningtohmi_2eproto.base);
}

ParkID::~ParkID() {
  // @@protoc_insertion_point(destructor:minieye.ParkID)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void ParkID::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void ParkID::ArenaDtor(void* object) {
  ParkID* _this = reinterpret_cast< ParkID* >(object);
  (void)_this;
}
void ParkID::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void ParkID::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const ParkID& ParkID::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_ParkID_planningtohmi_2eproto.base);
  return *internal_default_instance();
}


void ParkID::Clear() {
// @@protoc_insertion_point(message_clear_start:minieye.ParkID)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  recommend_id_.Clear();
  id_list_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* ParkID::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // repeated int32 recommend_id = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 10)) {
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::PackedInt32Parser(_internal_mutable_recommend_id(), ptr, ctx);
          CHK_(ptr);
        } else if (static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8) {
          _internal_add_recommend_id(::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr));
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // map<int32, int32> id_list = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 18)) {
          ptr -= 1;
          do {
            ptr += 1;
            ptr = ctx->ParseMessage(&id_list_, ptr);
            CHK_(ptr);
            if (!ctx->DataAvailable(ptr)) break;
          } while (::PROTOBUF_NAMESPACE_ID::internal::ExpectTag<18>(ptr));
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

::PROTOBUF_NAMESPACE_ID::uint8* ParkID::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:minieye.ParkID)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // repeated int32 recommend_id = 1;
  {
    int byte_size = _recommend_id_cached_byte_size_.load(std::memory_order_relaxed);
    if (byte_size > 0) {
      target = stream->WriteInt32Packed(
          1, _internal_recommend_id(), byte_size, target);
    }
  }

  // map<int32, int32> id_list = 2;
  if (!this->_internal_id_list().empty()) {
    typedef ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >::const_pointer
        ConstPtr;
    typedef ::PROTOBUF_NAMESPACE_ID::internal::SortItem< ::PROTOBUF_NAMESPACE_ID::int32, ConstPtr > SortItem;
    typedef ::PROTOBUF_NAMESPACE_ID::internal::CompareByFirstField<SortItem> Less;

    if (stream->IsSerializationDeterministic() &&
        this->_internal_id_list().size() > 1) {
      ::std::unique_ptr<SortItem[]> items(
          new SortItem[this->_internal_id_list().size()]);
      typedef ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >::size_type size_type;
      size_type n = 0;
      for (::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >::const_iterator
          it = this->_internal_id_list().begin();
          it != this->_internal_id_list().end(); ++it, ++n) {
        items[static_cast<ptrdiff_t>(n)] = SortItem(&*it);
      }
      ::std::sort(&items[0], &items[static_cast<ptrdiff_t>(n)], Less());
      for (size_type i = 0; i < n; i++) {
        target = ParkID_IdListEntry_DoNotUse::Funcs::InternalSerialize(2, items[static_cast<ptrdiff_t>(i)].second->first, items[static_cast<ptrdiff_t>(i)].second->second, target, stream);
      }
    } else {
      for (::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >::const_iterator
          it = this->_internal_id_list().begin();
          it != this->_internal_id_list().end(); ++it) {
        target = ParkID_IdListEntry_DoNotUse::Funcs::InternalSerialize(2, it->first, it->second, target, stream);
      }
    }
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:minieye.ParkID)
  return target;
}

size_t ParkID::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:minieye.ParkID)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // repeated int32 recommend_id = 1;
  {
    size_t data_size = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      Int32Size(this->recommend_id_);
    if (data_size > 0) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
            static_cast<::PROTOBUF_NAMESPACE_ID::int32>(data_size));
    }
    int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(data_size);
    _recommend_id_cached_byte_size_.store(cached_size,
                                    std::memory_order_relaxed);
    total_size += data_size;
  }

  // map<int32, int32> id_list = 2;
  total_size += 1 *
      ::PROTOBUF_NAMESPACE_ID::internal::FromIntSize(this->_internal_id_list_size());
  for (::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >::const_iterator
      it = this->_internal_id_list().begin();
      it != this->_internal_id_list().end(); ++it) {
    total_size += ParkID_IdListEntry_DoNotUse::Funcs::ByteSizeLong(it->first, it->second);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void ParkID::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:minieye.ParkID)
  GOOGLE_DCHECK_NE(&from, this);
  const ParkID* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<ParkID>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:minieye.ParkID)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:minieye.ParkID)
    MergeFrom(*source);
  }
}

void ParkID::MergeFrom(const ParkID& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:minieye.ParkID)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  recommend_id_.MergeFrom(from.recommend_id_);
  id_list_.MergeFrom(from.id_list_);
}

void ParkID::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:minieye.ParkID)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ParkID::CopyFrom(const ParkID& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:minieye.ParkID)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ParkID::IsInitialized() const {
  return true;
}

void ParkID::InternalSwap(ParkID* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  recommend_id_.InternalSwap(&other->recommend_id_);
  id_list_.Swap(&other->id_list_);
}

::PROTOBUF_NAMESPACE_ID::Metadata ParkID::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

void PlanningToHMI::InitAsDefaultInstance() {
  ::minieye::_PlanningToHMI_default_instance_._instance.get_mutable()->park_id_ = const_cast< ::minieye::ParkID*>(
      ::minieye::ParkID::internal_default_instance());
}
class PlanningToHMI::_Internal {
 public:
  static const ::minieye::ParkID& park_id(const PlanningToHMI* msg);
};

const ::minieye::ParkID&
PlanningToHMI::_Internal::park_id(const PlanningToHMI* msg) {
  return *msg->park_id_;
}
PlanningToHMI::PlanningToHMI(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:minieye.PlanningToHMI)
}
PlanningToHMI::PlanningToHMI(const PlanningToHMI& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  if (from._internal_has_park_id()) {
    park_id_ = new ::minieye::ParkID(*from.park_id_);
  } else {
    park_id_ = nullptr;
  }
  ::memcpy(&timestamp_, &from.timestamp_,
    static_cast<size_t>(reinterpret_cast<char*>(&apa_type_) -
    reinterpret_cast<char*>(&timestamp_)) + sizeof(apa_type_));
  // @@protoc_insertion_point(copy_constructor:minieye.PlanningToHMI)
}

void PlanningToHMI::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_PlanningToHMI_planningtohmi_2eproto.base);
  ::memset(&park_id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&apa_type_) -
      reinterpret_cast<char*>(&park_id_)) + sizeof(apa_type_));
}

PlanningToHMI::~PlanningToHMI() {
  // @@protoc_insertion_point(destructor:minieye.PlanningToHMI)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void PlanningToHMI::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  if (this != internal_default_instance()) delete park_id_;
}

void PlanningToHMI::ArenaDtor(void* object) {
  PlanningToHMI* _this = reinterpret_cast< PlanningToHMI* >(object);
  (void)_this;
}
void PlanningToHMI::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void PlanningToHMI::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const PlanningToHMI& PlanningToHMI::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_PlanningToHMI_planningtohmi_2eproto.base);
  return *internal_default_instance();
}


void PlanningToHMI::Clear() {
// @@protoc_insertion_point(message_clear_start:minieye.PlanningToHMI)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (GetArena() == nullptr && park_id_ != nullptr) {
    delete park_id_;
  }
  park_id_ = nullptr;
  ::memset(&timestamp_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&apa_type_) -
      reinterpret_cast<char*>(&timestamp_)) + sizeof(apa_type_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* PlanningToHMI::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  ::PROTOBUF_NAMESPACE_ID::Arena* arena = GetArena(); (void)arena;
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // uint64 timestamp = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          timestamp_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // uint64 tick = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          tick_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // .minieye.ParkID park_id = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 26)) {
          ptr = ctx->ParseMessage(_internal_mutable_park_id(), ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // float current_stage = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 37)) {
          current_stage_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // float remain_distance = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 45)) {
          remain_distance_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<float>(ptr);
          ptr += sizeof(float);
        } else goto handle_unusual;
        continue;
      // int32 gear = 6;
      case 6:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 48)) {
          gear_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // bool has_obstacle = 7;
      case 7:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 56)) {
          has_obstacle_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // bool is_left_park_out = 8;
      case 8:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 64)) {
          is_left_park_out_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // bool is_right_park_out = 9;
      case 9:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 72)) {
          is_right_park_out_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // uint32 park_direction = 10;
      case 10:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 80)) {
          park_direction_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // uint32 slot_type = 11;
      case 11:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 88)) {
          slot_type_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // uint32 apa_type = 12;
      case 12:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 96)) {
          apa_type_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
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

::PROTOBUF_NAMESPACE_ID::uint8* PlanningToHMI::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:minieye.PlanningToHMI)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // uint64 timestamp = 1;
  if (this->timestamp() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(1, this->_internal_timestamp(), target);
  }

  // uint64 tick = 2;
  if (this->tick() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt64ToArray(2, this->_internal_tick(), target);
  }

  // .minieye.ParkID park_id = 3;
  if (this->has_park_id()) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(
        3, _Internal::park_id(this), target, stream);
  }

  // float current_stage = 4;
  if (!(this->current_stage() <= 0 && this->current_stage() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(4, this->_internal_current_stage(), target);
  }

  // float remain_distance = 5;
  if (!(this->remain_distance() <= 0 && this->remain_distance() >= 0)) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFloatToArray(5, this->_internal_remain_distance(), target);
  }

  // int32 gear = 6;
  if (this->gear() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteInt32ToArray(6, this->_internal_gear(), target);
  }

  // bool has_obstacle = 7;
  if (this->has_obstacle() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(7, this->_internal_has_obstacle(), target);
  }

  // bool is_left_park_out = 8;
  if (this->is_left_park_out() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(8, this->_internal_is_left_park_out(), target);
  }

  // bool is_right_park_out = 9;
  if (this->is_right_park_out() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteBoolToArray(9, this->_internal_is_right_park_out(), target);
  }

  // uint32 park_direction = 10;
  if (this->park_direction() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(10, this->_internal_park_direction(), target);
  }

  // uint32 slot_type = 11;
  if (this->slot_type() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(11, this->_internal_slot_type(), target);
  }

  // uint32 apa_type = 12;
  if (this->apa_type() != 0) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(12, this->_internal_apa_type(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:minieye.PlanningToHMI)
  return target;
}

size_t PlanningToHMI::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:minieye.PlanningToHMI)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // .minieye.ParkID park_id = 3;
  if (this->has_park_id()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *park_id_);
  }

  // uint64 timestamp = 1;
  if (this->timestamp() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_timestamp());
  }

  // uint64 tick = 2;
  if (this->tick() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt64Size(
        this->_internal_tick());
  }

  // float current_stage = 4;
  if (!(this->current_stage() <= 0 && this->current_stage() >= 0)) {
    total_size += 1 + 4;
  }

  // float remain_distance = 5;
  if (!(this->remain_distance() <= 0 && this->remain_distance() >= 0)) {
    total_size += 1 + 4;
  }

  // int32 gear = 6;
  if (this->gear() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::Int32Size(
        this->_internal_gear());
  }

  // bool has_obstacle = 7;
  if (this->has_obstacle() != 0) {
    total_size += 1 + 1;
  }

  // bool is_left_park_out = 8;
  if (this->is_left_park_out() != 0) {
    total_size += 1 + 1;
  }

  // bool is_right_park_out = 9;
  if (this->is_right_park_out() != 0) {
    total_size += 1 + 1;
  }

  // uint32 park_direction = 10;
  if (this->park_direction() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_park_direction());
  }

  // uint32 slot_type = 11;
  if (this->slot_type() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_slot_type());
  }

  // uint32 apa_type = 12;
  if (this->apa_type() != 0) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_apa_type());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void PlanningToHMI::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:minieye.PlanningToHMI)
  GOOGLE_DCHECK_NE(&from, this);
  const PlanningToHMI* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<PlanningToHMI>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:minieye.PlanningToHMI)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:minieye.PlanningToHMI)
    MergeFrom(*source);
  }
}

void PlanningToHMI::MergeFrom(const PlanningToHMI& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:minieye.PlanningToHMI)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from.has_park_id()) {
    _internal_mutable_park_id()->::minieye::ParkID::MergeFrom(from._internal_park_id());
  }
  if (from.timestamp() != 0) {
    _internal_set_timestamp(from._internal_timestamp());
  }
  if (from.tick() != 0) {
    _internal_set_tick(from._internal_tick());
  }
  if (!(from.current_stage() <= 0 && from.current_stage() >= 0)) {
    _internal_set_current_stage(from._internal_current_stage());
  }
  if (!(from.remain_distance() <= 0 && from.remain_distance() >= 0)) {
    _internal_set_remain_distance(from._internal_remain_distance());
  }
  if (from.gear() != 0) {
    _internal_set_gear(from._internal_gear());
  }
  if (from.has_obstacle() != 0) {
    _internal_set_has_obstacle(from._internal_has_obstacle());
  }
  if (from.is_left_park_out() != 0) {
    _internal_set_is_left_park_out(from._internal_is_left_park_out());
  }
  if (from.is_right_park_out() != 0) {
    _internal_set_is_right_park_out(from._internal_is_right_park_out());
  }
  if (from.park_direction() != 0) {
    _internal_set_park_direction(from._internal_park_direction());
  }
  if (from.slot_type() != 0) {
    _internal_set_slot_type(from._internal_slot_type());
  }
  if (from.apa_type() != 0) {
    _internal_set_apa_type(from._internal_apa_type());
  }
}

void PlanningToHMI::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:minieye.PlanningToHMI)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PlanningToHMI::CopyFrom(const PlanningToHMI& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:minieye.PlanningToHMI)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PlanningToHMI::IsInitialized() const {
  return true;
}

void PlanningToHMI::InternalSwap(PlanningToHMI* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(PlanningToHMI, apa_type_)
      + sizeof(PlanningToHMI::apa_type_)
      - PROTOBUF_FIELD_OFFSET(PlanningToHMI, park_id_)>(
          reinterpret_cast<char*>(&park_id_),
          reinterpret_cast<char*>(&other->park_id_));
}

::PROTOBUF_NAMESPACE_ID::Metadata PlanningToHMI::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace minieye
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::minieye::ParkID_IdListEntry_DoNotUse* Arena::CreateMaybeMessage< ::minieye::ParkID_IdListEntry_DoNotUse >(Arena* arena) {
  return Arena::CreateMessageInternal< ::minieye::ParkID_IdListEntry_DoNotUse >(arena);
}
template<> PROTOBUF_NOINLINE ::minieye::ParkID* Arena::CreateMaybeMessage< ::minieye::ParkID >(Arena* arena) {
  return Arena::CreateMessageInternal< ::minieye::ParkID >(arena);
}
template<> PROTOBUF_NOINLINE ::minieye::PlanningToHMI* Arena::CreateMaybeMessage< ::minieye::PlanningToHMI >(Arena* arena) {
  return Arena::CreateMessageInternal< ::minieye::PlanningToHMI >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
