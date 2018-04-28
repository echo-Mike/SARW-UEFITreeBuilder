/// STD
#include <cstdio>
#include <algorithm>

/// PROJECT
#include "PiObjectRepresentation.hpp"
#include "PiSectionUtils.hpp"
#include "GuidAdvanced.hpp"

namespace Project {

namespace PiObject {

Types::memory_t default_empty = PROJ_DEFAULT_EMPTY;

namespace Helper
{

	static const char* getObjectName(const Object& obj)
	{
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

}

/**
 *    OBJECT
 */

void Object::toJson(nlohmann::json& j) const
{
	char strBuffer[32];
	std::memset(strBuffer, 0, sizeof(strBuffer));
	j["UID"] = uid;
	j["position"] = OffsetView(baseBegin, memory);
	std::snprintf(strBuffer, sizeof(strBuffer), "%d (%#.8X)", static_cast<std::uint32_t>(state), static_cast<std::uint32_t>(state));
	j["state"]["string"] = strBuffer;
	j["state"]["value"] = static_cast<std::uint32_t>(state);
	j["name"] = Helper::getObjectName(*this);
}

void Object::setUid(const MemoryView& buffer)
{
	uid = Checksums::hash(buffer, buffer.getLength());
}

void Object::setUid(MemoryView&& buffer)
{
	uid = Checksums::hash(buffer, buffer.getLength());
}

/**
 *    FREE_SPACE
 */

bool FreeSpace::IsCorrupted(Types::memory_t empty, const MemoryView& buffer)
{
	return (buffer.end != std::find_if_not(
		buffer.begin,
		buffer.end,
		[empty](const Types::memory_t v) -> bool
		{ 
			return v == empty; 
		}
	));
}

void FreeSpace::toJson(nlohmann::json& j) const
{
	Base::toJson(j);
	j["corrupted"] = isCorrupted();
	// Restore empty
	j["empty"] = ((state & InconsistencyState::FreeSpaceEmpty) >> PROJ_FREE_SPACE_STATE_EMPTY_START_BIT);
}

/**
 *    DATA
 */

void Data::toJson(nlohmann::json& j) const
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

/**
 *    COMPLEX_OBJECT
 */

void ComplexObject::toJson(nlohmann::json& j) const
{
	Base::toJson(j);
	if (!objects.empty()) {
		j["children"] = objects;
	}
}



namespace helper
{

	/*void to_json(nlohmann::json& j, const SectionHeader& obj)
	{
		switch (obj.header->Type) {
		// Encapsulation sections
		case EFI_SECTION_COMPRESSION :
		{

		} break;

		case EFI_SECTION_GUID_DEFINED :
		{

		} break;

		case EFI_SECTION_DISPOSABLE :
		{

		} break;

		// Leaf sections
		case EFI_SECTION_SMM_DEPEX :
		case EFI_SECTION_PEI_DEPEX :
		case EFI_SECTION_DXE_DEPEX :
		{

		} break;

		case EFI_SECTION_PE32 :
		case EFI_SECTION_PIC :
		{

		} break;

		case SCT_SECTION_POSTCODE :
		case INSYDE_SECTION_POSTCODE :
		{

		} break;

		case EFI_SECTION_TE :
		{

		} break;

		case EFI_SECTION_VERSION :
		{

		} break;

		case EFI_SECTION_USER_INTERFACE :
		{

		} break;

		case EFI_SECTION_COMPATIBILITY16 :
		{

		} break;

		case EFI_SECTION_FIRMWARE_VOLUME_IMAGE :
		{

		} break;

		case EFI_SECTION_FREEFORM_SUBTYPE_GUID :
		{

		} break;

		case EFI_SECTION_RAW :
		{

		} break;

		default :
		{

		} break;

		}
	}*/
}

}

namespace Pi {

namespace Volume {

void to_json(nlohmann::json& j, const Header& hdr)
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

}

namespace File {

void to_json(nlohmann::json& j, const Header& hdr)
{
	char strBuffer[128];
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
	std::snprintf(strBuffer, sizeof(strBuffer), "%d (%#X)", simple_size, simple_size);
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

}

namespace Section {

void to_json(nlohmann::json& j, const Header& hdr)
{

}

}

}

}