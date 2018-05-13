/// STD
#include <cstring>
#include <set>
#include <cstdio>

/// PROJECT
#include "PiVolumeUtils.hpp"
#include "GuidAdvanced.hpp"

namespace Project {
namespace Pi {
namespace Volume {
namespace Utils {

	namespace Helper
	{

		static const EFI_FV_BLOCK_MAP_ENTRY lastEntry = { 0, 0 };

		Types::length_t computeBlockMapLength(const Pi::Volume::Header::const_pointer_t hdr)
		{

			Types::length_t result = 0;
			const EFI_FV_BLOCK_MAP_ENTRY* blocks = hdr->BlockMap;

			while (std::memcmp(blocks, &lastEntry, Pi::Volume::BlockMap::structure_size))
			{
				result += static_cast<Types::length_t>(blocks->Length) * blocks->NumBlocks;
				blocks = ADVANCE_PTR_(blocks, const EFI_FV_BLOCK_MAP_ENTRY*, Pi::Volume::BlockMap::structure_size);
			}

			return result;
		}

		typedef std::set< Pi::Volume::Header::const_pointer_t > parsed_volumes_storage_t;

		static parsed_volumes_storage_t parsedVolumes;

		static const char* getVolumeAlignment(const Pi::Volume::Header::const_pointer_t header)
		{
			switch (header->Attributes & EFI_FVB2_ALIGNMENT)
			{
				case EFI_FVB2_ALIGNMENT_1    : return "1B";
				case EFI_FVB2_ALIGNMENT_2    : return "2B";
				case EFI_FVB2_ALIGNMENT_4    : return "4B";
				case EFI_FVB2_ALIGNMENT_8    : return "8B";
				case EFI_FVB2_ALIGNMENT_16   : return "16B";
				case EFI_FVB2_ALIGNMENT_32   : return "32B";
				case EFI_FVB2_ALIGNMENT_64   : return "64B";
				case EFI_FVB2_ALIGNMENT_128  : return "128B";
				case EFI_FVB2_ALIGNMENT_256  : return "256B";
				case EFI_FVB2_ALIGNMENT_512  : return "512B";
				case EFI_FVB2_ALIGNMENT_1K   : return "1K";
				case EFI_FVB2_ALIGNMENT_2K   : return "2K";
				case EFI_FVB2_ALIGNMENT_4K   : return "4K";
				case EFI_FVB2_ALIGNMENT_8K   : return "8K";
				case EFI_FVB2_ALIGNMENT_16K  : return "16K";
				case EFI_FVB2_ALIGNMENT_32K  : return "32K";
				case EFI_FVB2_ALIGNMENT_64K  : return "64K";
				case EFI_FVB2_ALIGNMENT_128K : return "128K";
				case EFI_FVB2_ALIGNMENT_256K : return "256K";
				case EFI_FVB2_ALIGNMENT_512K : return "512K";
				case EFI_FVB2_ALIGNMENT_1M   : return "1M";
				case EFI_FVB2_ALIGNMENT_2M   : return "2M";
				case EFI_FVB2_ALIGNMENT_4M   : return "4M";
				case EFI_FVB2_ALIGNMENT_8M   : return "8M";
				case EFI_FVB2_ALIGNMENT_16M  : return "16M";
				case EFI_FVB2_ALIGNMENT_32M  : return "32M";
				case EFI_FVB2_ALIGNMENT_64M  : return "64M";
				case EFI_FVB2_ALIGNMENT_128M : return "128M";
				case EFI_FVB2_ALIGNMENT_256M : return "256M";
				case EFI_FVB2_ALIGNMENT_512M : return "512M";
				case EFI_FVB2_ALIGNMENT_1G   : return "1G";
				case EFI_FVB2_ALIGNMENT_2G   : return "2G";
				default                      : return "Unknown";
			}
		}

	}

