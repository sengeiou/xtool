// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: devinfo.proto

#include "devinfo.pb.h"

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
namespace info {
class DeviceDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<Device> _instance;
} _Device_default_instance_;
class BatteryDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<Battery> _instance;
} _Battery_default_instance_;
}  // namespace info
static void InitDefaultsscc_info_Battery_devinfo_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::info::_Battery_default_instance_;
    new (ptr) ::info::Battery();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_Battery_devinfo_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_Battery_devinfo_2eproto}, {}};

static void InitDefaultsscc_info_Device_devinfo_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::info::_Device_default_instance_;
    new (ptr) ::info::Device();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_Device_devinfo_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_Device_devinfo_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_devinfo_2eproto[2];
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_devinfo_2eproto[3];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_devinfo_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_devinfo_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::info::Device, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::info::Device, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::info::Device, type_),
  PROTOBUF_FIELD_OFFSET(::info::Device, model_),
  PROTOBUF_FIELD_OFFSET(::info::Device, firmware_ver_),
  PROTOBUF_FIELD_OFFSET(::info::Device, hardware_ver_),
  PROTOBUF_FIELD_OFFSET(::info::Device, sn_),
  PROTOBUF_FIELD_OFFSET(::info::Device, mac_),
  2,
  3,
  4,
  5,
  0,
  1,
  PROTOBUF_FIELD_OFFSET(::info::Battery, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::info::Battery, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::info::Battery, level_),
  PROTOBUF_FIELD_OFFSET(::info::Battery, status_),
  0,
  1,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 11, sizeof(::info::Device)},
  { 17, 24, sizeof(::info::Battery)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::info::_Device_default_instance_),
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::info::_Battery_default_instance_),
};

const char descriptor_table_protodef_devinfo_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\rdevinfo.proto\022\004info\"\211\002\n\006Device\022(\n\004type"
  "\030\001 \001(\0162\021.info.Device.Type:\007INVALID\022-\n\005mo"
  "del\030\002 \001(\0162\022.info.Device.Model:\nDM_INVALI"
  "D\022\024\n\014firmware_ver\030\003 \001(\r\022\024\n\014hardware_ver\030"
  "\004 \001(\r\022\n\n\002sn\030\007 \001(\014\022\013\n\003mac\030\010 \001(\014\"7\n\004Type\022\013"
  "\n\007INVALID\020\000\022\010\n\004BAND\020\001\022\t\n\005WATCH\020\002\022\r\n\tEARP"
  "HONES\020\003\"(\n\005Model\022\016\n\nDM_INVALID\020\000\022\017\n\013DM_A"
  "POLLO3P\020\001\"`\n\007Battery\022\r\n\005level\030\001 \002(\r\022\016\n\006s"
  "tatus\030\002 \001(\r\"6\n\006Status\022\010\n\004IDLE\020\000\022\014\n\010CHARG"
  "ING\020\001\022\024\n\020CHARGE_COMPLETED\020\002B\002H\001"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_devinfo_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_devinfo_2eproto_sccs[2] = {
  &scc_info_Battery_devinfo_2eproto.base,
  &scc_info_Device_devinfo_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_devinfo_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_devinfo_2eproto = {
  false, false, descriptor_table_protodef_devinfo_2eproto, "devinfo.proto", 391,
  &descriptor_table_devinfo_2eproto_once, descriptor_table_devinfo_2eproto_sccs, descriptor_table_devinfo_2eproto_deps, 2, 0,
  schemas, file_default_instances, TableStruct_devinfo_2eproto::offsets,
  file_level_metadata_devinfo_2eproto, 2, file_level_enum_descriptors_devinfo_2eproto, file_level_service_descriptors_devinfo_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_devinfo_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_devinfo_2eproto)), true);
