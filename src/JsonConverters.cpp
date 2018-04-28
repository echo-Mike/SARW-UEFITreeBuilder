/// STD
#include <cstring>
#include <cstdio>
#include <string>
#include <codecvt>
/// PROJECT
#include "JsonConverters.hpp"
#include "PiSectionUtils.hpp"
#include "PiFileUtils.hpp"
#include "PiVolumeUtils.hpp"

namespace Helper
{

	static const char* getObjectName(const Project::PiObject::Object& obj)
	{
		using namespace Project::PiObject;
		switch (obj.state & InconsistencyState::TypeBits)
		{
			case InconsistencyState::FreeSpaceFlag : return "Free space";
			case InconsistencyState::DataFlag      : return "Data";
			case InconsistencyState::SectionFlag   : return "Section";
			case InconsistencyState::FileFlag      : return "File";
			case InconsistencyState::VolumeFlag    : return "Volume";
			default	                               : return "Unknown";
		}
	}

	static void parseDepexSection(nlohmann::json& j, const Project::Pi::Section::Header& hdr, Project::PiObject::Helper::SectionHeader::SectionType secType)
	{
		// EFI_DEP_AFTER
	}
}

void to_json(nlohmann::json& j, const EFI_GUID& guid)
{
	char guidstr[64];
	std::uint32_t first;
	std::uint16_t middle1, middle2, middle3;
	std::uint64_t last = 0;
	auto ptr = reinterpret_cast<Project::Types::const_byte_ptr_t>(&guid);
	proj_get_uint32_t(ptr, &first);
	proj_get_uint16_t(ptr + 4, &middle1);
	proj_get_uint16_t(ptr + 6, &middle2);
	if (proj_is_big_endian()) {
		proj_get_uint16_t(ptr + 8, &middle3);
		proj_get_uint_n(ptr + 10, &last, 6);
	} else {
		proj_get_rev_uint16_t(ptr + 8, &middle3);
		proj_get_rev_uint_n(ptr + 10, &last, 6);
	}
	std::memset(guidstr, 0, sizeof(guidstr));
	std::snprintf(guidstr, sizeof(guidstr), "%.8X-%.4hX-%.4hX-%.4hX-%.12llX", first, middle1, middle2, middle3, last);
	j = guidstr;
}

void Project::Guid::to_json(nlohmann::json& j, const Project::Guid::GuidWithName& guid)
{
	j["value"] = guid.value;
	j["name"] = guid.name;
}

void Project::to_json(nlohmann::json& j, const Project::Pi::Volume::Header& hdr)
{
	char strBuffer[256];
	auto ptr = reinterpret_cast<Types::const_byte_ptr_t>(hdr->ZeroVector);
	j["Structure type"] = "Firmware Volume";
	// Structure fields
	// ZeroVector
	std::memset(strBuffer, 0, sizeof(strBuffer));
	std::snprintf(strBuffer, sizeof(strBuffer),
		"%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX",
		ptr[ 0], ptr[ 1], ptr[ 2], ptr[ 3],
		ptr[ 4], ptr[ 5], ptr[ 6], ptr[ 7],
		ptr[ 8], ptr[ 9], ptr[10], ptr[11],
		ptr[12], ptr[13], ptr[14], ptr[15]
	);
	j["fields"]["ZeroVector"] = strBuffer;
	// FileSystemGuid
	auto& guid_ = Guid::NamedGuids::findNamedGuid(hdr->FileSystemGuid);
	if (&guid_ == Guid::NamedGuids::end())
		j["fields"]["FileSystemGuid"]["value"] = hdr->FileSystemGuid;
	else 
		j["fields"]["FileSystemGuid"] = guid_;
	// FvLength
	std::snprintf(strBuffer, sizeof(strBuffer), "%lld (%#llX)", hdr->FvLength, hdr->FvLength);
	j["fields"]["FvLength"]["string"] = strBuffer;
	j["fields"]["FvLength"]["value"] = hdr->FvLength;
	// Signature
	strBuffer[0] = (hdr->Signature & 0xFF000000) >> 24;
	strBuffer[1] = (hdr->Signature & 0x00FF0000) >> 16;
	strBuffer[2] = (hdr->Signature & 0x0000FF00) >>  8;
	strBuffer[3] = (hdr->Signature & 0x000000FF);
	strBuffer[4] = '\0';
	j["fields"]["Signature"] = strBuffer;
	// Attributes
	j["fields"]["Attributes"]["string"] = Pi::Volume::Utils::getAtributeString(hdr);
	j["fields"]["Attributes"]["alternative"] = Pi::Volume::Utils::getAtributeString(hdr, true);
	j["fields"]["Attributes"]["value"] = hdr->Attributes;
	// HeaderLength
	std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#hX)", hdr->HeaderLength, hdr->HeaderLength);
	j["fields"]["HeaderLength"]["string"] = strBuffer;
	j["fields"]["HeaderLength"]["value"] = hdr->HeaderLength;
	// Checksum
	std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#hX)", hdr->Checksum, hdr->Checksum);
	j["fields"]["Checksum"]["string"] = strBuffer;
	j["fields"]["Checksum"]["value"] = hdr->Checksum;
	// ExtHeaderOffset
	std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#hX)", hdr->ExtHeaderOffset, hdr->ExtHeaderOffset);
	j["fields"]["ExtHeaderOffset"]["string"] = strBuffer;
	j["fields"]["ExtHeaderOffset"]["value"] = hdr->ExtHeaderOffset;
	// Reserved
	j["fields"]["Reserved"] = hdr->Reserved;
	// Revision
	j["fields"]["Revision"] = hdr->Revision;
}

