// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: soc_to_ihu.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_soc_5fto_5fihu_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_soc_5fto_5fihu_2eproto

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
#define PROTOBUF_INTERNAL_EXPORT_soc_5fto_5fihu_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_soc_5fto_5fihu_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_soc_5fto_5fihu_2eproto;
namespace minieye {
class SocToIHU;
class SocToIHUDefaultTypeInternal;
extern SocToIHUDefaultTypeInternal _SocToIHU_default_instance_;
}  // namespace minieye
PROTOBUF_NAMESPACE_OPEN
template<> ::minieye::SocToIHU* Arena::CreateMaybeMessage<::minieye::SocToIHU>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace minieye {

// ===================================================================

class SocToIHU PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:minieye.SocToIHU) */ {
 public:
  inline SocToIHU() : SocToIHU(nullptr) {};
  virtual ~SocToIHU();

  SocToIHU(const SocToIHU& from);
  SocToIHU(SocToIHU&& from) noexcept
    : SocToIHU() {
    *this = ::std::move(from);
  }

  inline SocToIHU& operator=(const SocToIHU& from) {
    CopyFrom(from);
    return *this;
  }
  inline SocToIHU& operator=(SocToIHU&& from) noexcept {
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
  static const SocToIHU& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const SocToIHU* internal_default_instance() {
    return reinterpret_cast<const SocToIHU*>(
               &_SocToIHU_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(SocToIHU& a, SocToIHU& b) {
    a.Swap(&b);
  }
  inline void Swap(SocToIHU* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(SocToIHU* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline SocToIHU* New() const final {
    return CreateMaybeMessage<SocToIHU>(nullptr);
  }

  SocToIHU* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<SocToIHU>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const SocToIHU& from);
  void MergeFrom(const SocToIHU& from);
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
  void InternalSwap(SocToIHU* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "minieye.SocToIHU";
  }
  protected:
  explicit SocToIHU(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_soc_5fto_5fihu_2eproto);
    return ::descriptor_table_soc_5fto_5fihu_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kFaultStatusCameraFrontFieldNumber = 1,
    kFaultStatusCameraLeftFieldNumber = 2,
    kFaultStatusCameraRearFieldNumber = 3,
    kFaultStatusCameraRightFieldNumber = 4,
    kAVMFuncStsFieldNumber = 5,
    kAVMActDeactDisplayFieldNumber = 6,
    kAVMDisplayModeFieldNumber = 7,
    kAVMCalibrationStateFieldNumber = 8,
    kSetTurnIndicationFeedbackFieldNumber = 9,
    kSetBodyColorFeedbackFieldNumber = 10,
    kSetAutoZoomFeedbackFieldNumber = 11,
    kSetPDCTriggerFeedbackFieldNumber = 12,
    kSetTransparencyFeedbackFieldNumber = 13,
    kSetAutoExitFeedbackFieldNumber = 14,
    kSetLicensePlateFeedbackFieldNumber = 15,
    kSetGuideLineFeedbackFieldNumber = 16,
    kSetFactoryDefaultFeedbackFieldNumber = 17,
    kSetRadarWariningFeedbackFieldNumber = 18,
    kSetSteeringWheelFeedbackFieldNumber = 19,
    kSetLanguageFeedbackFieldNumber = 20,
    kTransparentHoodSwFeedbackFieldNumber = 21,
    kTrailSwFeedbackFieldNumber = 22,
    kTimestampFieldNumber = 23,
    kTickFieldNumber = 24,
  };
  // uint32 FaultStatusCameraFront = 1;
  void clear_faultstatuscamerafront();
  ::PROTOBUF_NAMESPACE_ID::uint32 faultstatuscamerafront() const;
  void set_faultstatuscamerafront(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_faultstatuscamerafront() const;
  void _internal_set_faultstatuscamerafront(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 FaultStatusCameraLeft = 2;
  void clear_faultstatuscameraleft();
  ::PROTOBUF_NAMESPACE_ID::uint32 faultstatuscameraleft() const;
  void set_faultstatuscameraleft(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_faultstatuscameraleft() const;
  void _internal_set_faultstatuscameraleft(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 FaultStatusCameraRear = 3;
  void clear_faultstatuscamerarear();
  ::PROTOBUF_NAMESPACE_ID::uint32 faultstatuscamerarear() const;
  void set_faultstatuscamerarear(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_faultstatuscamerarear() const;
  void _internal_set_faultstatuscamerarear(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 FaultStatusCameraRight = 4;
  void clear_faultstatuscameraright();
  ::PROTOBUF_NAMESPACE_ID::uint32 faultstatuscameraright() const;
  void set_faultstatuscameraright(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_faultstatuscameraright() const;
  void _internal_set_faultstatuscameraright(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 AVMFuncSts = 5;
  void clear_avmfuncsts();
  ::PROTOBUF_NAMESPACE_ID::uint32 avmfuncsts() const;
  void set_avmfuncsts(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_avmfuncsts() const;
  void _internal_set_avmfuncsts(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 AVMActDeactDisplay = 6;
  void clear_avmactdeactdisplay();
  ::PROTOBUF_NAMESPACE_ID::uint32 avmactdeactdisplay() const;
  void set_avmactdeactdisplay(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_avmactdeactdisplay() const;
  void _internal_set_avmactdeactdisplay(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 AVMDisplayMode = 7;
  void clear_avmdisplaymode();
  ::PROTOBUF_NAMESPACE_ID::uint32 avmdisplaymode() const;
  void set_avmdisplaymode(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_avmdisplaymode() const;
  void _internal_set_avmdisplaymode(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 AVMCalibrationState = 8;
  void clear_avmcalibrationstate();
  ::PROTOBUF_NAMESPACE_ID::uint32 avmcalibrationstate() const;
  void set_avmcalibrationstate(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_avmcalibrationstate() const;
  void _internal_set_avmcalibrationstate(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 SetTurnIndicationFeedback = 9;
  void clear_setturnindicationfeedback();
  ::PROTOBUF_NAMESPACE_ID::uint32 setturnindicationfeedback() const;
  void set_setturnindicationfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_setturnindicationfeedback() const;
  void _internal_set_setturnindicationfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 SetBodyColorFeedback = 10;
  void clear_setbodycolorfeedback();
  ::PROTOBUF_NAMESPACE_ID::uint32 setbodycolorfeedback() const;
  void set_setbodycolorfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_setbodycolorfeedback() const;
  void _internal_set_setbodycolorfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 SetAutoZoomFeedback = 11;
  void clear_setautozoomfeedback();
  ::PROTOBUF_NAMESPACE_ID::uint32 setautozoomfeedback() const;
  void set_setautozoomfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_setautozoomfeedback() const;
  void _internal_set_setautozoomfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 SetPDCTriggerFeedback = 12;
  void clear_setpdctriggerfeedback();
  ::PROTOBUF_NAMESPACE_ID::uint32 setpdctriggerfeedback() const;
  void set_setpdctriggerfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_setpdctriggerfeedback() const;
  void _internal_set_setpdctriggerfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 SetTransparencyFeedback = 13;
  void clear_settransparencyfeedback();
  ::PROTOBUF_NAMESPACE_ID::uint32 settransparencyfeedback() const;
  void set_settransparencyfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_settransparencyfeedback() const;
  void _internal_set_settransparencyfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 SetAutoExitFeedback = 14;
  void clear_setautoexitfeedback();
  ::PROTOBUF_NAMESPACE_ID::uint32 setautoexitfeedback() const;
  void set_setautoexitfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_setautoexitfeedback() const;
  void _internal_set_setautoexitfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 SetLicensePlateFeedback = 15;
  void clear_setlicenseplatefeedback();
  ::PROTOBUF_NAMESPACE_ID::uint32 setlicenseplatefeedback() const;
  void set_setlicenseplatefeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_setlicenseplatefeedback() const;
  void _internal_set_setlicenseplatefeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 SetGuideLineFeedback = 16;
  void clear_setguidelinefeedback();
  ::PROTOBUF_NAMESPACE_ID::uint32 setguidelinefeedback() const;
  void set_setguidelinefeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_setguidelinefeedback() const;
  void _internal_set_setguidelinefeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 SetFactoryDefaultFeedback = 17;
  void clear_setfactorydefaultfeedback();
  ::PROTOBUF_NAMESPACE_ID::uint32 setfactorydefaultfeedback() const;
  void set_setfactorydefaultfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_setfactorydefaultfeedback() const;
  void _internal_set_setfactorydefaultfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 SetRadarWariningFeedback = 18;
  void clear_setradarwariningfeedback();
  ::PROTOBUF_NAMESPACE_ID::uint32 setradarwariningfeedback() const;
  void set_setradarwariningfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_setradarwariningfeedback() const;
  void _internal_set_setradarwariningfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 SetSteeringWheelFeedback = 19;
  void clear_setsteeringwheelfeedback();
  ::PROTOBUF_NAMESPACE_ID::uint32 setsteeringwheelfeedback() const;
  void set_setsteeringwheelfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_setsteeringwheelfeedback() const;
  void _internal_set_setsteeringwheelfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 SetLanguageFeedback = 20;
  void clear_setlanguagefeedback();
  ::PROTOBUF_NAMESPACE_ID::uint32 setlanguagefeedback() const;
  void set_setlanguagefeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_setlanguagefeedback() const;
  void _internal_set_setlanguagefeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 TransparentHoodSwFeedback = 21;
  void clear_transparenthoodswfeedback();
  ::PROTOBUF_NAMESPACE_ID::uint32 transparenthoodswfeedback() const;
  void set_transparenthoodswfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_transparenthoodswfeedback() const;
  void _internal_set_transparenthoodswfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint32 TrailSwFeedback = 22;
  void clear_trailswfeedback();
  ::PROTOBUF_NAMESPACE_ID::uint32 trailswfeedback() const;
  void set_trailswfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_trailswfeedback() const;
  void _internal_set_trailswfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // uint64 timestamp = 23;
  void clear_timestamp();
  ::PROTOBUF_NAMESPACE_ID::uint64 timestamp() const;
  void set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_timestamp() const;
  void _internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // uint64 tick = 24;
  void clear_tick();
  ::PROTOBUF_NAMESPACE_ID::uint64 tick() const;
  void set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_tick() const;
  void _internal_set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // @@protoc_insertion_point(class_scope:minieye.SocToIHU)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::uint32 faultstatuscamerafront_;
  ::PROTOBUF_NAMESPACE_ID::uint32 faultstatuscameraleft_;
  ::PROTOBUF_NAMESPACE_ID::uint32 faultstatuscamerarear_;
  ::PROTOBUF_NAMESPACE_ID::uint32 faultstatuscameraright_;
  ::PROTOBUF_NAMESPACE_ID::uint32 avmfuncsts_;
  ::PROTOBUF_NAMESPACE_ID::uint32 avmactdeactdisplay_;
  ::PROTOBUF_NAMESPACE_ID::uint32 avmdisplaymode_;
  ::PROTOBUF_NAMESPACE_ID::uint32 avmcalibrationstate_;
  ::PROTOBUF_NAMESPACE_ID::uint32 setturnindicationfeedback_;
  ::PROTOBUF_NAMESPACE_ID::uint32 setbodycolorfeedback_;
  ::PROTOBUF_NAMESPACE_ID::uint32 setautozoomfeedback_;
  ::PROTOBUF_NAMESPACE_ID::uint32 setpdctriggerfeedback_;
  ::PROTOBUF_NAMESPACE_ID::uint32 settransparencyfeedback_;
  ::PROTOBUF_NAMESPACE_ID::uint32 setautoexitfeedback_;
  ::PROTOBUF_NAMESPACE_ID::uint32 setlicenseplatefeedback_;
  ::PROTOBUF_NAMESPACE_ID::uint32 setguidelinefeedback_;
  ::PROTOBUF_NAMESPACE_ID::uint32 setfactorydefaultfeedback_;
  ::PROTOBUF_NAMESPACE_ID::uint32 setradarwariningfeedback_;
  ::PROTOBUF_NAMESPACE_ID::uint32 setsteeringwheelfeedback_;
  ::PROTOBUF_NAMESPACE_ID::uint32 setlanguagefeedback_;
  ::PROTOBUF_NAMESPACE_ID::uint32 transparenthoodswfeedback_;
  ::PROTOBUF_NAMESPACE_ID::uint32 trailswfeedback_;
  ::PROTOBUF_NAMESPACE_ID::uint64 timestamp_;
  ::PROTOBUF_NAMESPACE_ID::uint64 tick_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_soc_5fto_5fihu_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// SocToIHU

// uint32 FaultStatusCameraFront = 1;
inline void SocToIHU::clear_faultstatuscamerafront() {
  faultstatuscamerafront_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_faultstatuscamerafront() const {
  return faultstatuscamerafront_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::faultstatuscamerafront() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.FaultStatusCameraFront)
  return _internal_faultstatuscamerafront();
}
inline void SocToIHU::_internal_set_faultstatuscamerafront(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  faultstatuscamerafront_ = value;
}
inline void SocToIHU::set_faultstatuscamerafront(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_faultstatuscamerafront(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.FaultStatusCameraFront)
}

// uint32 FaultStatusCameraLeft = 2;
inline void SocToIHU::clear_faultstatuscameraleft() {
  faultstatuscameraleft_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_faultstatuscameraleft() const {
  return faultstatuscameraleft_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::faultstatuscameraleft() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.FaultStatusCameraLeft)
  return _internal_faultstatuscameraleft();
}
inline void SocToIHU::_internal_set_faultstatuscameraleft(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  faultstatuscameraleft_ = value;
}
inline void SocToIHU::set_faultstatuscameraleft(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_faultstatuscameraleft(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.FaultStatusCameraLeft)
}

// uint32 FaultStatusCameraRear = 3;
inline void SocToIHU::clear_faultstatuscamerarear() {
  faultstatuscamerarear_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_faultstatuscamerarear() const {
  return faultstatuscamerarear_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::faultstatuscamerarear() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.FaultStatusCameraRear)
  return _internal_faultstatuscamerarear();
}
inline void SocToIHU::_internal_set_faultstatuscamerarear(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  faultstatuscamerarear_ = value;
}
inline void SocToIHU::set_faultstatuscamerarear(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_faultstatuscamerarear(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.FaultStatusCameraRear)
}

// uint32 FaultStatusCameraRight = 4;
inline void SocToIHU::clear_faultstatuscameraright() {
  faultstatuscameraright_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_faultstatuscameraright() const {
  return faultstatuscameraright_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::faultstatuscameraright() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.FaultStatusCameraRight)
  return _internal_faultstatuscameraright();
}
inline void SocToIHU::_internal_set_faultstatuscameraright(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  faultstatuscameraright_ = value;
}
inline void SocToIHU::set_faultstatuscameraright(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_faultstatuscameraright(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.FaultStatusCameraRight)
}

// uint32 AVMFuncSts = 5;
inline void SocToIHU::clear_avmfuncsts() {
  avmfuncsts_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_avmfuncsts() const {
  return avmfuncsts_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::avmfuncsts() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.AVMFuncSts)
  return _internal_avmfuncsts();
}
inline void SocToIHU::_internal_set_avmfuncsts(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  avmfuncsts_ = value;
}
inline void SocToIHU::set_avmfuncsts(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_avmfuncsts(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.AVMFuncSts)
}

// uint32 AVMActDeactDisplay = 6;
inline void SocToIHU::clear_avmactdeactdisplay() {
  avmactdeactdisplay_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_avmactdeactdisplay() const {
  return avmactdeactdisplay_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::avmactdeactdisplay() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.AVMActDeactDisplay)
  return _internal_avmactdeactdisplay();
}
inline void SocToIHU::_internal_set_avmactdeactdisplay(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  avmactdeactdisplay_ = value;
}
inline void SocToIHU::set_avmactdeactdisplay(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_avmactdeactdisplay(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.AVMActDeactDisplay)
}

// uint32 AVMDisplayMode = 7;
inline void SocToIHU::clear_avmdisplaymode() {
  avmdisplaymode_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_avmdisplaymode() const {
  return avmdisplaymode_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::avmdisplaymode() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.AVMDisplayMode)
  return _internal_avmdisplaymode();
}
inline void SocToIHU::_internal_set_avmdisplaymode(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  avmdisplaymode_ = value;
}
inline void SocToIHU::set_avmdisplaymode(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_avmdisplaymode(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.AVMDisplayMode)
}

// uint32 AVMCalibrationState = 8;
inline void SocToIHU::clear_avmcalibrationstate() {
  avmcalibrationstate_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_avmcalibrationstate() const {
  return avmcalibrationstate_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::avmcalibrationstate() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.AVMCalibrationState)
  return _internal_avmcalibrationstate();
}
inline void SocToIHU::_internal_set_avmcalibrationstate(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  avmcalibrationstate_ = value;
}
inline void SocToIHU::set_avmcalibrationstate(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_avmcalibrationstate(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.AVMCalibrationState)
}

// uint32 SetTurnIndicationFeedback = 9;
inline void SocToIHU::clear_setturnindicationfeedback() {
  setturnindicationfeedback_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_setturnindicationfeedback() const {
  return setturnindicationfeedback_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::setturnindicationfeedback() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.SetTurnIndicationFeedback)
  return _internal_setturnindicationfeedback();
}
inline void SocToIHU::_internal_set_setturnindicationfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  setturnindicationfeedback_ = value;
}
inline void SocToIHU::set_setturnindicationfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_setturnindicationfeedback(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.SetTurnIndicationFeedback)
}

// uint32 SetBodyColorFeedback = 10;
inline void SocToIHU::clear_setbodycolorfeedback() {
  setbodycolorfeedback_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_setbodycolorfeedback() const {
  return setbodycolorfeedback_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::setbodycolorfeedback() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.SetBodyColorFeedback)
  return _internal_setbodycolorfeedback();
}
inline void SocToIHU::_internal_set_setbodycolorfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  setbodycolorfeedback_ = value;
}
inline void SocToIHU::set_setbodycolorfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_setbodycolorfeedback(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.SetBodyColorFeedback)
}

// uint32 SetAutoZoomFeedback = 11;
inline void SocToIHU::clear_setautozoomfeedback() {
  setautozoomfeedback_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_setautozoomfeedback() const {
  return setautozoomfeedback_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::setautozoomfeedback() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.SetAutoZoomFeedback)
  return _internal_setautozoomfeedback();
}
inline void SocToIHU::_internal_set_setautozoomfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  setautozoomfeedback_ = value;
}
inline void SocToIHU::set_setautozoomfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_setautozoomfeedback(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.SetAutoZoomFeedback)
}

// uint32 SetPDCTriggerFeedback = 12;
inline void SocToIHU::clear_setpdctriggerfeedback() {
  setpdctriggerfeedback_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_setpdctriggerfeedback() const {
  return setpdctriggerfeedback_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::setpdctriggerfeedback() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.SetPDCTriggerFeedback)
  return _internal_setpdctriggerfeedback();
}
inline void SocToIHU::_internal_set_setpdctriggerfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  setpdctriggerfeedback_ = value;
}
inline void SocToIHU::set_setpdctriggerfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_setpdctriggerfeedback(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.SetPDCTriggerFeedback)
}

// uint32 SetTransparencyFeedback = 13;
inline void SocToIHU::clear_settransparencyfeedback() {
  settransparencyfeedback_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_settransparencyfeedback() const {
  return settransparencyfeedback_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::settransparencyfeedback() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.SetTransparencyFeedback)
  return _internal_settransparencyfeedback();
}
inline void SocToIHU::_internal_set_settransparencyfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  settransparencyfeedback_ = value;
}
inline void SocToIHU::set_settransparencyfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_settransparencyfeedback(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.SetTransparencyFeedback)
}

// uint32 SetAutoExitFeedback = 14;
inline void SocToIHU::clear_setautoexitfeedback() {
  setautoexitfeedback_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_setautoexitfeedback() const {
  return setautoexitfeedback_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::setautoexitfeedback() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.SetAutoExitFeedback)
  return _internal_setautoexitfeedback();
}
inline void SocToIHU::_internal_set_setautoexitfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  setautoexitfeedback_ = value;
}
inline void SocToIHU::set_setautoexitfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_setautoexitfeedback(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.SetAutoExitFeedback)
}

// uint32 SetLicensePlateFeedback = 15;
inline void SocToIHU::clear_setlicenseplatefeedback() {
  setlicenseplatefeedback_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_setlicenseplatefeedback() const {
  return setlicenseplatefeedback_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::setlicenseplatefeedback() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.SetLicensePlateFeedback)
  return _internal_setlicenseplatefeedback();
}
inline void SocToIHU::_internal_set_setlicenseplatefeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  setlicenseplatefeedback_ = value;
}
inline void SocToIHU::set_setlicenseplatefeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_setlicenseplatefeedback(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.SetLicensePlateFeedback)
}

// uint32 SetGuideLineFeedback = 16;
inline void SocToIHU::clear_setguidelinefeedback() {
  setguidelinefeedback_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_setguidelinefeedback() const {
  return setguidelinefeedback_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::setguidelinefeedback() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.SetGuideLineFeedback)
  return _internal_setguidelinefeedback();
}
inline void SocToIHU::_internal_set_setguidelinefeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  setguidelinefeedback_ = value;
}
inline void SocToIHU::set_setguidelinefeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_setguidelinefeedback(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.SetGuideLineFeedback)
}

// uint32 SetFactoryDefaultFeedback = 17;
inline void SocToIHU::clear_setfactorydefaultfeedback() {
  setfactorydefaultfeedback_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_setfactorydefaultfeedback() const {
  return setfactorydefaultfeedback_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::setfactorydefaultfeedback() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.SetFactoryDefaultFeedback)
  return _internal_setfactorydefaultfeedback();
}
inline void SocToIHU::_internal_set_setfactorydefaultfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  setfactorydefaultfeedback_ = value;
}
inline void SocToIHU::set_setfactorydefaultfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_setfactorydefaultfeedback(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.SetFactoryDefaultFeedback)
}

// uint32 SetRadarWariningFeedback = 18;
inline void SocToIHU::clear_setradarwariningfeedback() {
  setradarwariningfeedback_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_setradarwariningfeedback() const {
  return setradarwariningfeedback_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::setradarwariningfeedback() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.SetRadarWariningFeedback)
  return _internal_setradarwariningfeedback();
}
inline void SocToIHU::_internal_set_setradarwariningfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  setradarwariningfeedback_ = value;
}
inline void SocToIHU::set_setradarwariningfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_setradarwariningfeedback(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.SetRadarWariningFeedback)
}

