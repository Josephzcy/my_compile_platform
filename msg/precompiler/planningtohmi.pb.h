// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: planningtohmi.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_planningtohmi_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_planningtohmi_2eproto

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
#include <google/protobuf/map.h>  // IWYU pragma: export
#include <google/protobuf/map_entry.h>
#include <google/protobuf/map_field_inl.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_planningtohmi_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_planningtohmi_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_planningtohmi_2eproto;
namespace minieye {
class ParkID;
class ParkIDDefaultTypeInternal;
extern ParkIDDefaultTypeInternal _ParkID_default_instance_;
class ParkID_IdListEntry_DoNotUse;
class ParkID_IdListEntry_DoNotUseDefaultTypeInternal;
extern ParkID_IdListEntry_DoNotUseDefaultTypeInternal _ParkID_IdListEntry_DoNotUse_default_instance_;
class PlanningToHMI;
class PlanningToHMIDefaultTypeInternal;
extern PlanningToHMIDefaultTypeInternal _PlanningToHMI_default_instance_;
}  // namespace minieye
PROTOBUF_NAMESPACE_OPEN
template<> ::minieye::ParkID* Arena::CreateMaybeMessage<::minieye::ParkID>(Arena*);
template<> ::minieye::ParkID_IdListEntry_DoNotUse* Arena::CreateMaybeMessage<::minieye::ParkID_IdListEntry_DoNotUse>(Arena*);
template<> ::minieye::PlanningToHMI* Arena::CreateMaybeMessage<::minieye::PlanningToHMI>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace minieye {

// ===================================================================

class ParkID_IdListEntry_DoNotUse : public ::PROTOBUF_NAMESPACE_ID::internal::MapEntry<ParkID_IdListEntry_DoNotUse, 
    ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32,
    0 > {
public:
  typedef ::PROTOBUF_NAMESPACE_ID::internal::MapEntry<ParkID_IdListEntry_DoNotUse, 
    ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32,
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32,
    0 > SuperType;
  ParkID_IdListEntry_DoNotUse();
  ParkID_IdListEntry_DoNotUse(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  void MergeFrom(const ParkID_IdListEntry_DoNotUse& other);
  static const ParkID_IdListEntry_DoNotUse* internal_default_instance() { return reinterpret_cast<const ParkID_IdListEntry_DoNotUse*>(&_ParkID_IdListEntry_DoNotUse_default_instance_); }
  static bool ValidateKey(void*) { return true; }
  static bool ValidateValue(void*) { return true; }
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& other) final;
  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_planningtohmi_2eproto);
    return ::descriptor_table_planningtohmi_2eproto.file_level_metadata[0];
  }

  public:
};

// -------------------------------------------------------------------

class ParkID PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:minieye.ParkID) */ {
 public:
  inline ParkID() : ParkID(nullptr) {};
  virtual ~ParkID();

  ParkID(const ParkID& from);
  ParkID(ParkID&& from) noexcept
    : ParkID() {
    *this = ::std::move(from);
  }

