/// STD
#include <cstring>
#include <cstdio>
#include <string>
#include <codecvt>
#include <vector>
/// PROJECT
#include "JsonConverters.hpp"
#include "PiSectionUtils.hpp"
#include "PiFileUtils.hpp"
#include "PiVolumeUtils.hpp"
#include "GuidAdvanced.hpp"

namespace Helper
{

	static const EFI_FV_BLOCK_MAP_ENTRY lastEntry = { 0, 0 };

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

	static void parseDepexSection(nlohmann::json& /*j*/, const Project::Pi::Section::Header& /*hdr*/, Project::PiObject::Helper::SectionHeader::SectionType /*secType*/)
	{
		// EFI_DEP_AFTER
	}
}

void to_json(nlohmann::json& j, const EFI_GUID& guid)
{
	char guidstr[PROJ_64B];
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
	char strBuffer[PROJ_256B];
	auto ptr = reinterpret_cast<Types::const_byte_ptr_t>(hdr->ZeroVector);
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
	j["ZeroVector"] = strBuffer;
	// FileSystemGuid
	auto& guid_ = Guid::NamedGuids::findNamedGuid(hdr->FileSystemGuid);
	if (&guid_ == Guid::NamedGuids::end())
		j["FileSystemGuid"]["value"] = hdr->FileSystemGuid;
	else 
		j["FileSystemGuid"] = guid_;
	// FvLength
	std::snprintf(strBuffer, sizeof(strBuffer), "%lld (%#.16llX)", hdr->FvLength, hdr->FvLength);
	j["FvLength"]["string"] = strBuffer;
	j["FvLength"]["value"] = hdr->FvLength;
	// Signature
	strBuffer[0] = (hdr->Signature & 0xFF000000) >> 24;
	strBuffer[1] = (hdr->Signature & 0x00FF0000) >> 16;
	strBuffer[2] = (hdr->Signature & 0x0000FF00) >>  8;
	strBuffer[3] = (hdr->Signature & 0x000000FF);
	strBuffer[4] = '\0';
	j["Signature"] = strBuffer;
	// Attributes
	j["Attributes"]["string"] = Pi::Volume::Utils::getAtributeString(hdr);
	j["Attributes"]["alternative"] = Pi::Volume::Utils::getAtributeString(hdr, true);
	j["Attributes"]["value"] = hdr->Attributes;
	// HeaderLength
	std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#.4hX)", hdr->HeaderLength, hdr->HeaderLength);
	j["HeaderLength"]["string"] = strBuffer;
	j["HeaderLength"]["value"] = hdr->HeaderLength;
	// Checksum
	std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#.4hX)", hdr->Checksum, hdr->Checksum);
	j["Checksum"]["string"] = strBuffer;
	j["Checksum"]["value"] = hdr->Checksum;
	// ExtHeaderOffset
	std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#.4hX)", hdr->ExtHeaderOffset, hdr->ExtHeaderOffset);
	j["ExtHeaderOffset"]["string"] = strBuffer;
	j["ExtHeaderOffset"]["value"] = hdr->ExtHeaderOffset;
	// Reserved
	j["Reserved"] = hdr->Reserved;
	// Revision
	j["Revision"] = hdr->Revision;
}

