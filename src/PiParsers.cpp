/// STD
#include <cstring>
/// PROJECT
#include "vFFSGuids.hpp"
#include "PiFinders.hpp"
#include "PiParsers.hpp"

namespace Project
{
	namespace Parsers
	{

		namespace helper
		{
			static const EFI_FV_BLOCK_MAP_ENTRY lastEntry = { 0, 0 };
		}

		static Types::length_t getFileSize(const Pi::File::Header& header)
		{
			return  ( static_cast<Types::length_t>(header->Size[2]) << 16 ) +
					( static_cast<Types::length_t>(header->Size[1]) <<  8 ) +
					  static_cast<Types::length_t>(header->Size[0]);
		}

		static void processFileHeaders(PiObject::Volume& volume, const Finders::FilesVec_t& fileHeaders, const MemoryView& buffer)
		{
			Types::memory_t empty = volume.header->Attributes & EFI_FVB2_ERASE_POLARITY ? '\xFF' : '\x00';
			Parsers::FileParser fileParser;
			MemoryView fileMemory(buffer.begin, buffer.begin);

			// Call parser for every file header entry
			for (const auto& fileHeader : fileHeaders)
			{	// Check that previous file fill all space before current
				if (fileMemory.end != fileHeader.begin) {
					volume.freeSpace.emplace_back(empty, fileMemory.end, fileHeader.begin);
				}

				if (fileHeader->Attributes & FFS_ATTRIB_LARGE_FILE) {
					Pi::File::Extended::Header header(fileHeader.begin);
					fileMemory.begin = header.begin;
					fileMemory.end = header.begin + header->ExtendedSize;

					if (buffer.isOutside(fileMemory.end - 1))
					{
						DEBUG_ERROR_MESSAGE
							DEBUG_PRINT("\tMessage: File exceeds volume boundary.");
						DEBUG_END_MESSAGE_AND_EXIT(ExitCodes::ParseErrorVolume)
					}

					volume.files.emplace_back( fileParser(header, fileMemory, volume.memory, empty) );

				} else {
					fileMemory.begin = fileHeader.begin;
					fileMemory.end = fileHeader.begin + getFileSize(fileHeader);

					if (buffer.isOutside(fileMemory.end - 1))
					{
						DEBUG_ERROR_MESSAGE
							DEBUG_PRINT("\tMessage: File exceeds volume boundary.");
						DEBUG_END_MESSAGE_AND_EXIT(ExitCodes::ParseErrorVolume)
					}

					volume.files.emplace_back( fileParser(fileHeader, fileMemory, volume.memory, empty) );
				}
			}
			
			// Check for space after last file
			if (fileMemory.end != buffer.end) {
				volume.freeSpace.emplace_back(empty, fileMemory.end, buffer.end);
			}
		}