	Types::length_t getSize(const Pi::Volume::Header::const_pointer_t header)
	{
		Types::length_t volumeField = header->FvLength;
		// There may be out of bound access
		Types::length_t blockMap = Helper::computeBlockMapLength(header);
		if ( volumeField != blockMap ) DEBUG_WARNING_MESSAGE
				DEBUG_PRINT("\tMessage: Volume block map length and value of FvLength do not match.");
				DEBUG_PRINT("\tGUID: ", header->FileSystemGuid);
				DEBUG_PRINT("\tFvLength: ", volumeField);
				DEBUG_PRINT("\tCalculated block map length: ", blockMap);
			DEBUG_END_MESSAGE
		return volumeField;
	}

	Types::length_t getHeaderSize(const Pi::Volume::Header::const_pointer_t header)
	{	// Size of first block map entry is already counted in size of header structure itself
		using namespace Helper;
		Types::count_t blockEntryCount = 0;
		const EFI_FV_BLOCK_MAP_ENTRY* blocks = header->BlockMap;
		while (std::memcmp(blocks, &lastEntry, Pi::Volume::BlockMap::structure_size)) {
			++blockEntryCount;
			blocks = ADVANCE_PTR_(blocks, const EFI_FV_BLOCK_MAP_ENTRY*, Pi::Volume::BlockMap::structure_size);
		}
		// Count real size of base volume header: 
		//	sizeof(EFI_FIRMWARE_VOLUME_HEADER) + sizeof(EFI_FV_BLOCK_MAP_ENTRY) * blockEntryCount
		return Pi::Volume::Header::structure_size + Pi::Volume::BlockMap::structure_size * blockEntryCount;
	}

	bool addParsedVolume(const Pi::Volume::Header& header)
	{
		return Helper::parsedVolumes.insert(header.get()).second;
	}

	bool isParsedVolume(const Pi::Volume::Header& header)
	{
		return Helper::parsedVolumes.count(header.get());
	}