void Project::to_json(nlohmann::json& j, const Project::Pi::File::Header& hdr)
{
	char strBuffer[PROJ_64B];
	// Structure fields
	// Name
	auto& guid_ = Guid::NamedGuids::findNamedGuid(hdr->Name);
	if (&guid_ == Guid::NamedGuids::end())
		j["Name"]["value"] = hdr->Name;
	else
		j["Name"] = guid_;
	// IntegrityCheck
	std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#.4hX)", hdr->IntegrityCheck.Checksum16, hdr->IntegrityCheck.Checksum16);
	j["IntegrityCheck"]["Checksum16"]["string"] = strBuffer;
	j["IntegrityCheck"]["Checksum16"]["value"] = hdr->IntegrityCheck.Checksum16;
	std::snprintf(strBuffer, sizeof(strBuffer), "%hhd (%#.2hhX)", hdr->IntegrityCheck.Checksum.Header, hdr->IntegrityCheck.Checksum.Header);
	j["IntegrityCheck"]["Header"]["string"] = strBuffer;
	j["IntegrityCheck"]["Header"]["value"] = hdr->IntegrityCheck.Checksum.Header;
	std::snprintf(strBuffer, sizeof(strBuffer), "%hhd (%#.2hhX)", hdr->IntegrityCheck.Checksum.File, hdr->IntegrityCheck.Checksum.File);
	j["IntegrityCheck"]["File"]["string"] = strBuffer;
	j["IntegrityCheck"]["File"]["value"] = hdr->IntegrityCheck.Checksum.File;
	// Type
	j["Type"]["string"] = Pi::File::Utils::fileTypeToCStr(hdr->Type);
	j["Type"]["value"] = hdr->Type;
	// Attributes
	j["Attributes"]["string"] = Pi::File::Utils::getAtributeString(hdr);
	j["Attributes"]["alternative"] = Pi::File::Utils::getAtributeString(hdr, true);
	j["Attributes"]["value"] = hdr->Attributes;
	// Size
	auto simple_size = Pi::File::Utils::getSize(hdr);
	std::snprintf(strBuffer, sizeof(strBuffer), "%lld (%#.16llX)", simple_size, simple_size);
	j["Size"]["string"] = strBuffer;
	j["Size"]["value"] = simple_size;
	// State
	j["State"]["string"] = Pi::File::Utils::getStateString(hdr);
	j["State"]["alternative"] = Pi::File::Utils::getStateString(hdr, true);
	j["State"]["value"] = hdr->State;
	// ExtendedSize
	if (hdr->Attributes & FFS_ATTRIB_LARGE_FILE)
	{
		auto extended_size = Pi::File::Utils::getSize2(hdr);
		std::snprintf(strBuffer, sizeof(strBuffer), "%lld (%#.16llX)", extended_size, extended_size);
		j["ExtendedSize"]["string"] = strBuffer;
		j["ExtendedSize"]["value"] = extended_size;
	}
}

void Project::to_json(nlohmann::json& j, const Project::Pi::Section::Header& hdr)
{
	char strBuffer[PROJ_64B];
	// Structure fields
	// Size
	auto simple_size = Pi::Section::Utils::getSize(hdr);
	std::snprintf(strBuffer, sizeof(strBuffer), "%lld (%#.16llX)", simple_size, simple_size);
	j["Size"]["string"] = strBuffer;
	j["Size"]["value"] = simple_size;
	// Type
	j["Type"]["string"] = Pi::Section::Utils::sectionTypeToCStr(hdr->Type);
	j["Type"]["value"] = hdr->Type;
	// ExtendedSize
	if (Pi::Section::Utils::isExtendedSection(hdr))
	{
		auto extended_size = Pi::Section::Utils::getSize2(hdr);
		std::snprintf(strBuffer, sizeof(strBuffer), "%lld (%#.16llX)", extended_size, extended_size);
		j["ExtendedSize"]["string"] = strBuffer;
		j["ExtendedSize"]["value"] = extended_size;
	}
}