void Project::to_json(nlohmann::json& j, const Project::Pi::File::Header& hdr)
{
	char strBuffer[64];
	j["Structure type"] = "Firmware File";
	j["isExtended"] = static_cast<bool>(hdr->Attributes & FFS_ATTRIB_LARGE_FILE);
	// Structure fields
	// Name
	auto& guid_ = Guid::NamedGuids::findNamedGuid(hdr->Name);
	if (&guid_ == Guid::NamedGuids::end())
		j["fields"]["Name"]["value"] = hdr->Name;
	else
		j["fields"]["Name"] = guid_;
	// IntegrityCheck
	std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#hX)", hdr->IntegrityCheck.Checksum16, hdr->IntegrityCheck.Checksum16);
	j["fields"]["IntegrityCheck"]["Checksum16"]["string"] = strBuffer;
	j["fields"]["IntegrityCheck"]["Checksum16"]["value"] = hdr->IntegrityCheck.Checksum16;
	std::snprintf(strBuffer, sizeof(strBuffer), "%hhd (%#hhX)", hdr->IntegrityCheck.Checksum.Header, hdr->IntegrityCheck.Checksum.Header);
	j["fields"]["IntegrityCheck"]["Header"]["string"] = strBuffer;
	j["fields"]["IntegrityCheck"]["Header"]["value"] = hdr->IntegrityCheck.Checksum.Header;
	std::snprintf(strBuffer, sizeof(strBuffer), "%hhd (%#hhX)", hdr->IntegrityCheck.Checksum.File, hdr->IntegrityCheck.Checksum.File);
	j["fields"]["IntegrityCheck"]["File"]["string"] = strBuffer;
	j["fields"]["IntegrityCheck"]["File"]["value"] = hdr->IntegrityCheck.Checksum.File;
	// Type
	j["fields"]["Type"]["string"] = Pi::File::Utils::fileTypeToCStr(hdr->Type);
	j["fields"]["Type"]["value"] = hdr->Type;
	// Attributes
	j["fields"]["Attributes"]["string"] = Pi::File::Utils::getAtributeString(hdr);
	j["fields"]["Attributes"]["alternative"] = Pi::File::Utils::getAtributeString(hdr, true);
	j["fields"]["Attributes"]["value"] = hdr->Attributes;
	// Size
	auto simple_size = Pi::File::Utils::getSize(hdr);
	std::snprintf(strBuffer, sizeof(strBuffer), "%lld (%#llX)", simple_size, simple_size);
	j["fields"]["Size"]["string"] = strBuffer;
	j["fields"]["Size"]["value"] = simple_size;
	// State
	j["fields"]["State"]["string"] = Pi::File::Utils::getStateString(hdr);
	j["fields"]["State"]["alternative"] = Pi::File::Utils::getStateString(hdr, true);
	j["fields"]["State"]["value"] = hdr->State;
	// ExtendedSize
	if (hdr->Attributes & FFS_ATTRIB_LARGE_FILE)
	{
		auto extended_size = Pi::File::Utils::getSize2(hdr);
		std::snprintf(strBuffer, sizeof(strBuffer), "%lld (%#llX)", extended_size, extended_size);
		j["fields"]["ExtendedSize"]["string"] = strBuffer;
		j["fields"]["ExtendedSize"]["value"] = extended_size;
	}
}

