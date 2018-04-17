/// STD
#include <cstring>

/// PROJECT
#include "vFFSGuids.hpp"
#include "PiFinders.hpp"
#include "PiParsers.hpp"
#include "PiFileUtils.hpp"
#include "PiSectionUtils.hpp"

namespace Project
{

	namespace Parsers
	{

		namespace VolumeParserNs
		{

			static const EFI_FV_BLOCK_MAP_ENTRY lastEntry = { 0, 0 };

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

					fileMemory.begin = fileHeader.begin;
					fileMemory.end = fileHeader.begin + Pi::File::Utils::getSizeAuto(fileHeader.get());

					if (buffer.isOutside(fileMemory.end - 1))
						DEBUG_ERROR_MESSAGE
							DEBUG_PRINT("\tMessage: File exceeds volume boundary.");
						DEBUG_END_MESSAGE_AND_EXIT(ExitCodes::ParseErrorVolume)

					volume.files.emplace_back(fileParser(fileHeader, fileMemory, volume.memory, empty));
				}
			
				// Check for space after last file
				if (fileMemory.end != buffer.end) {
					volume.freeSpace.emplace_back(empty, fileMemory.end, buffer.end);
				}
			}
		}

		PiObject::Volume VolumeParser::operator()(const Pi::Volume::Header& volumeView, const MemoryView& buffer, const MemoryView& baseBuffer)
		{
			using namespace VolumeParserNs;

			PiObject::Volume volume(volumeView, baseBuffer, buffer);
			Finders::FilesVec_t fileViews;

			Types::memory_t empty = volume.header->Attributes & EFI_FVB2_ERASE_POLARITY ? '\xFF' : '\x00';

			// Define memory used by full header: Pi volume header + block map
			{	// Size of first entry is already counted in size of header structure itself
				Types::count_t blockEntryCount = 0;
				const EFI_FV_BLOCK_MAP_ENTRY* blocks = volume.header->BlockMap;

				while (std::memcmp(blocks, &lastEntry, Pi::Volume::BlockMap::structure_size)) {
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
				Finders::FileFinder fileFinder;

				switch (searchAreaCount)
				{
					case 0 : // No extended header found
						volumeBody.begin = ALIGN_PTR8(volume.fullHeader.begin, volume.fullHeader.end);
						volumeBody.setEnd(volume.memory.end);
						fileViews = fileFinder(volumeBody, empty);
						break;
					case 1 : // Extended header is right after block map or space between is insignificant
						volumeBody.begin = ALIGN_PTR8(volume.fullHeader.begin, volume.extHeader.memory.end);
						volumeBody.setEnd(volume.memory.end);
						fileViews = fileFinder(volumeBody, empty);
						break;
					case 2 : // Both areas: between block map and extended header and after extended header are significant
						volumeBody.begin = ALIGN_PTR8(volume.fullHeader.begin, volume.fullHeader.end);
						volumeBody.setEnd(volume.extHeader.header.begin);

						// Find all files in first area
						fileViews = fileFinder(volumeBody, empty);
						if (fileViews.empty()) { // No files found: add area to empty space
							volume.freeSpace.emplace_back(empty, volume.fullHeader.end, volume.extHeader.header.begin);
						} else { // Some files found: process them to file objects and free space 
							processFileHeaders(volume, fileViews, volumeBody);
							fileViews.clear();
						}

						volumeBody.begin = ALIGN_PTR8(volume.fullHeader.begin, volume.extHeader.memory.end);
						volumeBody.setEnd(volume.memory.end);
						// Find files in second area
						fileViews = fileFinder(volumeBody, empty);
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

		namespace FileParserNs
		{
			static PiObject::File fileByHeader(const Pi::File::Header& fileView, const MemoryView& buffer, const MemoryView& baseBuffer)
			{
				if (fileView->Attributes & FFS_ATTRIB_LARGE_FILE) {
					return PiObject::File(Pi::File::Extended::Header(fileView.begin), baseBuffer, buffer);
				}
				return PiObject::File(fileView, baseBuffer, buffer);
			}

			static void processSectionHeaders(PiObject::File& file, const Finders::SectionsVec_t& sectionHeaders, const MemoryView& buffer, Types::memory_t empty)
			{
				Parsers::SectionParser sectionParser;
				MemoryView sectionMemory(buffer.begin, buffer.begin);

				// Call parser for every section header entry
				for (const auto& sectionHdr : sectionHeaders)
				{	// Check that previous section fill all space before current
					if (sectionMemory.end != sectionMemory.begin) {
						file.freeSpace.emplace_back(empty, sectionMemory.end, sectionMemory.begin);
					}

					sectionMemory.begin = sectionHdr.begin;
					sectionMemory.end = sectionHdr.begin + Pi::Section::Utils::getSizeAuto(sectionHdr.get());

					if (buffer.isOutside(sectionMemory.end - 1))
						DEBUG_ERROR_MESSAGE
							DEBUG_PRINT("\tMessage: Section exceeds file boundary.");
						DEBUG_END_MESSAGE_AND_EXIT(ExitCodes::ParseErrorFile)

					file.sections.emplace_back(sectionParser(sectionHdr, sectionMemory, file.memory, empty));
				}
			
				// Check for space after last section
				if (sectionMemory.end != buffer.end) {
					file.freeSpace.emplace_back(empty, sectionMemory.end, buffer.end);
				}
			}
		}

		PiObject::File FileParser::operator()(const Pi::File::Header& fileView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty)
		{
			using namespace FileParserNs;

			auto file = fileByHeader(fileView, buffer, baseBuffer);
			Finders::SectionsVec_t sections;

			// Find all PI section headers in body of file
			{
				MemoryView fileBody;
				Finders::SectionFinder sectionFinder;

				if (file.header.isExtended()) {
					fileBody.begin = file.header.extended.end;
					fileBody.setEnd(buffer.end);
				} else {
					fileBody.begin = file.header.header.end;
					fileBody.setEnd(buffer.end);
				}

				if (fileBody.getLength() == 0) {
					DEBUG_INFO_MESSAGE
						DEBUG_PRINT("\tMessage: File without body found.");
						DEBUG_PRINT("\tGUID: ", file.header.asSimpleHeader()->Name);
					DEBUG_END_MESSAGE
				} else {
					sections = sectionFinder(fileBody, empty);
					if (sections.empty()) 
					{	// File must be one of non-sectioned type
						auto fileType = static_cast<Types::memory_t>(file.header.asSimpleHeader()->Type);
						// Check that file is not a sectioned type
						if ( Pi::File::Utils::isSectionedFileType(fileType) )
						{	// It is sectioned type by UEFI PI : report a warning
							DEBUG_WARNING_MESSAGE
								DEBUG_PRINT("\tMessage: No section found in sectioned file type.");
								DEBUG_PRINT("\tGUID: ", file.header.asSimpleHeader()->Name);
								DEBUG_PRINT("\tFile type: ", Pi::File::Utils::fileTypeToCStr(fileType));
							DEBUG_END_MESSAGE
						}
					} else {
						processSectionHeaders(file, sections, fileBody, empty);
					}
				}
			}

			return file;
		}

		PiObject::Section SectionParser::operator()(const Pi::Section::Header& sectionView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty)
		{

		}

	}
}