	std::string getAtributeString(const Pi::Volume::Header::const_pointer_t header, bool alternative)
	{
		char strBuffer[PROJ_1KB];
		std::memset(strBuffer, 0, sizeof(strBuffer));
		if (alternative)
		{
			std::snprintf(strBuffer, sizeof(strBuffer),
"READ_DISABLED : %c | READ_ENABLED : %c | READ_STATUS : %c |\
 WRITE_DISABLED : %c | WRITE_ENABLED : %c | WRITE_STATUS : %c |\
 LOCK : %c | LOCK_STATUS : %c |\
 STICKY_WRITE : %c |\
 MEMORY_MAPPED : %c |\
 ERASE_POLARITY %#.2hhX |\
 READ_LOCK : %c | READ_LOCK_STATUS : %c |\
 WRITE_LOCK : %c | WRITE_LOCK_STATUS : %c |\
 ALIGNMENT : %s |\
 WEAK_ALIGNMENT : %c\
 ( %#.8X )",
				header->Attributes & EFI_FVB2_READ_DISABLED_CAP  ? 'T' : 'F',
				header->Attributes & EFI_FVB2_READ_ENABLED_CAP   ? 'T' : 'F',
				header->Attributes & EFI_FVB2_READ_STATUS        ? 'T' : 'F',
				header->Attributes & EFI_FVB2_WRITE_DISABLED_CAP ? 'T' : 'F',
				header->Attributes & EFI_FVB2_WRITE_ENABLED_CAP  ? 'T' : 'F',
				header->Attributes & EFI_FVB2_WRITE_STATUS       ? 'T' : 'F',
				header->Attributes & EFI_FVB2_LOCK_CAP           ? 'T' : 'F',
				header->Attributes & EFI_FVB2_LOCK_STATUS        ? 'T' : 'F',
				header->Attributes & EFI_FVB2_STICKY_WRITE       ? 'T' : 'F',
				header->Attributes & EFI_FVB2_MEMORY_MAPPED      ? 'T' : 'F',
				header->Attributes & EFI_FVB2_ERASE_POLARITY     ? 0xFF : 0x00,
				header->Attributes & EFI_FVB2_READ_LOCK_CAP      ? 'T' : 'F',
				header->Attributes & EFI_FVB2_READ_LOCK_STATUS   ? 'T' : 'F',
				header->Attributes & EFI_FVB2_WRITE_LOCK_CAP     ? 'T' : 'F',
				header->Attributes & EFI_FVB2_WRITE_LOCK_STATUS  ? 'T' : 'F',
				Helper::getVolumeAlignment(header),
				header->Attributes & EFI_FVB2_WEAK_ALIGNMENT     ? 'T' : 'F',
				header->Attributes
			);
		}
		else 
		{
			std::snprintf(strBuffer, sizeof(strBuffer),
				"%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s ALIGNMENT : %s ( %#.8X )",
				header->Attributes & EFI_FVB2_READ_DISABLED_CAP  ? " READ_DISABLED |"     : "",
				header->Attributes & EFI_FVB2_READ_ENABLED_CAP   ? " READ_ENABLED |"      : "",
				header->Attributes & EFI_FVB2_READ_STATUS        ? " READ_STATUS |"       : "",
				header->Attributes & EFI_FVB2_WRITE_DISABLED_CAP ? " WRITE_DISABLED |"    : "",
				header->Attributes & EFI_FVB2_WRITE_ENABLED_CAP  ? " WRITE_ENABLED |"     : "",
				header->Attributes & EFI_FVB2_WRITE_STATUS       ? " WRITE_STATUS |"      : "",
				header->Attributes & EFI_FVB2_LOCK_CAP           ? " LOCK |"              : "",
				header->Attributes & EFI_FVB2_LOCK_STATUS        ? " LOCK_STATUS |"       : "",
				header->Attributes & EFI_FVB2_STICKY_WRITE       ? " STICKY_WRITE |"      : "",
				header->Attributes & EFI_FVB2_MEMORY_MAPPED      ? " MEMORY_MAPPED |"     : "",
				header->Attributes & EFI_FVB2_ERASE_POLARITY     ? " ERASE_POLARITY |"    : "",
				header->Attributes & EFI_FVB2_READ_LOCK_CAP      ? " READ_LOCK |"         : "",
				header->Attributes & EFI_FVB2_READ_LOCK_STATUS   ? " READ_LOCK_STATUS |"  : "",
				header->Attributes & EFI_FVB2_WRITE_LOCK_CAP     ? " WRITE_LOCK |"        : "",
				header->Attributes & EFI_FVB2_WRITE_LOCK_STATUS  ? " WRITE_LOCK_STATUS |" : "",
				header->Attributes & EFI_FVB2_WEAK_ALIGNMENT     ? " WEAK_ALIGNMENT |"    : "",
				Helper::getVolumeAlignment(header),
				header->Attributes
			);
		}
		return std::string(strBuffer);
	}

	extended_header_entries_vec_t findExtendedHeaderEntries(const Extension::Header& hdr, const MemoryView& buffer)
	{
		extended_header_entries_vec_t result;
		if (hdr->ExtHeaderSize > Extension::Header::structure_size)
		{
			auto entryPtr = ADVANCE_PTR_(hdr.begin, Extension::Entry::const_pointer_t, Extension::Header::structure_size);
			while (buffer.isInside(entryPtr))
			{
				result.emplace_back(entryPtr);
				entryPtr = ADVANCE_PTR_(entryPtr, Extension::Entry::const_pointer_t, entryPtr->ExtEntrySize);
			}
			if (UnifyPtrCast(entryPtr) > buffer.end) DEBUG_WARNING_MESSAGE
				DEBUG_PRINT("\tMessage: Last extended volume header entry exceeds provided boundary.");
			DEBUG_END_MESSAGE
		}
		return result;
	}

} 
} 
} 
}