void Project::to_json(nlohmann::json& j, const Pi::Volume::Extension::Entry& entry)
{
	char strBuffer[PROJ_256B];
	std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#.4hX)", entry->ExtEntryType, entry->ExtEntryType);
	j["ExtEntryType"]["value"] = entry->ExtEntryType;
	switch (entry->ExtEntryType)
	{
		case EFI_FV_EXT_TYPE_OEM_TYPE:
		{
			Pi::Volume::Extension::EntryOemType fullEntry(entry.begin);
			std::memset(strBuffer, 0, sizeof(strBuffer));
			char* ptr = strBuffer;
			Types::count_t usedOemTypesCount = 0;
			for (char i = 0; i < sizeof(fullEntry->TypeMask) * 8; ++i)
			{
				if ((1ul << i) & fullEntry->TypeMask)
				{                    //"0xXX | "
					std::snprintf(ptr, (2 + 2 + 3 + 1), "%#.2hhX | ", EFI_FV_FILETYPE_OEM_MIN + i);
					ptr += 7; // Now points to char(0) putted by std::snprintf
					++usedOemTypesCount;
				}
			}
			if (ptr != strBuffer) // At least one OEM type is used
			{
				ptr -= 2; // Now points to '|' char
			}                //"(0xXXXXXXXX)"
			std::snprintf(ptr, (2 + 2 + 8 + 1), "(%#.8X)", fullEntry->TypeMask);
			j["TypeMask"] = strBuffer;
			if (usedOemTypesCount && entry->ExtEntrySize >= Pi::Volume::Extension::EntryOemType::structure_size + sizeof(EFI_GUID) * usedOemTypesCount)
			{	// Entry have enough memory space to hold all declared GUIDs
				std::vector<EFI_GUID> guidVec;
				auto guid = ADVANCE_PTR_(fullEntry.begin, const EFI_GUID*, Pi::Volume::Extension::EntryOemType::structure_size);
				for (Types::count_t i = 0; i < usedOemTypesCount; ++i)
				{
					guidVec.emplace_back(*guid);
					guid = ADVANCE_PTR_(guid, const EFI_GUID*, PROJ_GUID_SIZE);
				}
				j["GUIDs"] = guidVec;
			}
			std::memcpy(strBuffer, "EFI_FV_EXT_TYPE_OEM_TYPE", sizeof("EFI_FV_EXT_TYPE_OEM_TYPE"));
		} break;

		case EFI_FV_EXT_TYPE_GUID_TYPE:
		{
			Pi::Volume::Extension::EntryGuidType fullEntry(entry.begin);
			j["FormatType"]["value"] = fullEntry->FormatType;

			if (fullEntry->FormatType == EFI_GUID EFI_FIRMWARE_CONTENTS_SIGNED_GUID)
			{
				j["FormatType"]["name"] = "EFI_FIRMWARE_CONTENTS_SIGNED_GUID";
				Guid::FindGUIDResult::result_t searchResult;
				MemoryView body(fullEntry.end, entry.begin + entry->ExtEntrySize);
				if (Guid::findGuid(EFI_GUID EFI_CERT_TYPE_RSA2048_SHA256_GUID, body, searchResult) == Guid::FindGUIDResult::Found)
				{
					j["CertType"] = "EFI_CERT_TYPE_RSA2048_SHA256_GUID";
				}
				else if (Guid::findGuid(EFI_GUID EFI_CERT_TYPE_PKCS7_GUID, body, searchResult) == Guid::FindGUIDResult::Found)
				{
					j["CertType"] = "EFI_CERT_TYPE_PKCS7_GUID";
				}
				else
				{
					j["CertType"] = "UNKNOWN";
				}
			}

			std::memcpy(strBuffer, "EFI_FV_EXT_TYPE_GUID_TYPE", sizeof("EFI_FV_EXT_TYPE_GUID_TYPE"));
		} break;

		case EFI_FV_EXT_TYPE_USED_SIZE_TYPE:
		{
			Pi::Volume::Extension::EntryUsedSize fullEntry(entry.begin);
			std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#.8X)", fullEntry->UsedSize, fullEntry->UsedSize);
			j["UsedSize"] = strBuffer;
			std::memcpy(strBuffer, "EFI_FV_EXT_TYPE_USED_SIZE_TYPE", sizeof("EFI_FV_EXT_TYPE_USED_SIZE_TYPE"));
		} break;

		default: break;
	}
	j["ExtEntryType"]["string"] = strBuffer;
}