void Project::to_json(nlohmann::json& j, const Project::Pi::Section::Header& hdr)
{
	char strBuffer[64];
	j["Structure type"] = "Firmware Section";
	j["isExtended"] = Pi::Section::Utils::isExtendedSection(hdr);
	// Structure fields
	// Size
	auto simple_size = Pi::Section::Utils::getSize(hdr);
	std::snprintf(strBuffer, sizeof(strBuffer), "%lld (%#llX)", simple_size, simple_size);
	j["fields"]["Size"]["string"] = strBuffer;
	j["fields"]["Size"]["value"] = simple_size;
	// Type
	j["fields"]["Type"]["string"] = Pi::Section::Utils::sectionTypeToCStr(hdr->Type);
	j["fields"]["Type"]["value"] = hdr->Type;
	// ExtendedSize
	if (Pi::Section::Utils::isExtendedSection(hdr))
	{
		auto extended_size = Pi::Section::Utils::getSize2(hdr);
		std::snprintf(strBuffer, sizeof(strBuffer), "%lld (%#llX)", extended_size, extended_size);
		j["fields"]["ExtendedSize"]["string"] = strBuffer;
		j["fields"]["ExtendedSize"]["value"] = extended_size;
	}
}

void Project::PiObject::Helper::to_json(nlohmann::json& j, const SectionHeader& obj)
{
	char strBuffer[64];
	j = obj.header;
	switch (obj.sectionType)
	{
		case SectionHeader::Compression:
		{
			UINT32 section_uncompressedLength = 0;
			UINT8 section_compressionType = 0;

			if (Pi::Section::Utils::isExtendedSection(obj.header)) {
				Pi::Section::Extended::Compression sv(obj.header.begin);
				section_uncompressedLength = sv->UncompressedLength;
				section_compressionType = sv->CompressionType;
			}
			else
			{
				Pi::Section::Compression sv(obj.header.begin);
				section_uncompressedLength = sv->UncompressedLength;
				section_compressionType = sv->CompressionType;
			}
			std::snprintf(strBuffer, sizeof(strBuffer), "%d (%#X)", section_uncompressedLength, section_uncompressedLength);
			j["fields"]["UncompressedLength"]["string"] = strBuffer;
			j["fields"]["UncompressedLength"]["value"] = section_uncompressedLength;
			std::snprintf(strBuffer, sizeof(strBuffer), "%hhd (%#hhX)", section_compressionType, section_compressionType);
			switch (section_compressionType)
			{
				case EFI_NOT_COMPRESSED       : j["fields"]["CompressionType"]["string"] = "EFI_NOT_COMPRESSED"; break;
				case EFI_STANDARD_COMPRESSION : j["fields"]["CompressionType"]["string"] = "EFI_STANDARD_COMPRESSION"; break;
				default                       : j["fields"]["CompressionType"]["string"] = strBuffer; break;
			}
			j["fields"]["CompressionType"]["value"] = section_compressionType;
		} break;

		case SectionHeader::SmmDepex:
		case SectionHeader::PeiDepex:
		case SectionHeader::DxeDepex:
		{
			::Helper::parseDepexSection(j, obj.header, obj.sectionType);
		} break;

		case SectionHeader::FreeformSubtypeGuid:
		{
			EFI_GUID section_guid;
			if (Pi::Section::Utils::isExtendedSection(obj.header)) {
				section_guid = reinterpret_cast<Pi::Section::Extended::FreeformSubtypeGuid::const_pointer_t>(obj.header.begin)->SubTypeGuid;
			} else {
				section_guid = reinterpret_cast<Pi::Section::FreeformSubtypeGuid::const_pointer_t>(obj.header.begin)->SubTypeGuid;
			}
			auto& guid_ = Guid::NamedGuids::findNamedGuid(section_guid);
			if (&guid_ == Guid::NamedGuids::end())
				j["fields"]["SubTypeGuid"]["value"] = section_guid;
			else
				j["fields"]["SubTypeGuid"] = guid_;
		} break;

		case SectionHeader::GuidDefined:
		{
			EFI_GUID section_guid;
			UINT16   section_data_offset = 0;
			UINT16   section_attributes = 0;
			if (Pi::Section::Utils::isExtendedSection(obj.header)) {
				Pi::Section::Extended::GuidDefined sv(obj.header.begin);
				section_guid = sv->SectionDefinitionGuid;
				section_data_offset = sv->DataOffset;
				section_attributes = sv->Attributes;
			}
			else
			{
				Pi::Section::GuidDefined sv(obj.header.begin);
				section_guid = sv->SectionDefinitionGuid;
				section_data_offset = sv->DataOffset;
				section_attributes = sv->Attributes;
			}
			auto& guid_ = Guid::NamedGuids::findNamedGuid(section_guid);
			if (&guid_ == Guid::NamedGuids::end())
				j["fields"]["SectionDefinitionGuid"]["value"] = section_guid;
			else
				j["fields"]["SectionDefinitionGuid"] = guid_;
			std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#hX)", section_data_offset, section_data_offset);
			j["fields"]["DataOffset"]["string"] = strBuffer;
			j["fields"]["DataOffset"]["value"] = section_data_offset;
			std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#hX)", section_attributes, section_attributes);
			switch (section_attributes)
			{
				case EFI_GUIDED_SECTION_PROCESSING_REQUIRED : j["fields"]["Attributes"]["string"] = "EFI_GUIDED_SECTION_PROCESSING_REQUIRED"; break;
				case EFI_GUIDED_SECTION_AUTH_STATUS_VALID   : j["fields"]["Attributes"]["string"] = "EFI_GUIDED_SECTION_AUTH_STATUS_VALID"; break;
				default                                     : j["fields"]["Attributes"]["string"] = strBuffer; break;
			}
			j["fields"]["Attributes"]["value"] = section_attributes;
		} break;

		case SectionHeader::UserInterface:
		{	// It is nearly impossible that this section will have type EFI_USER_INTERFACE_SECTION2
			auto ptr_ = reinterpret_cast<const char16_t*>(reinterpret_cast<Pi::Section::UserInterface::const_pointer_t>(obj.header.begin)->FileNameString);
			std::u16string u16str(ptr_, (Pi::Section::Utils::getSize(obj.header) - Pi::Section::Header::structure_size) / sizeof(CHAR16));
			std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
			j["fields"]["FileNameString"] = convert.to_bytes(u16str);
		} break;

		case SectionHeader::Version:
		{	// It is nearly impossible that this section will have type EFI_VERSION_SECTION2
			auto* ptr_ = reinterpret_cast<const char16_t*>(reinterpret_cast<Pi::Section::Version::const_pointer_t>(obj.header.begin)->VersionString);
			std::u16string u16str(ptr_, (Pi::Section::Utils::getSize(obj.header) - Pi::Section::Header::structure_size - sizeof(UINT16)) / sizeof(CHAR16));
			std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
			j["fields"]["VersionString"] = convert.to_bytes(u16str);
			UINT16 section_build_number = reinterpret_cast<Pi::Section::Version::const_pointer_t>(obj.header.begin)->BuildNumber;
			std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#hX)", section_build_number, section_build_number);
			j["fields"]["BuildNumber"]["string"] = strBuffer;
			j["fields"]["BuildNumber"]["value"] = section_build_number;
		} break;

		case SectionHeader::PostcodeSct:
		case SectionHeader::PostcodeInsyde:
		{	// It is nearly impossible that this section will have type POSTCODE_SECTION2
			UINT32 section_postcode = reinterpret_cast<Pi::Section::Postcode::const_pointer_t>(obj.header.begin)->Postcode;
			std::snprintf(strBuffer, sizeof(strBuffer), "%d (%#X)", section_postcode, section_postcode);
			j["fields"]["Postcode"]["string"] = strBuffer;
			j["fields"]["Postcode"]["value"] = section_postcode;
		} break;

		case SectionHeader::Raw:
		case SectionHeader::Te:
		case SectionHeader::Pic:
		case SectionHeader::Pe32:
		case SectionHeader::FirmwareVolumeImage:
		case SectionHeader::Disposable:
		case SectionHeader::Compatibility16:
		default: break;
	}
}