  inline ParkID& operator=(const ParkID& from) {
    CopyFrom(from);
    return *this;
  }
  inline ParkID& operator=(ParkID&& from) noexcept {
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
  static const ParkID& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const ParkID* internal_default_instance() {
    return reinterpret_cast<const ParkID*>(
               &_ParkID_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(ParkID& a, ParkID& b) {
    a.Swap(&b);
  }
  inline void Swap(ParkID* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(ParkID* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline ParkID* New() const final {
    return CreateMaybeMessage<ParkID>(nullptr);
  }

  ParkID* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<ParkID>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const ParkID& from);
  void MergeFrom(const ParkID& from);
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
  void InternalSwap(ParkID* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "minieye.ParkID";
  }
  protected:
  explicit ParkID(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_planningtohmi_2eproto);
    return ::descriptor_table_planningtohmi_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------


  // accessors -------------------------------------------------------

  enum : int {
    kRecommendIdFieldNumber = 1,
    kIdListFieldNumber = 2,
  };
  // repeated int32 recommend_id = 1;
  int recommend_id_size() const;
  private:
  int _internal_recommend_id_size() const;
  public:
  void clear_recommend_id();
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_recommend_id(int index) const;
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >&
      _internal_recommend_id() const;
  void _internal_add_recommend_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >*
      _internal_mutable_recommend_id();
  public:
  ::PROTOBUF_NAMESPACE_ID::int32 recommend_id(int index) const;
  void set_recommend_id(int index, ::PROTOBUF_NAMESPACE_ID::int32 value);
  void add_recommend_id(::PROTOBUF_NAMESPACE_ID::int32 value);
  const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >&
      recommend_id() const;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >*
      mutable_recommend_id();

  // map<int32, int32> id_list = 2;
  int id_list_size() const;
  private:
  int _internal_id_list_size() const;
  public:
  void clear_id_list();
  private:
  const ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >&
      _internal_id_list() const;
  ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >*
      _internal_mutable_id_list();
  public:
  const ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >&
      id_list() const;
  ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >*
      mutable_id_list();

  // @@protoc_insertion_point(class_scope:minieye.ParkID)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 > recommend_id_;
  mutable std::atomic<int> _recommend_id_cached_byte_size_;
  ::PROTOBUF_NAMESPACE_ID::internal::MapField<
      ParkID_IdListEntry_DoNotUse,
      ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32,
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32,
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::TYPE_INT32,
      0 > id_list_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_planningtohmi_2eproto;
};
// -------------------------------------------------------------------

class PlanningToHMI PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:minieye.PlanningToHMI) */ {
 public:
  inline PlanningToHMI() : PlanningToHMI(nullptr) {};
  virtual ~PlanningToHMI();

  PlanningToHMI(const PlanningToHMI& from);
  PlanningToHMI(PlanningToHMI&& from) noexcept
    : PlanningToHMI() {
    *this = ::std::move(from);
  }

  inline PlanningToHMI& operator=(const PlanningToHMI& from) {
    CopyFrom(from);
    return *this;
  }
  inline PlanningToHMI& operator=(PlanningToHMI&& from) noexcept {
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
  static const PlanningToHMI& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const PlanningToHMI* internal_default_instance() {
    return reinterpret_cast<const PlanningToHMI*>(
               &_PlanningToHMI_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    2;

  friend void swap(PlanningToHMI& a, PlanningToHMI& b) {
    a.Swap(&b);
  }
  inline void Swap(PlanningToHMI* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(PlanningToHMI* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline PlanningToHMI* New() const final {
    return CreateMaybeMessage<PlanningToHMI>(nullptr);
  }

  PlanningToHMI* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<PlanningToHMI>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const PlanningToHMI& from);
  void MergeFrom(const PlanningToHMI& from);
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
  void InternalSwap(PlanningToHMI* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "minieye.PlanningToHMI";
  }
  protected:
  explicit PlanningToHMI(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_planningtohmi_2eproto);
    return ::descriptor_table_planningtohmi_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kParkIdFieldNumber = 3,
    kTimestampFieldNumber = 1,
    kTickFieldNumber = 2,
    kCurrentStageFieldNumber = 4,
    kRemainDistanceFieldNumber = 5,
    kGearFieldNumber = 6,
    kHasObstacleFieldNumber = 7,
    kIsLeftParkOutFieldNumber = 8,
    kIsRightParkOutFieldNumber = 9,
    kParkDirectionFieldNumber = 10,
    kSlotTypeFieldNumber = 11,
    kApaTypeFieldNumber = 12,
  };
  // .minieye.ParkID park_id = 3;
  bool has_park_id() const;
  private:
  bool _internal_has_park_id() const;
  public:
  void clear_park_id();
  const ::minieye::ParkID& park_id() const;
  ::minieye::ParkID* release_park_id();
  ::minieye::ParkID* mutable_park_id();
  void set_allocated_park_id(::minieye::ParkID* park_id);
  private:
  const ::minieye::ParkID& _internal_park_id() const;
  ::minieye::ParkID* _internal_mutable_park_id();
  public:
  void unsafe_arena_set_allocated_park_id(
      ::minieye::ParkID* park_id);
  ::minieye::ParkID* unsafe_arena_release_park_id();

  // uint64 timestamp = 1;
  void clear_timestamp();
  ::PROTOBUF_NAMESPACE_ID::uint64 timestamp() const;
  void set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_timestamp() const;
  void _internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // uint64 tick = 2;
  void clear_tick();
  ::PROTOBUF_NAMESPACE_ID::uint64 tick() const;
  void set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_tick() const;
  void _internal_set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // float current_stage = 4;
  void clear_current_stage();
  float current_stage() const;
  void set_current_stage(float value);
  private:
  float _internal_current_stage() const;
  void _internal_set_current_stage(float value);
  public:

  // float remain_distance = 5;
  void clear_remain_distance();
  float remain_distance() const;
  void set_remain_distance(float value);
  private:
  float _internal_remain_distance() const;
  void _internal_set_remain_distance(float value);
  public:

  // int32 gear = 6;
  void clear_gear();
  ::PROTOBUF_NAMESPACE_ID::int32 gear() const;
  void set_gear(::PROTOBUF_NAMESPACE_ID::int32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::int32 _internal_gear() const;
  void _internal_set_gear(::PROTOBUF_NAMESPACE_ID::int32 value);
  public:

  // bool has_obstacle = 7;
  void clear_has_obstacle();
  bool has_obstacle() const;
  void set_has_obstacle(bool value);
  private:
  bool _internal_has_obstacle() const;
  void _internal_set_has_obstacle(bool value);
  public:

  // bool is_left_park_out = 8;
  void clear_is_left_park_out();
  bool is_left_park_out() const;
  void set_is_left_park_out(bool value);
  private:
  bool _internal_is_left_park_out() const;
  void _internal_set_is_left_park_out(bool value);
  public:

  // bool is_right_park_out = 9;
  void clear_is_right_park_out();
  bool is_right_park_out() const;
  void set_is_right_park_out(bool value);
  private:
  bool _internal_is_right_park_out() const;
  void _internal_set_is_right_park_out(bool value);
  public:

  // uint32 park_direction = 10;
  void clear_park_direction();
  ::PROTOBUF_NAMESPACE_ID::uint32 park_direction() const;
  void set_park_direction(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_park_direction() const;
  void _internal_set_park_direction(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 slot_type = 11;
  void clear_slot_type();
  ::PROTOBUF_NAMESPACE_ID::uint32 slot_type() const;
  void set_slot_type(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_slot_type() const;
  void _internal_set_slot_type(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 apa_type = 12;
  void clear_apa_type();
  ::PROTOBUF_NAMESPACE_ID::uint32 apa_type() const;
  void set_apa_type(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_apa_type() const;
  void _internal_set_apa_type(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:minieye.PlanningToHMI)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::minieye::ParkID* park_id_;
  ::PROTOBUF_NAMESPACE_ID::uint64 timestamp_;
  ::PROTOBUF_NAMESPACE_ID::uint64 tick_;
  float current_stage_;
  float remain_distance_;
  ::PROTOBUF_NAMESPACE_ID::int32 gear_;
  bool has_obstacle_;
  bool is_left_park_out_;
  bool is_right_park_out_;
  ::PROTOBUF_NAMESPACE_ID::uint32 park_direction_;
  ::PROTOBUF_NAMESPACE_ID::uint32 slot_type_;
  ::PROTOBUF_NAMESPACE_ID::uint32 apa_type_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_planningtohmi_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// -------------------------------------------------------------------

// ParkID

// repeated int32 recommend_id = 1;
inline int ParkID::_internal_recommend_id_size() const {
  return recommend_id_.size();
}
inline int ParkID::recommend_id_size() const {
  return _internal_recommend_id_size();
}
inline void ParkID::clear_recommend_id() {
  recommend_id_.Clear();
}
inline ::PROTOBUF_NAMESPACE_ID::int32 ParkID::_internal_recommend_id(int index) const {
  return recommend_id_.Get(index);
}
inline ::PROTOBUF_NAMESPACE_ID::int32 ParkID::recommend_id(int index) const {
  // @@protoc_insertion_point(field_get:minieye.ParkID.recommend_id)
  return _internal_recommend_id(index);
}
inline void ParkID::set_recommend_id(int index, ::PROTOBUF_NAMESPACE_ID::int32 value) {
  recommend_id_.Set(index, value);
  // @@protoc_insertion_point(field_set:minieye.ParkID.recommend_id)
}
inline void ParkID::_internal_add_recommend_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  recommend_id_.Add(value);
}
inline void ParkID::add_recommend_id(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_add_recommend_id(value);
  // @@protoc_insertion_point(field_add:minieye.ParkID.recommend_id)
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >&
ParkID::_internal_recommend_id() const {
  return recommend_id_;
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >&
ParkID::recommend_id() const {
  // @@protoc_insertion_point(field_list:minieye.ParkID.recommend_id)
  return _internal_recommend_id();
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >*
ParkID::_internal_mutable_recommend_id() {
  return &recommend_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedField< ::PROTOBUF_NAMESPACE_ID::int32 >*
ParkID::mutable_recommend_id() {
  // @@protoc_insertion_point(field_mutable_list:minieye.ParkID.recommend_id)
  return _internal_mutable_recommend_id();
}

// map<int32, int32> id_list = 2;
inline int ParkID::_internal_id_list_size() const {
  return id_list_.size();
}
inline int ParkID::id_list_size() const {
  return _internal_id_list_size();
}
inline void ParkID::clear_id_list() {
  id_list_.Clear();
}
inline const ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >&
ParkID::_internal_id_list() const {
  return id_list_.GetMap();
}
inline const ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >&
ParkID::id_list() const {
  // @@protoc_insertion_point(field_map:minieye.ParkID.id_list)
  return _internal_id_list();
}
inline ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >*
ParkID::_internal_mutable_id_list() {
  return id_list_.MutableMap();
}
inline ::PROTOBUF_NAMESPACE_ID::Map< ::PROTOBUF_NAMESPACE_ID::int32, ::PROTOBUF_NAMESPACE_ID::int32 >*
ParkID::mutable_id_list() {
  // @@protoc_insertion_point(field_mutable_map:minieye.ParkID.id_list)
  return _internal_mutable_id_list();
}

// -------------------------------------------------------------------

// PlanningToHMI

// uint64 timestamp = 1;
inline void PlanningToHMI::clear_timestamp() {
  timestamp_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 PlanningToHMI::_internal_timestamp() const {
  return timestamp_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 PlanningToHMI::timestamp() const {
  // @@protoc_insertion_point(field_get:minieye.PlanningToHMI.timestamp)
  return _internal_timestamp();
}
inline void PlanningToHMI::_internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  timestamp_ = value;
}
inline void PlanningToHMI::set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_timestamp(value);
  // @@protoc_insertion_point(field_set:minieye.PlanningToHMI.timestamp)
}

// uint64 tick = 2;
inline void PlanningToHMI::clear_tick() {
  tick_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 PlanningToHMI::_internal_tick() const {
  return tick_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 PlanningToHMI::tick() const {
  // @@protoc_insertion_point(field_get:minieye.PlanningToHMI.tick)
  return _internal_tick();
}
inline void PlanningToHMI::_internal_set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  tick_ = value;
}
inline void PlanningToHMI::set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_tick(value);
  // @@protoc_insertion_point(field_set:minieye.PlanningToHMI.tick)
}

// .minieye.ParkID park_id = 3;
inline bool PlanningToHMI::_internal_has_park_id() const {
  return this != internal_default_instance() && park_id_ != nullptr;
}
inline bool PlanningToHMI::has_park_id() const {
  return _internal_has_park_id();
}
inline void PlanningToHMI::clear_park_id() {
  if (GetArena() == nullptr && park_id_ != nullptr) {
    delete park_id_;
  }
  park_id_ = nullptr;
}
inline const ::minieye::ParkID& PlanningToHMI::_internal_park_id() const {
  const ::minieye::ParkID* p = park_id_;
  return p != nullptr ? *p : *reinterpret_cast<const ::minieye::ParkID*>(
      &::minieye::_ParkID_default_instance_);
}
inline const ::minieye::ParkID& PlanningToHMI::park_id() const {
  // @@protoc_insertion_point(field_get:minieye.PlanningToHMI.park_id)
  return _internal_park_id();
}
inline void PlanningToHMI::unsafe_arena_set_allocated_park_id(
    ::minieye::ParkID* park_id) {
  if (GetArena() == nullptr) {
    delete reinterpret_cast<::PROTOBUF_NAMESPACE_ID::MessageLite*>(park_id_);
  }
  park_id_ = park_id;
  if (park_id) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_unsafe_arena_set_allocated:minieye.PlanningToHMI.park_id)
}
inline ::minieye::ParkID* PlanningToHMI::release_park_id() {
  auto temp = unsafe_arena_release_park_id();
  if (GetArena() != nullptr) {
    temp = ::PROTOBUF_NAMESPACE_ID::internal::DuplicateIfNonNull(temp);
  }
  return temp;
}
inline ::minieye::ParkID* PlanningToHMI::unsafe_arena_release_park_id() {
  // @@protoc_insertion_point(field_release:minieye.PlanningToHMI.park_id)
  
  ::minieye::ParkID* temp = park_id_;
  park_id_ = nullptr;
  return temp;
}
inline ::minieye::ParkID* PlanningToHMI::_internal_mutable_park_id() {
  
  if (park_id_ == nullptr) {
    auto* p = CreateMaybeMessage<::minieye::ParkID>(GetArena());
    park_id_ = p;
  }
  return park_id_;
}
inline ::minieye::ParkID* PlanningToHMI::mutable_park_id() {
  // @@protoc_insertion_point(field_mutable:minieye.PlanningToHMI.park_id)
  return _internal_mutable_park_id();
}
inline void PlanningToHMI::set_allocated_park_id(::minieye::ParkID* park_id) {
  ::PROTOBUF_NAMESPACE_ID::Arena* message_arena = GetArena();
  if (message_arena == nullptr) {
    delete park_id_;
  }
  if (park_id) {
    ::PROTOBUF_NAMESPACE_ID::Arena* submessage_arena =
      ::PROTOBUF_NAMESPACE_ID::Arena::GetArena(park_id);
    if (message_arena != submessage_arena) {
      park_id = ::PROTOBUF_NAMESPACE_ID::internal::GetOwnedMessage(
          message_arena, park_id, submessage_arena);
    }
    
  } else {
    
  }
  park_id_ = park_id;
  // @@protoc_insertion_point(field_set_allocated:minieye.PlanningToHMI.park_id)
}

// float current_stage = 4;
inline void PlanningToHMI::clear_current_stage() {
  current_stage_ = 0;
}
inline float PlanningToHMI::_internal_current_stage() const {
  return current_stage_;
}
inline float PlanningToHMI::current_stage() const {
  // @@protoc_insertion_point(field_get:minieye.PlanningToHMI.current_stage)
  return _internal_current_stage();
}
inline void PlanningToHMI::_internal_set_current_stage(float value) {
  
  current_stage_ = value;
}
inline void PlanningToHMI::set_current_stage(float value) {
  _internal_set_current_stage(value);
  // @@protoc_insertion_point(field_set:minieye.PlanningToHMI.current_stage)
}

// float remain_distance = 5;
inline void PlanningToHMI::clear_remain_distance() {
  remain_distance_ = 0;
}
inline float PlanningToHMI::_internal_remain_distance() const {
  return remain_distance_;
}
inline float PlanningToHMI::remain_distance() const {
  // @@protoc_insertion_point(field_get:minieye.PlanningToHMI.remain_distance)
  return _internal_remain_distance();
}
inline void PlanningToHMI::_internal_set_remain_distance(float value) {
  
  remain_distance_ = value;
}
inline void PlanningToHMI::set_remain_distance(float value) {
  _internal_set_remain_distance(value);
  // @@protoc_insertion_point(field_set:minieye.PlanningToHMI.remain_distance)
}

// int32 gear = 6;
inline void PlanningToHMI::clear_gear() {
  gear_ = 0;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 PlanningToHMI::_internal_gear() const {
  return gear_;
}
inline ::PROTOBUF_NAMESPACE_ID::int32 PlanningToHMI::gear() const {
  // @@protoc_insertion_point(field_get:minieye.PlanningToHMI.gear)
  return _internal_gear();
}
inline void PlanningToHMI::_internal_set_gear(::PROTOBUF_NAMESPACE_ID::int32 value) {
  
  gear_ = value;
}
inline void PlanningToHMI::set_gear(::PROTOBUF_NAMESPACE_ID::int32 value) {
  _internal_set_gear(value);
  // @@protoc_insertion_point(field_set:minieye.PlanningToHMI.gear)
}

// bool has_obstacle = 7;
inline void PlanningToHMI::clear_has_obstacle() {
  has_obstacle_ = false;
}
inline bool PlanningToHMI::_internal_has_obstacle() const {
  return has_obstacle_;
}
inline bool PlanningToHMI::has_obstacle() const {
  // @@protoc_insertion_point(field_get:minieye.PlanningToHMI.has_obstacle)
  return _internal_has_obstacle();
}
inline void PlanningToHMI::_internal_set_has_obstacle(bool value) {
  
  has_obstacle_ = value;
}
inline void PlanningToHMI::set_has_obstacle(bool value) {
  _internal_set_has_obstacle(value);
  // @@protoc_insertion_point(field_set:minieye.PlanningToHMI.has_obstacle)
}

// bool is_left_park_out = 8;
inline void PlanningToHMI::clear_is_left_park_out() {
  is_left_park_out_ = false;
}
inline bool PlanningToHMI::_internal_is_left_park_out() const {
  return is_left_park_out_;
}
inline bool PlanningToHMI::is_left_park_out() const {
  // @@protoc_insertion_point(field_get:minieye.PlanningToHMI.is_left_park_out)
  return _internal_is_left_park_out();
}
inline void PlanningToHMI::_internal_set_is_left_park_out(bool value) {
  
  is_left_park_out_ = value;
}
inline void PlanningToHMI::set_is_left_park_out(bool value) {
  _internal_set_is_left_park_out(value);
  // @@protoc_insertion_point(field_set:minieye.PlanningToHMI.is_left_park_out)
}

// bool is_right_park_out = 9;
inline void PlanningToHMI::clear_is_right_park_out() {
  is_right_park_out_ = false;
}
inline bool PlanningToHMI::_internal_is_right_park_out() const {
  return is_right_park_out_;
}
inline bool PlanningToHMI::is_right_park_out() const {
  // @@protoc_insertion_point(field_get:minieye.PlanningToHMI.is_right_park_out)
  return _internal_is_right_park_out();
}
inline void PlanningToHMI::_internal_set_is_right_park_out(bool value) {
  
  is_right_park_out_ = value;
}
inline void PlanningToHMI::set_is_right_park_out(bool value) {
  _internal_set_is_right_park_out(value);
  // @@protoc_insertion_point(field_set:minieye.PlanningToHMI.is_right_park_out)
}

// uint32 park_direction = 10;
inline void PlanningToHMI::clear_park_direction() {
  park_direction_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 PlanningToHMI::_internal_park_direction() const {
  return park_direction_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 PlanningToHMI::park_direction() const {
  // @@protoc_insertion_point(field_get:minieye.PlanningToHMI.park_direction)
  return _internal_park_direction();
}
inline void PlanningToHMI::_internal_set_park_direction(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  park_direction_ = value;
}
inline void PlanningToHMI::set_park_direction(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_park_direction(value);
  // @@protoc_insertion_point(field_set:minieye.PlanningToHMI.park_direction)
}

// uint32 slot_type = 11;
inline void PlanningToHMI::clear_slot_type() {
  slot_type_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 PlanningToHMI::_internal_slot_type() const {
  return slot_type_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 PlanningToHMI::slot_type() const {
  // @@protoc_insertion_point(field_get:minieye.PlanningToHMI.slot_type)
  return _internal_slot_type();
}
inline void PlanningToHMI::_internal_set_slot_type(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  slot_type_ = value;
}
inline void PlanningToHMI::set_slot_type(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_slot_type(value);
  // @@protoc_insertion_point(field_set:minieye.PlanningToHMI.slot_type)
}

// uint32 apa_type = 12;
inline void PlanningToHMI::clear_apa_type() {
  apa_type_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 PlanningToHMI::_internal_apa_type() const {
  return apa_type_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 PlanningToHMI::apa_type() const {
  // @@protoc_insertion_point(field_get:minieye.PlanningToHMI.apa_type)
  return _internal_apa_type();
}
inline void PlanningToHMI::_internal_set_apa_type(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  apa_type_ = value;
}
inline void PlanningToHMI::set_apa_type(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_apa_type(value);
  // @@protoc_insertion_point(field_set:minieye.PlanningToHMI.apa_type)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace minieye

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_planningtohmi_2eproto