// uint32 SetSteeringWheelFeedback = 19;
inline void SocToIHU::clear_setsteeringwheelfeedback() {
  setsteeringwheelfeedback_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_setsteeringwheelfeedback() const {
  return setsteeringwheelfeedback_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::setsteeringwheelfeedback() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.SetSteeringWheelFeedback)
  return _internal_setsteeringwheelfeedback();
}
inline void SocToIHU::_internal_set_setsteeringwheelfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  setsteeringwheelfeedback_ = value;
}
inline void SocToIHU::set_setsteeringwheelfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_setsteeringwheelfeedback(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.SetSteeringWheelFeedback)
}

// uint32 SetLanguageFeedback = 20;
inline void SocToIHU::clear_setlanguagefeedback() {
  setlanguagefeedback_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_setlanguagefeedback() const {
  return setlanguagefeedback_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::setlanguagefeedback() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.SetLanguageFeedback)
  return _internal_setlanguagefeedback();
}
inline void SocToIHU::_internal_set_setlanguagefeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  setlanguagefeedback_ = value;
}
inline void SocToIHU::set_setlanguagefeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_setlanguagefeedback(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.SetLanguageFeedback)
}

// uint32 TransparentHoodSwFeedback = 21;
inline void SocToIHU::clear_transparenthoodswfeedback() {
  transparenthoodswfeedback_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_transparenthoodswfeedback() const {
  return transparenthoodswfeedback_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::transparenthoodswfeedback() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.TransparentHoodSwFeedback)
  return _internal_transparenthoodswfeedback();
}
inline void SocToIHU::_internal_set_transparenthoodswfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  transparenthoodswfeedback_ = value;
}
inline void SocToIHU::set_transparenthoodswfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_transparenthoodswfeedback(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.TransparentHoodSwFeedback)
}