		PiObject::Volume VolumeParser::operator()(const Pi::Volume::Header& volumeView, const MemoryView& buffer, const MemoryView& baseBuffer)
		{
			PiObject::Volume volume(volumeView, baseBuffer, buffer);
			Finders::FilesVec_t fileViews;

			Types::memory_t empty = volume.header->Attributes & EFI_FVB2_ERASE_POLARITY ? '\xFF' : '\x00';

			// Define memory used by full header: Pi volume header + block map
			{	// Size of first entry is already counted in size of header structure itself
				Types::count_t blockEntryCount = 0;
				const EFI_FV_BLOCK_MAP_ENTRY* blocks = volume.header->BlockMap;

				while (std::memcmp(blocks, &helper::lastEntry, Pi::Volume::BlockMap::structure_size)) {
					++blockEntryCount;
					// Next will work only if alignment for EFI_FV_BLOCK_MAP_ENTRY is not weird
					++blocks;
				}
				// Count real size of base volume header: 
				//	sizeof(EFI_FIRMWARE_VOLUME_HEADER) + sizeof(EFI_FV_BLOCK_MAP_ENTRY) * blockEntryCount
				volume.fullHeader.setLength(Pi::Volume::Header::structure_size + Pi::Volume::BlockMap::structure_size * blockEntryCount);
			}

			// Consider memory used by extended header and determine areas of interests
			Types::count_t searchAreaCount = 0;
			// Find extension header and ban file search on it
			if (volume.header->ExtHeaderOffset)
			{
			 	volume.extHeader.header = volume.header.begin + volume.header->ExtHeaderOffset;
				volume.extHeader.memory = volume.extHeader.header;
				volume.extHeader.memory.setLength(volume.extHeader.header->ExtHeaderSize);

				if (volume.extHeader.memory.isInside(volume.fullHeader.end - 1))
					DEBUG_ERROR_MESSAGE
						DEBUG_PRINT("\tMessage: Block map collided with extended header.");
					DEBUG_END_MESSAGE_AND_EXIT(ExitCodes::ParseErrorVolume)

				// If there is no space between header and extended header than there is only one
				// search area, otherwise there is two
				searchAreaCount = volume.extHeader.memory.isInside(volume.fullHeader.end) ? 1 : 2;
				// Case when extended header is padded to a not significant value of bytes :
				// No legitimate file can be created in that space
				if (volume.extHeader.header.begin - volume.fullHeader.end < Pi::File::Header::structure_size)
				{	// Create new free space instance
					volume.freeSpace.emplace_back(empty, volume.fullHeader.end, volume.extHeader.header.begin);
					--searchAreaCount;
				}
			}

			// Find Pi file headers for all areas of interests based on their count
			{
				MemoryView volumeBody;

				switch (searchAreaCount)
				{
					case 0 : // No extended header found
						volumeBody.begin = ALIGN_PTR8(volume.fullHeader.begin, volume.fullHeader.end);
						volumeBody.setEnd(volume.memory.end);
						fileViews = Finders::FileFinder()(volumeBody);
						break;
					case 1 : // Extended header is right after block map or space between is insignificant
						volumeBody.begin = ALIGN_PTR8(volume.fullHeader.begin, volume.extHeader.memory.end);
						volumeBody.setEnd(volume.memory.end);
						fileViews = Finders::FileFinder()(volumeBody);
						break;
					case 2 : // Both areas: between block map and extended header and after extended header are significant
						volumeBody.begin = ALIGN_PTR8(volume.fullHeader.begin, volume.fullHeader.end);
						volumeBody.setEnd(volume.extHeader.header.begin);

						// Find all files in first area
						fileViews = Finders::FileFinder()(volumeBody);
						if (fileViews.empty()) { // No files found: add area to empty space
							volume.freeSpace.emplace_back(empty, volume.fullHeader.end, volume.extHeader.header.begin);
						} else { // Some files found: process them to file objects and free space 
							processFileHeaders(volume, fileViews, volumeBody);
							fileViews.clear();
						}

						volumeBody.begin = ALIGN_PTR8(volume.fullHeader.begin, volume.extHeader.memory.end);
						volumeBody.setEnd(volume.memory.end);
						// Find files in second area
						fileViews = Finders::FileFinder()(volumeBody);
						break;
					default:
						DEBUG_ERROR_MESSAGE
							DEBUG_PRINT("\tMessage: Unexpected count of volume search areas.");
						DEBUG_END_MESSAGE_AND_EXIT(ExitCodes::ParseErrorVolume)
						break;
				}

				// Process found files to file objects and free space 
				if (!fileViews.empty()) {
					processFileHeaders(volume, fileViews, volumeBody);
				} else { // Add whole volume body as free space
					DEBUG_WARNING_MESSAGE
						DEBUG_PRINT("\tMessage: No files found in volume.");
					DEBUG_END_MESSAGE
					volume.freeSpace.emplace_back(empty, volumeBody.begin, volumeBody.begin);
				}
			}

			// Return filled volume object
			return volume;
		}

		PiObject::File FileParser::operator()(const Pi::File::Header& fileView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty)
		{

		}

		PiObject::File FileParser::operator()(const Pi::File::Extended::Header& fileView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty)
		{

		}

		PiObject::Section SectionParser::operator()(const Pi::Section::Header& sectionView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty)
		{

		}

		PiObject::Section SectionParser::operator()(const Pi::Section::Extended::Header& sectionView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty)
		{

		}

	}
}