void Project::PiObject::Helper::to_json(nlohmann::json& j, const SectionHeader& obj)
{
	char strBuffer[PROJ_64B];
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
			std::snprintf(strBuffer, sizeof(strBuffer), "%d (%#.8X)", section_uncompressedLength, section_uncompressedLength);
			j["UncompressedLength"]["string"] = strBuffer;
			j["UncompressedLength"]["value"] = section_uncompressedLength;
			std::snprintf(strBuffer, sizeof(strBuffer), "%hhd (%#.2hhX)", section_compressionType, section_compressionType);
			switch (section_compressionType)
			{
				case EFI_NOT_COMPRESSED       : j["CompressionType"]["string"] = "EFI_NOT_COMPRESSED"; break;
				case EFI_STANDARD_COMPRESSION : j["CompressionType"]["string"] = "EFI_STANDARD_COMPRESSION"; break;
				default                       : j["CompressionType"]["string"] = strBuffer; break;
			}
			j["CompressionType"]["value"] = section_compressionType;
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
				j["SubTypeGuid"]["value"] = section_guid;
			else
				j["SubTypeGuid"] = guid_;
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
				j["SectionDefinitionGuid"]["value"] = section_guid;
			else
				j["SectionDefinitionGuid"] = guid_;
			std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#.4hX)", section_data_offset, section_data_offset);
			j["DataOffset"]["string"] = strBuffer;
			j["DataOffset"]["value"] = section_data_offset;
			std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#.4hX)", section_attributes, section_attributes);
			switch (section_attributes)
			{
				case EFI_GUIDED_SECTION_PROCESSING_REQUIRED : j["Attributes"]["string"] = "EFI_GUIDED_SECTION_PROCESSING_REQUIRED"; break;
				case EFI_GUIDED_SECTION_AUTH_STATUS_VALID   : j["Attributes"]["string"] = "EFI_GUIDED_SECTION_AUTH_STATUS_VALID"; break;
				default                                     : j["Attributes"]["string"] = strBuffer; break;
			}
			j["Attributes"]["value"] = section_attributes;
		} break;

		case SectionHeader::UserInterface:
		{	// It is nearly impossible that this section will have type EFI_USER_INTERFACE_SECTION2
			auto ptr_ = reinterpret_cast<const char16_t*>(reinterpret_cast<Pi::Section::UserInterface::const_pointer_t>(obj.header.begin)->FileNameString);
			std::u16string u16str(ptr_, (Pi::Section::Utils::getSize(obj.header) - Pi::Section::Header::structure_size) / sizeof(CHAR16));
			std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
			j["FileNameString"] = convert.to_bytes(u16str);
		} break;

		case SectionHeader::Version:
		{	// It is nearly impossible that this section will have type EFI_VERSION_SECTION2
			auto* ptr_ = reinterpret_cast<const char16_t*>(reinterpret_cast<Pi::Section::Version::const_pointer_t>(obj.header.begin)->VersionString);
			std::u16string u16str(ptr_, (Pi::Section::Utils::getSize(obj.header) - Pi::Section::Header::structure_size - sizeof(UINT16)) / sizeof(CHAR16));
			std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
			j["VersionString"] = convert.to_bytes(u16str);
			UINT16 section_build_number = reinterpret_cast<Pi::Section::Version::const_pointer_t>(obj.header.begin)->BuildNumber;
			std::snprintf(strBuffer, sizeof(strBuffer), "%hd (%#.4hX)", section_build_number, section_build_number);
			j["BuildNumber"]["string"] = strBuffer;
			j["BuildNumber"]["value"] = section_build_number;
		} break;

		case SectionHeader::PostcodeSct:
		case SectionHeader::PostcodeInsyde:
		{	// It is nearly impossible that this section will have type POSTCODE_SECTION2
			UINT32 section_postcode = reinterpret_cast<Pi::Section::Postcode::const_pointer_t>(obj.header.begin)->Postcode;
			std::snprintf(strBuffer, sizeof(strBuffer), "%d (%#.8X)", section_postcode, section_postcode);
			j["Postcode"]["string"] = strBuffer;
			j["Postcode"]["value"] = section_postcode;
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
	char strBuffer[PROJ_32B];
	std::memset(strBuffer, 0, sizeof(strBuffer));
	j["UID"] = uid;
	j["position"] = OffsetView(baseBegin, memory);
	std::snprintf(strBuffer, sizeof(strBuffer), "%d (%#.8X)", static_cast<std::uint32_t>(state), static_cast<std::uint32_t>(state));
	j["state"]["string"] = strBuffer;
	j["state"]["value"] = static_cast<std::uint32_t>(state);
	j["Object Type"] = ::Helper::getObjectName(*this);
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
			data_type = "Firmware Volume";
			Pi::Volume::Header hdr(memory.begin);
			j["Fields"] = hdr;
		} break;
		case InconsistencyState::DataInvalidFile:
		{
			data_type = "Firmware File";
			Pi::File::Header hdr(memory.begin);
			j["Fields"] = hdr;
		} break;
		case InconsistencyState::DataInvalidSection:
		{
			data_type = "Firmware Section";
			Pi::Section::Header hdr(memory.begin);
			j["Fields"] = hdr;
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
	j["header"] = header;
	if (header.sectionType == Helper::SectionHeader::Compression ||
		header.sectionType == Helper::SectionHeader::GuidDefined)
	{
		auto storage = getDecomressedDataStorage();
		if (storage)
		{
			auto iter = storage->find(getUid());
			if (iter != storage->end())
			{
				j["uncompressed data"] = iter->second;
			} 
			else if (header.sectionType == Helper::SectionHeader::GuidDefined)
			{
				//TODO: add parsing of not compressed GUIDed sections <= Currently implemented in debug log form.
			}
		}
	}
	Base::toJson(j);
}

