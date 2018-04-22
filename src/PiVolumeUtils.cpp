/// STD
#include <cstring>
#include <set>

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

		Types::length_t computeBlockMapLength(Pi::Volume::Header::const_pointer_t hdr)
		{

			Types::length_t result = 0;
			const EFI_FV_BLOCK_MAP_ENTRY* blocks = hdr->BlockMap;

			while (std::memcmp(blocks, &lastEntry, Pi::Volume::BlockMap::structure_size))
			{
				result += blocks->Length * blocks->NumBlocks;
				blocks = ADVANCE_PTR_(blocks, const EFI_FV_BLOCK_MAP_ENTRY*, Pi::Volume::BlockMap::structure_size);
			}

			return result;
		}

		typedef std::set< Pi::Volume::Header::const_pointer_t > parsed_volumes_storage_t;

		static parsed_volumes_storage_t parsedVolumes;

	}

	Types::length_t getSize(Pi::Volume::Header::const_pointer_t header)
	{
		Types::length_t volumeField = header->FvLength;
		Types::length_t blockMap = 0; 
		
		try {
			blockMap = Helper::computeBlockMapLength(header);
		}
		catch (const std::exception& e)
		{
			DEBUG_ERROR_MESSAGE
				DEBUG_PRINT("\tMessage: Error occurred during block map length calculation.");
				DEBUG_PRINT("\tErr message: ", e.what());
			DEBUG_END_MESSAGE
			return 0;
		}
		catch (...)
		{
			DEBUG_ERROR_MESSAGE
				DEBUG_PRINT("\tMessage: Error occurred during block map length calculation.");
			DEBUG_END_MESSAGE
			return 0;
		}
		
		if ( volumeField != blockMap )
		{
			DEBUG_INFO_MESSAGE
				DEBUG_PRINT("\tMessage: Volume block map length and value of FvLength do not match.");
				DEBUG_PRINT("\tGUID: ", header->FileSystemGuid);
				DEBUG_PRINT("\tFvLength: ", volumeField);
				DEBUG_PRINT("\tCalculated block map length: ", blockMap);
			DEBUG_END_MESSAGE

			throw std::length_error("");
		}
		return volumeField < blockMap ? blockMap : volumeField;
	}

	bool addParsedVolume(const Pi::Volume::Header& header)
	{
		return Helper::parsedVolumes.insert(header.get()).second;
	}

	bool isParsedVolume(const Pi::Volume::Header& header)
	{
		return Helper::parsedVolumes.count(header.get());
	}

} 
} 
} 
}