void Project::PiObject::Helper::to_json(nlohmann::json& j, const unique_section_decomp_buff_t& obj)
{
	j["length"] = obj->memory.getLength();
	switch (obj->decompresser)
	{
		case Decompression::Decompresser::TianoEdk  : j["algorithm"] = "TIANO EDK"; break;
		case Decompression::Decompresser::TianoEfi  : j["algorithm"] = "EFI1.1 EDK"; break;
		case Decompression::Decompresser::TianoEdk2 : j["algorithm"] = "TIANO EDK2"; break;
		case Decompression::Decompresser::TianoEfi2 : j["algorithm"] = "EFI1.1 EDK2"; break;
		case Decompression::Decompresser::Lzma      : j["algorithm"] = "LZMA"; break;
		case Decompression::Decompresser::Lzma86    : j["algorithm"] = "LZMA86"; break;
		case Decompression::Decompresser::Brotli    : j["algorithm"] = "BROTLI"; break;
		case Decompression::Decompresser::Unknown   : 
		default                                     : j["algorithm"] = "Unknown"; break;
	}
}


void Project::PiObject::Object::toJson(nlohmann::json& j) const
{
	char strBuffer[32];
	std::memset(strBuffer, 0, sizeof(strBuffer));
	j["UID"] = uid;
	j["position"] = OffsetView(baseBegin, memory);
	std::snprintf(strBuffer, sizeof(strBuffer), "%d (%#.8X)", static_cast<std::uint32_t>(state), static_cast<std::uint32_t>(state));
	j["state"]["string"] = strBuffer;
	j["state"]["value"] = static_cast<std::uint32_t>(state);
	j["name"] = ::Helper::getObjectName(*this);
}