void Project::PiObject::File::toJson(nlohmann::json& j) const
{
	j["header"] = header.header;
	auto name = Pi::File::Utils::findFileName(objects);
	if (!name.empty()) {
		j["File name"] = std::move(name);
	}
	Base::toJson(j);
}

void Project::PiObject::Volume::toJson(nlohmann::json& j) const
{
	switch (state)
	{
		case InconsistencyState::VolumeNormal:
		{   // Basic header fields
			j["header"] = normalHdr;
			{   // Volume name
				auto& guid_ = Guid::NamedGuids::findNamedGuid(extendedHdr->FvName);
				if (&guid_ == Guid::NamedGuids::end())
					j["FvName"]["value"] = extendedHdr->FvName;
				else
					j["FvName"] = guid_;
			}
			// Extended header
			auto entryVec = Pi::Volume::Utils::findExtendedHeaderEntries(extendedHdr, fullExtendedHdr);
			if (!entryVec.empty())
			{
				j["Extended header entries"] = entryVec;
			}
			// Block map
			if (normalHdr.getLength() != fullNormalHdr.getLength())
			{
				j["BlockMap"] = nlohmann::json::array();
				auto* ptr = normalHdr->BlockMap;
				do {
					j["BlockMap"].emplace_back(nlohmann::json::object({
						{ "NumBlocks", ptr->NumBlocks },
						{ "Length", ptr->Length }
					}));
					ptr = ADVANCE_PTR_(ptr, const EFI_FV_BLOCK_MAP_ENTRY*, Pi::Volume::BlockMap::structure_size);
				} while (fullNormalHdr.isInside(UnifyPtrCast(ptr)) && !std::memcmp(ptr, &::Helper::lastEntry, Pi::Volume::BlockMap::structure_size));
			}
		} break;

		case InconsistencyState::VolumeBlockMapCollision:
		case InconsistencyState::VolumeInvalidExtendedHeader:
		case InconsistencyState::VolumeUnknownFileSystem:
			j["header"] = normalHdr;
			break;

		case InconsistencyState::VolumeAlreadyParsed:
			j["Message"] = "ALREADY_PARSED";
		default: break;
	}

	Base::toJson(j);
}