namespace info {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Device_Type_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_devinfo_2eproto);
  return file_level_enum_descriptors_devinfo_2eproto[0];
}
bool Device_Type_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr Device_Type Device::INVALID;
constexpr Device_Type Device::BAND;
constexpr Device_Type Device::WATCH;
constexpr Device_Type Device::EARPHONES;
constexpr Device_Type Device::Type_MIN;
constexpr Device_Type Device::Type_MAX;
constexpr int Device::Type_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Device_Model_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_devinfo_2eproto);
  return file_level_enum_descriptors_devinfo_2eproto[1];
}
bool Device_Model_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr Device_Model Device::DM_INVALID;
constexpr Device_Model Device::DM_APOLLO3P;
constexpr Device_Model Device::Model_MIN;
constexpr Device_Model Device::Model_MAX;
constexpr int Device::Model_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Battery_Status_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_devinfo_2eproto);
  return file_level_enum_descriptors_devinfo_2eproto[2];
}
bool Battery_Status_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

#if (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)
constexpr Battery_Status Battery::IDLE;
constexpr Battery_Status Battery::CHARGING;
constexpr Battery_Status Battery::CHARGE_COMPLETED;
constexpr Battery_Status Battery::Status_MIN;
constexpr Battery_Status Battery::Status_MAX;
constexpr int Battery::Status_ARRAYSIZE;
#endif  // (__cplusplus < 201703) && (!defined(_MSC_VER) || _MSC_VER >= 1900)

// ===================================================================

class Device::_Internal {
 public:
  using HasBits = decltype(std::declval<Device>()._has_bits_);
  static void set_has_type(HasBits* has_bits) {
    (*has_bits)[0] |= 4u;
  }
  static void set_has_model(HasBits* has_bits) {
    (*has_bits)[0] |= 8u;
  }
  static void set_has_firmware_ver(HasBits* has_bits) {
    (*has_bits)[0] |= 16u;
  }
  static void set_has_hardware_ver(HasBits* has_bits) {
    (*has_bits)[0] |= 32u;
  }
  static void set_has_sn(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_mac(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
};

Device::Device(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:info.Device)
}
Device::Device(const Device& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  sn_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_sn()) {
    sn_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_sn(), 
      GetArena());
  }
  mac_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  if (from._internal_has_mac()) {
    mac_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, from._internal_mac(), 
      GetArena());
  }
  ::memcpy(&type_, &from.type_,
    static_cast<size_t>(reinterpret_cast<char*>(&hardware_ver_) -
    reinterpret_cast<char*>(&type_)) + sizeof(hardware_ver_));
  // @@protoc_insertion_point(copy_constructor:info.Device)
}

void Device::SharedCtor() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&scc_info_Device_devinfo_2eproto.base);
  sn_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  mac_.UnsafeSetDefault(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  ::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
      reinterpret_cast<char*>(&type_) - reinterpret_cast<char*>(this)),
      0, static_cast<size_t>(reinterpret_cast<char*>(&hardware_ver_) -
      reinterpret_cast<char*>(&type_)) + sizeof(hardware_ver_));
}

Device::~Device() {
  // @@protoc_insertion_point(destructor:info.Device)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void Device::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
  sn_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
  mac_.DestroyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}

void Device::ArenaDtor(void* object) {
  Device* _this = reinterpret_cast< Device* >(object);
  (void)_this;
}
void Device::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Device::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Device& Device::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_Device_devinfo_2eproto.base);
  return *internal_default_instance();
}


