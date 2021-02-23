// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: time.proto

#include "time.pb.h"

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
namespace proto {
namespace time {
class UnixTimestampDefaultTypeInternal {
 public:
  ::PROTOBUF_NAMESPACE_ID::internal::ExplicitlyConstructed<UnixTimestamp> _instance;
} _UnixTimestamp_default_instance_;
}  // namespace time
}  // namespace proto
static void InitDefaultsscc_info_UnixTimestamp_time_2eproto() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  {
    void* ptr = &::proto::time::_UnixTimestamp_default_instance_;
    new (ptr) ::proto::time::UnixTimestamp();
    ::PROTOBUF_NAMESPACE_ID::internal::OnShutdownDestroyMessage(ptr);
  }
}

::PROTOBUF_NAMESPACE_ID::internal::SCCInfo<0> scc_info_UnixTimestamp_time_2eproto =
    {{ATOMIC_VAR_INIT(::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase::kUninitialized), 0, 0, InitDefaultsscc_info_UnixTimestamp_time_2eproto}, {}};

static ::PROTOBUF_NAMESPACE_ID::Metadata file_level_metadata_time_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::EnumDescriptor const** file_level_enum_descriptors_time_2eproto = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_time_2eproto = nullptr;

const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_time_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  PROTOBUF_FIELD_OFFSET(::proto::time::UnixTimestamp, _has_bits_),
  PROTOBUF_FIELD_OFFSET(::proto::time::UnixTimestamp, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  PROTOBUF_FIELD_OFFSET(::proto::time::UnixTimestamp, time_),
  0,
};
static const ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 6, sizeof(::proto::time::UnixTimestamp)},
};

static ::PROTOBUF_NAMESPACE_ID::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::PROTOBUF_NAMESPACE_ID::Message*>(&::proto::time::_UnixTimestamp_default_instance_),
};

const char descriptor_table_protodef_time_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\ntime.proto\022\nproto.time\"\035\n\rUnixTimestam"
  "p\022\014\n\004time\030\001 \002(\007B\002H\001"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_time_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_time_2eproto_sccs[1] = {
  &scc_info_UnixTimestamp_time_2eproto.base,
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_time_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_time_2eproto = {
  false, false, descriptor_table_protodef_time_2eproto, "time.proto", 59,
  &descriptor_table_time_2eproto_once, descriptor_table_time_2eproto_sccs, descriptor_table_time_2eproto_deps, 1, 0,
  schemas, file_default_instances, TableStruct_time_2eproto::offsets,
  file_level_metadata_time_2eproto, 1, file_level_enum_descriptors_time_2eproto, file_level_service_descriptors_time_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_time_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_time_2eproto)), true);
namespace proto {
namespace time {

// ===================================================================

class UnixTimestamp::_Internal {
 public:
  using HasBits = decltype(std::declval<UnixTimestamp>()._has_bits_);
  static void set_has_time(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x00000001) ^ 0x00000001) != 0;
  }
};

UnixTimestamp::UnixTimestamp(::PROTOBUF_NAMESPACE_ID::Arena* arena)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena) {
  SharedCtor();
  RegisterArenaDtor(arena);
  // @@protoc_insertion_point(arena_constructor:proto.time.UnixTimestamp)
}
UnixTimestamp::UnixTimestamp(const UnixTimestamp& from)
  : ::PROTOBUF_NAMESPACE_ID::Message(),
      _has_bits_(from._has_bits_) {
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  time_ = from.time_;
  // @@protoc_insertion_point(copy_constructor:proto.time.UnixTimestamp)
}

void UnixTimestamp::SharedCtor() {
  time_ = 0u;
}