void Project::PiObject::FreeSpace::toJson(nlohmann::json& j) const
{
	Base::toJson(j);
	j["corrupted"] = isCorrupted();
	// Restore empty
	j["empty"] = ((state & InconsistencyState::FreeSpaceEmpty) >> PROJ_FREE_SPACE_STATE_EMPTY_START_BIT);
}

void Project::PiObject::Data::toJson(nlohmann::json& j) const
{
	Base::toJson(j);
	const char* data_type = nullptr;
	switch (state)
	{
		case InconsistencyState::DataNormal : data_type = "Data"; break;
		case InconsistencyState::DataInvalidVolume :
		{
			data_type = "InvalidVolume";
			Pi::Volume::Header hdr(memory.begin);
			j["Invalid structure"] = hdr;
		} break;
		case InconsistencyState::DataInvalidFile:
		{
			data_type = "InvalidFile";
			Pi::File::Header hdr(memory.begin);
			j["Invalid structure"] = hdr;
		} break;
		case InconsistencyState::DataInvalidSection:
		{
			data_type = "InvalidSection";
			Pi::Section::Header hdr(memory.begin);
			j["Invalid structure"] = hdr;
		} break;
		default : data_type = "Invalid Data state"; break;
	}
	j["Data type"] = data_type;
}

void Project::PiObject::ComplexObject::toJson(nlohmann::json& j) const
{
	Base::toJson(j);
	if (!objects.empty()) {
		j["children"] = objects;
	}
}

void Project::PiObject::Section::toJson(nlohmann::json& j) const
{
	Base::toJson(j);

}

void Project::PiObject::File::toJson(nlohmann::json& j) const
{
	Base::toJson(j);

}

void Project::PiObject::Volume::toJson(nlohmann::json& j) const
{
	Base::toJson(j);

}