void Device::Clear() {
// @@protoc_insertion_point(message_clear_start:info.Device)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      sn_.ClearNonDefaultToEmpty();
    }
    if (cached_has_bits & 0x00000002u) {
      mac_.ClearNonDefaultToEmpty();
    }
  }
  if (cached_has_bits & 0x0000003cu) {
    ::memset(&type_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&hardware_ver_) -
        reinterpret_cast<char*>(&type_)) + sizeof(hardware_ver_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Device::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // optional .info.Device.Type type = 1 [default = INVALID];
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          if (PROTOBUF_PREDICT_TRUE(::info::Device_Type_IsValid(val))) {
            _internal_set_type(static_cast<::info::Device_Type>(val));
          } else {
            ::PROTOBUF_NAMESPACE_ID::internal::WriteVarint(1, val, mutable_unknown_fields());
          }
        } else goto handle_unusual;
        continue;
      // optional .info.Device.Model model = 2 [default = DM_INVALID];
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          ::PROTOBUF_NAMESPACE_ID::uint64 val = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint64(&ptr);
          CHK_(ptr);
          if (PROTOBUF_PREDICT_TRUE(::info::Device_Model_IsValid(val))) {
            _internal_set_model(static_cast<::info::Device_Model>(val));
          } else {
            ::PROTOBUF_NAMESPACE_ID::internal::WriteVarint(2, val, mutable_unknown_fields());
          }
        } else goto handle_unusual;
        continue;
      // optional uint32 firmware_ver = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 24)) {
          _Internal::set_has_firmware_ver(&has_bits);
          firmware_ver_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // optional uint32 hardware_ver = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 32)) {
          _Internal::set_has_hardware_ver(&has_bits);
          hardware_ver_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // optional bytes sn = 7;
      case 7:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 58)) {
          auto str = _internal_mutable_sn();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // optional bytes mac = 8;
      case 8:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 66)) {
          auto str = _internal_mutable_mac();
          ptr = ::PROTOBUF_NAMESPACE_ID::internal::InlineGreedyStringParser(str, ptr, ctx);
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
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Device::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:info.Device)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // optional .info.Device.Type type = 1 [default = INVALID];
  if (cached_has_bits & 0x00000004u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      1, this->_internal_type(), target);
  }

  // optional .info.Device.Model model = 2 [default = DM_INVALID];
  if (cached_has_bits & 0x00000008u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteEnumToArray(
      2, this->_internal_model(), target);
  }

  // optional uint32 firmware_ver = 3;
  if (cached_has_bits & 0x00000010u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(3, this->_internal_firmware_ver(), target);
  }

  // optional uint32 hardware_ver = 4;
  if (cached_has_bits & 0x00000020u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(4, this->_internal_hardware_ver(), target);
  }

  // optional bytes sn = 7;
  if (cached_has_bits & 0x00000001u) {
    target = stream->WriteBytesMaybeAliased(
        7, this->_internal_sn(), target);
  }

  // optional bytes mac = 8;
  if (cached_has_bits & 0x00000002u) {
    target = stream->WriteBytesMaybeAliased(
        8, this->_internal_mac(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:info.Device)
  return target;
}

size_t Device::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:info.Device)
  size_t total_size = 0;

  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x0000003fu) {
    // optional bytes sn = 7;
    if (cached_has_bits & 0x00000001u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
          this->_internal_sn());
    }

    // optional bytes mac = 8;
    if (cached_has_bits & 0x00000002u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::BytesSize(
          this->_internal_mac());
    }

    // optional .info.Device.Type type = 1 [default = INVALID];
    if (cached_has_bits & 0x00000004u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_type());
    }

    // optional .info.Device.Model model = 2 [default = DM_INVALID];
    if (cached_has_bits & 0x00000008u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::EnumSize(this->_internal_model());
    }

    // optional uint32 firmware_ver = 3;
    if (cached_has_bits & 0x00000010u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
          this->_internal_firmware_ver());
    }

    // optional uint32 hardware_ver = 4;
    if (cached_has_bits & 0x00000020u) {
      total_size += 1 +
        ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
          this->_internal_hardware_ver());
    }

  }
  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Device::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:info.Device)
  GOOGLE_DCHECK_NE(&from, this);
  const Device* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Device>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:info.Device)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:info.Device)
    MergeFrom(*source);
  }
}

void Device::MergeFrom(const Device& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:info.Device)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x0000003fu) {
    if (cached_has_bits & 0x00000001u) {
      _internal_set_sn(from._internal_sn());
    }
    if (cached_has_bits & 0x00000002u) {
      _internal_set_mac(from._internal_mac());
    }
    if (cached_has_bits & 0x00000004u) {
      type_ = from.type_;
    }
    if (cached_has_bits & 0x00000008u) {
      model_ = from.model_;
    }
    if (cached_has_bits & 0x00000010u) {
      firmware_ver_ = from.firmware_ver_;
    }
    if (cached_has_bits & 0x00000020u) {
      hardware_ver_ = from.hardware_ver_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void Device::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:info.Device)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Device::CopyFrom(const Device& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:info.Device)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Device::IsInitialized() const {
  return true;
}

void Device::InternalSwap(Device* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  sn_.Swap(&other->sn_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  mac_.Swap(&other->mac_, &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Device, hardware_ver_)
      + sizeof(Device::hardware_ver_)
      - PROTOBUF_FIELD_OFFSET(Device, type_)>(
          reinterpret_cast<char*>(&type_),
          reinterpret_cast<char*>(&other->type_));
}

::PROTOBUF_NAMESPACE_ID::Metadata Device::GetMetadata() const {
  return GetMetadataStatic();
}


// ===================================================================

class Battery::_Internal {
 public:
  using HasBits = decltype(std::declval<Battery>()._has_bits_);
  static void set_has_level(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static void set_has_status(HasBits* has_bits) {
    (*has_bits)[0] |= 2u;
  }
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x00000001) ^ 0x00000001) != 0;
  }
};