UnixTimestamp::~UnixTimestamp() {
  // @@protoc_insertion_point(destructor:proto.time.UnixTimestamp)
  SharedDtor();
  _internal_metadata_.Delete<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

void UnixTimestamp::SharedDtor() {
  GOOGLE_DCHECK(GetArena() == nullptr);
}

void UnixTimestamp::ArenaDtor(void* object) {
  UnixTimestamp* _this = reinterpret_cast< UnixTimestamp* >(object);
  (void)_this;
}
void UnixTimestamp::RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena*) {
}
void UnixTimestamp::SetCachedSize(int size) const {
  _cached_size_.Set(size);
}
const UnixTimestamp& UnixTimestamp::default_instance() {
  ::PROTOBUF_NAMESPACE_ID::internal::InitSCC(&::scc_info_UnixTimestamp_time_2eproto.base);
  return *internal_default_instance();
}


void UnixTimestamp::Clear() {
// @@protoc_insertion_point(message_clear_start:proto.time.UnixTimestamp)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  time_ = 0u;
  _has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* UnixTimestamp::_InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
  while (!ctx->Done(&ptr)) {
    ::PROTOBUF_NAMESPACE_ID::uint32 tag;
    ptr = ::PROTOBUF_NAMESPACE_ID::internal::ReadTag(ptr, &tag);
    CHK_(ptr);
    switch (tag >> 3) {
      // required fixed32 time = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<::PROTOBUF_NAMESPACE_ID::uint8>(tag) == 13)) {
          _Internal::set_has_time(&has_bits);
          time_ = ::PROTOBUF_NAMESPACE_ID::internal::UnalignedLoad<::PROTOBUF_NAMESPACE_ID::uint32>(ptr);
          ptr += sizeof(::PROTOBUF_NAMESPACE_ID::uint32);
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

::PROTOBUF_NAMESPACE_ID::uint8* UnixTimestamp::_InternalSerialize(
    ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:proto.time.UnixTimestamp)
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required fixed32 time = 1;
  if (cached_has_bits & 0x00000001u) {
    target = stream->EnsureSpace(target);
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::WriteFixed32ToArray(1, this->_internal_time(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:proto.time.UnixTimestamp)
  return target;
}

size_t UnixTimestamp::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:proto.time.UnixTimestamp)
  size_t total_size = 0;

  // required fixed32 time = 1;
  if (_internal_has_time()) {
    total_size += 1 + 4;
  }
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    return ::PROTOBUF_NAMESPACE_ID::internal::ComputeUnknownFieldsSize(
        _internal_metadata_, total_size, &_cached_size_);
  }
  int cached_size = ::PROTOBUF_NAMESPACE_ID::internal::ToCachedSize(total_size);
  SetCachedSize(cached_size);
  return total_size;
}

void UnixTimestamp::MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:proto.time.UnixTimestamp)
  GOOGLE_DCHECK_NE(&from, this);
  const UnixTimestamp* source =
      ::PROTOBUF_NAMESPACE_ID::DynamicCastToGenerated<UnixTimestamp>(
          &from);
  if (source == nullptr) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:proto.time.UnixTimestamp)
    ::PROTOBUF_NAMESPACE_ID::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:proto.time.UnixTimestamp)
    MergeFrom(*source);
  }
}

void UnixTimestamp::MergeFrom(const UnixTimestamp& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:proto.time.UnixTimestamp)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_time()) {
    _internal_set_time(from._internal_time());
  }
}

void UnixTimestamp::CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:proto.time.UnixTimestamp)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void UnixTimestamp::CopyFrom(const UnixTimestamp& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:proto.time.UnixTimestamp)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool UnixTimestamp::IsInitialized() const {
  if (_Internal::MissingRequiredFields(_has_bits_)) return false;
  return true;
}

void UnixTimestamp::InternalSwap(UnixTimestamp* other) {
  using std::swap;
  _internal_metadata_.Swap<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(&other->_internal_metadata_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  swap(time_, other->time_);
}

::PROTOBUF_NAMESPACE_ID::Metadata UnixTimestamp::GetMetadata() const {
  return GetMetadataStatic();
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace time
}  // namespace proto
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::proto::time::UnixTimestamp* Arena::CreateMaybeMessage< ::proto::time::UnixTimestamp >(Arena* arena) {
  return Arena::CreateMessageInternal< ::proto::time::UnixTimestamp >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