// uint32 TrailSwFeedback = 22;
inline void SocToIHU::clear_trailswfeedback() {
  trailswfeedback_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::_internal_trailswfeedback() const {
  return trailswfeedback_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 SocToIHU::trailswfeedback() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.TrailSwFeedback)
  return _internal_trailswfeedback();
}
inline void SocToIHU::_internal_set_trailswfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  trailswfeedback_ = value;
}
inline void SocToIHU::set_trailswfeedback(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_trailswfeedback(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.TrailSwFeedback)
}

// uint64 timestamp = 23;
inline void SocToIHU::clear_timestamp() {
  timestamp_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SocToIHU::_internal_timestamp() const {
  return timestamp_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SocToIHU::timestamp() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.timestamp)
  return _internal_timestamp();
}
inline void SocToIHU::_internal_set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  timestamp_ = value;
}
inline void SocToIHU::set_timestamp(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_timestamp(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.timestamp)
}

// uint64 tick = 24;
inline void SocToIHU::clear_tick() {
  tick_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SocToIHU::_internal_tick() const {
  return tick_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 SocToIHU::tick() const {
  // @@protoc_insertion_point(field_get:minieye.SocToIHU.tick)
  return _internal_tick();
}
inline void SocToIHU::_internal_set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  tick_ = value;
}
inline void SocToIHU::set_tick(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_tick(value);
  // @@protoc_insertion_point(field_set:minieye.SocToIHU.tick)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace minieye

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_soc_5fto_5fihu_2eproto