Battery::Battery(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:info.Battery)
}
Battery::Battery(const Battery& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::memcpy(&level_, &from.level_,
    static_cast<size_t>(reinterpret_cast<char*>(&status_) -
    reinterpret_cast<char*>(&level_)) + sizeof(status_));
  // @@protoc_insertion_point(copy_constructor:info.Battery)
}

void Battery::SharedCtor() {
  ::memset(reinterpret_cast<char*>(this) + static_cast<size_t>(
      reinterpret_cast<char*>(&level_) - reinterpret_cast<char*>(this)),
      0, static_cast<size_t>(reinterpret_cast<char*>(&status_) -
      reinterpret_cast<char*>(&level_)) + sizeof(status_));
}

Battery::~Battery() {
  // @@protoc_insertion_point(destructor:info.Battery)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void Battery::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void Battery::ArenaDtor(void* object) {
  Battery* _this = reinterpret_cast< Battery* >(object);
  (void)_this;
}
void Battery::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void Battery::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const Battery& Battery::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_Battery_devinfo_2eproto.base);
  return *internal_default_instance();
}


void Battery::Clear() {
// @@protoc_insertion_point(message_clear_start:info.Battery)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    ::memset(&level_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&status_) -
        reinterpret_cast<char*>(&level_)) + sizeof(status_));
  }
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* Battery::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // required uint32 level = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 8)) {
          _Internal::set_has_level(&has_bits);
          level_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else goto handle_unusual;
        continue;
      // optional uint32 status = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 16)) {
          _Internal::set_has_status(&has_bits);
          status_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
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
  _has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto success;
#undef CHK_
}

::PROTOBUF_NAMESPACE_ID::uint8* Battery::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:info.Battery)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required uint32 level = 1;
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(1, this->_internal_level(), target);
  }

  // optional uint32 status = 2;
  if (cached_has_bits & 0x00000002u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteUInt32ToArray(2, this->_internal_status(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:info.Battery)
  return target;
}

size_t Battery::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:info.Battery)
  size_t total_size = 0;

  // required uint32 level = 1;
  if (_internal_has_level()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_level());
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // optional uint32 status = 2;
  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 0x00000002u) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::UInt32Size(
        this->_internal_status());
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void Battery::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:info.Battery)
  GOOGLE_DCHECK_NE(&from, this);
  const Battery* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<Battery>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:info.Battery)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:info.Battery)
    MergeFrom(*source);
  }
}

void Battery::MergeFrom(const Battery& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:info.Battery)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 0x00000003u) {
    if (cached_has_bits & 0x00000001u) {
      level_ = from.level_;
    }
    if (cached_has_bits & 0x00000002u) {
      status_ = from.status_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
}

void Battery::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:info.Battery)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Battery::CopyFrom(const Battery& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:info.Battery)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Battery::IsInitialized() const {
  if (_Internal::MissingRequiredFields(_has_bits_)) return false;
  return true;
}

void Battery::InternalSwap(Battery* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Battery, status_)
      + sizeof(Battery::status_)
      - PROTOBUF_FIELD_OFFSET(Battery, level_)>(
          reinterpret_cast<char*>(&level_),
          reinterpret_cast<char*>(&other->level_));
}

::PROTOBUF_NAMESPACE_ID::Metadata Battery::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace info
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::info::Device* Arena::CreateMaybeMessage< ::info::Device >(Arena* arena) {
  return Arena::CreateMessageInternal< ::info::Device >(arena);
}
template<> PROTOBUF_NOINLINE ::info::Battery* Arena::CreateMaybeMessage< ::info::Battery >(Arena* arena) {
  return Arena::CreateMessageInternal< ::info::Battery >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
