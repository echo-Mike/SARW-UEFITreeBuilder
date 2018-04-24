/// STD
#include <cstring>

/// PROJECT
#include "GuidAdvanced.hpp"
#include "PiFinders.hpp"
#include "PiParsers.hpp"
#include "PiVolumeUtils.hpp"
#include "PiFileUtils.hpp"
#include "PiSectionUtils.hpp"
#include "DecompressionModule.hpp"

namespace Project
{

	namespace Parsers
	{

		namespace FreeSpaceParserNs
		{

			void parseVolume(PiObject::object_vec_t& results, const Pi::Volume::Header& header, const MemoryView& buffer)
			{
				MemoryView volumeBody(header.begin, header.begin + Pi::Volume::Utils::getSize(header));

				if (buffer.isOutside(volumeBody.end - 1))
					DEBUG_ERROR_MESSAGE
						OffsetView ov(buffer.begin, volumeBody);
						DEBUG_PRINT("\tMessage: Volume exceeds provided buffer.");
						DEBUG_PRINT("\tVolume offset: ", ov.offset);
						DEBUG_PRINT("\tVolume length: ", ov.length);
						DEBUG_PRINT("\tBuffer length: ", buffer.getLength());
						DEBUG_PRINT("\tExceeds by: ", ov.offset + ov.length - buffer.getLength());
					DEBUG_END_MESSAGE_AND_EXIT(ExitCodes::ParseErrorFreeSpace)

				results.emplace_back(new PiObject::Volume( VolumeParser(header, volumeBody, buffer, PROJ_DEFAULT_EMPTY ) ));
			}

			void parseVolumes(PiObject::object_vec_t& results, const Finders::VolumesVec_t& volumes, const MemoryView& buffer)
			{
				MemoryView volumeBody(buffer.begin, buffer.begin);

				for (const auto& header : volumes)
				{	// Check that previous object fill all space before current
					if (volumeBody.end != header.begin) {
						results.emplace_back(new PiObject::FreeSpace(PROJ_DEFAULT_EMPTY, buffer, MemoryView(volumeBody.end, header.begin)));
					}

					volumeBody.begin = header.begin;
					volumeBody.end = header.begin + Pi::Volume::Utils::getSize(header);

					if (buffer.isOutside(volumeBody.end - 1))
						DEBUG_ERROR_MESSAGE
							OffsetView ov(buffer.begin, volumeBody);
							DEBUG_PRINT("\tMessage: Volume exceeds provided buffer.");
							DEBUG_PRINT("\tVolume offset: ", ov.offset);
							DEBUG_PRINT("\tVolume length: ", ov.length);
							DEBUG_PRINT("\tBuffer length: ", buffer.getLength());
							DEBUG_PRINT("\tExceeds by: ", ov.offset + ov.length - buffer.getLength());
						DEBUG_END_MESSAGE_AND_EXIT(ExitCodes::ParseErrorFreeSpace)

					results.emplace_back(new PiObject::Volume( VolumeParser(header, volumeBody, buffer, PROJ_DEFAULT_EMPTY ) ));
				}
				// Check for space after last volume
				if (volumeBody.end != buffer.end) {
					results.emplace_back( new PiObject::FreeSpace(PROJ_DEFAULT_EMPTY, buffer, MemoryView(volumeBody.end, buffer.end)));
				}
			}
		}

		PiObject::object_vec_t FreeSpaceParser(const MemoryView& buffer)
		{
			using namespace FreeSpaceParserNs;

			PiObject::object_vec_t result;
			auto volumes = Finders::VolumeFinder(buffer);

			if (volumes.empty()) { // Volumes not found : add as a free space
				result.emplace_back(new PiObject::FreeSpace(PROJ_DEFAULT_EMPTY, buffer, buffer));
				// Report parsing stats
				DEBUG_INFO_MESSAGE
					DEBUG_PRINT("\tMessage: Free space parsing ended.");
					DEBUG_PRINT("\tNothing found.");
				DEBUG_END_MESSAGE
			} else { // Something is found
				Finders::VolumesVec_t toProcess;
				Finders::VolumesVec_t deferred;
				{	// Check intersections
					auto end = volumes.end();
					auto last = volumes.begin();
					std::advance(last, std::distance(volumes.begin(), volumes.end()) - 1);
					MemoryView thisBody;
					// "volumes" array is sorted by header address, so only previous volume may contain next one
					for (auto iter = volumes.begin(); iter != last; iter = std::next(iter))
					{	// Add "this" volume to processing list
						toProcess.emplace_back(*iter);
						// Setup this MV
						thisBody.begin = iter->begin;
						thisBody.setEnd(iter->begin + Pi::Volume::Utils::getSize(iter->get()));
						bool error = false;
						for (auto iter2 = std::next(iter); iter2 != end; iter2 = std::next(iter2))
						{	// At least the left side must be inside for intersection
							if (!thisBody.isInside(iter2->begin))
							{	// This skips previous volume in outer loop
								iter = std::prev(iter2);
								// At min: iter2 = std::next(iter)
								// Outer loop continues from current "other" volume
								break;
							}
							// Check right side intersection
							if (thisBody.isInside(iter2->begin + Pi::Volume::Utils::getSize(iter2->get()) - 1))
							{	// "other" volume is fully contained in "this" one
								deferred.emplace_back(*iter2);
							} else { // This is deadly intersection report an error
								OffsetView thisOV(buffer.begin, thisBody);
								OffsetView otherOV(buffer.begin, MemoryView(iter2->begin, iter2->begin + Pi::Volume::Utils::getSize(iter2->get()) - 1));
								DEBUG_ERROR_MESSAGE
									DEBUG_PRINT("\tMessage: Intersected volumes found.");
									DEBUG_PRINT("\tThis position: ", std::distance(volumes.begin(), iter) + 1);
									DEBUG_PRINT("\tThis offset: ", thisOV.offset);
									DEBUG_PRINT("\tOther position: ", std::distance(volumes.begin(), iter2) + 1);
									DEBUG_PRINT("\tOther offset: ", otherOV.offset);
								DEBUG_END_MESSAGE
								error = true;
								break;
							}
						}
						if (error) break;
					}
					if (volumes.size() == 1) { 
						// In this case outer loop never triggers insertion
						toProcess.emplace_back(volumes.front());
					}
				}

				// Now toProcess contains only primal volumes : process them first
				// only primal volumes may contain free space between them
				parseVolumes(result, toProcess, buffer);
				// Now parse deferred if them are not parsed yet
				auto deferedProc = 0;
				for (const auto& volume : deferred) {
					if (!Pi::Volume::Utils::isParsedVolume(volume)) {
						parseVolume(result, volume, buffer);
						++deferedProc;
					}
				}

				// Report parsing stats
				DEBUG_INFO_MESSAGE
					DEBUG_PRINT("\tMessage: Free space parsing ended.");
					DEBUG_PRINT("\tTotal volumes found: ", volumes.size());
					DEBUG_PRINT("\tPrimal volumes: ", toProcess.size());
					DEBUG_PRINT("\tDeferred volumes: ", toProcess.size());
					DEBUG_PRINT("\tDeferred volumes processed: ", deferedProc);
					DEBUG_PRINT("\tTotal objects: ", result.size());
				DEBUG_END_MESSAGE
			}

			return result;
		}

		namespace VolumeParserNs
		{

			static const EFI_FV_BLOCK_MAP_ENTRY lastEntry = { 0, 0 };

			inline static Types::length_t calcFileLen(const MemoryView& hdr)
			{
				return Pi::File::Utils::getSizeAuto(reinterpret_cast<Pi::File::Header::const_pointer_t>(hdr.begin));
			}
		}

		PiObject::Volume VolumeParser(const Pi::Volume::Header& volumeView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t)
		{
			using namespace VolumeParserNs;
			PiObject::Volume volume(volumeView, baseBuffer, buffer);
			Finders::FilesVec_t fileViews;

			if (!Pi::Volume::Utils::addParsedVolume(volumeView))
			{
				DEBUG_INFO_MESSAGE
					DEBUG_PRINT("\tMessage: Found already parsed volume.");
					DEBUG_PRINT("\tUID: ", volume.getUid());
				DEBUG_END_MESSAGE
				volume.state |= PiObject::InconsistencyState::VolumeAlreadyParsed;
				return volume;
			}

			Types::memory_t empty = volume->Attributes & EFI_FVB2_ERASE_POLARITY ? '\xFF' : '\x00';

			// Define memory used by full header: Pi volume header + block map
			{	// Size of first entry is already counted in size of header structure itself
				Types::count_t blockEntryCount = 0;
				const EFI_FV_BLOCK_MAP_ENTRY* blocks = volume->BlockMap;

				while (std::memcmp(blocks, &lastEntry, Pi::Volume::BlockMap::structure_size)) {
					++blockEntryCount;
					blocks = ADVANCE_PTR_(blocks, const EFI_FV_BLOCK_MAP_ENTRY*, Pi::Volume::BlockMap::structure_size);
				}
				// Count real size of base volume header: 
				//	sizeof(EFI_FIRMWARE_VOLUME_HEADER) + sizeof(EFI_FV_BLOCK_MAP_ENTRY) * blockEntryCount
				volume.fullNormalHdr.setLength(Pi::Volume::Header::structure_size + Pi::Volume::BlockMap::structure_size * blockEntryCount);
			}

			// Consider memory used by extended header and determine areas of interests
			Types::count_t searchAreaCount = 0;
			// Find extension header and ban file search on it
			if (volume->ExtHeaderOffset)
			{
			 	volume.extendedHdr = volume.normalHdr.begin + volume->ExtHeaderOffset;
				volume.fullExtendedHdr = volume.extendedHdr;
				volume.fullExtendedHdr.setLength(volume.extendedHdr->ExtHeaderSize);

				if (volume.fullExtendedHdr.isInside(volume.fullNormalHdr.end - 1))
					DEBUG_ERROR_MESSAGE
						DEBUG_PRINT("\tMessage: Block map collided with extended header.");
					DEBUG_END_MESSAGE_AND_EXIT(ExitCodes::ParseErrorVolume)

				// If there is no space between header and extended header than there is only one
				// search area, otherwise there is two
				searchAreaCount = volume.fullExtendedHdr.isInside(volume.fullNormalHdr.end) ? 1 : 2;
				// Case when extended header is padded to a not significant value of bytes :
				// No legitimate file can be created in that space
				if (volume.extendedHdr.begin - volume.fullNormalHdr.end < Pi::File::Header::structure_size)
				{	// Create new free space instance
					volume.emplace_back<PiObject::FreeSpace>(empty, volume.memory, MemoryView(volume.fullNormalHdr.end, volume.extendedHdr.begin));
					--searchAreaCount;
				}
			}

			// Find Pi file headers for all areas of interests based on their count
			{
				MemoryView volumeBody;

				switch (searchAreaCount)
				{
					case 0 : // No extended header found
						volumeBody.begin = ALIGN_PTR8(volume.fullNormalHdr.begin, volume.fullNormalHdr.end);
						volumeBody.setEnd(volume.memory.end);
						fileViews = Finders::FileFinder(volumeBody, empty);
						break;
					case 1 : // Extended header is right after block map or space between is insignificant
						volumeBody.begin = ALIGN_PTR8(volume.fullNormalHdr.begin, volume.fullExtendedHdr.end);
						volumeBody.setEnd(volume.memory.end);
						fileViews = Finders::FileFinder(volumeBody, empty);
						break;
					case 2 : // Both areas: between block map and extended header and after extended header are significant
						volumeBody.begin = ALIGN_PTR8(volume.fullNormalHdr.begin, volume.fullNormalHdr.end);
						volumeBody.setEnd(volume.extendedHdr.begin);

						// Find all files in first area
						fileViews = Finders::FileFinder(volumeBody, empty);
						if (fileViews.empty()) { // No files found: add area to empty space
							volume.emplace_back<PiObject::FreeSpace>(empty, volume.memory, MemoryView(volume.fullNormalHdr.end, volume.extendedHdr.begin));
						} else { // Some files found: process them to file objects and free space
							processHeaders< PiObject::File, calcFileLen >(
								FileParser,
								volume, fileViews, volumeBody, empty, 
								"\tMessage: File exceeds volume boundary.", ExitCodes::ParseErrorVolume
							);
							// processFileHeaders(volume, fileViews, volumeBody);
							fileViews.clear();
						}

						volumeBody.begin = ALIGN_PTR8(volume.fullNormalHdr.begin, volume.fullExtendedHdr.end);
						volumeBody.setEnd(volume.memory.end);
						// Find files in second area
						fileViews = Finders::FileFinder(volumeBody, empty);
						break;
					default:
						DEBUG_ERROR_MESSAGE
							DEBUG_PRINT("\tMessage: Unexpected count of volume search areas.");
						DEBUG_END_MESSAGE_AND_EXIT(ExitCodes::ParseErrorVolume)
						break;
				}

				// Process found files to file objects and free space 
				if (!fileViews.empty()) {
					processHeaders< PiObject::File, calcFileLen >(
						FileParser,
						volume, fileViews, volumeBody, empty, 
						"\tMessage: File exceeds volume boundary.", ExitCodes::ParseErrorVolume
					);
					//processFileHeaders(volume, fileViews, volumeBody);
				} else { // Add whole volume body as free space
					DEBUG_WARNING_MESSAGE
						DEBUG_PRINT("\tMessage: No files found in volume.");
					DEBUG_END_MESSAGE
					volume.emplace_back<PiObject::FreeSpace>(empty, volume.memory, volumeBody);
				}
			}

			// Return filled volume object
			return volume;
		}

		namespace FileParserNs
		{
			inline static PiObject::File fileByHeader(const Pi::File::Header& fileView, const MemoryView& buffer, const MemoryView& baseBuffer)
			{
				using namespace ::Project::PiObject::helper;

				if (fileView->Attributes & FFS_ATTRIB_LARGE_FILE) {
					return PiObject::File(FileHeader::Extended, fileView, baseBuffer, buffer);
				}
				return PiObject::File(FileHeader::Simple , fileView, baseBuffer, buffer);
			}

			inline static Types::length_t calcSecLen(const MemoryView& hdr)
			{
				return Pi::Section::Utils::getSizeAuto(reinterpret_cast<Pi::Section::Header::const_pointer_t>(hdr.begin));
			}
		}

		PiObject::File FileParser(const Pi::File::Header& fileView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty)
		{
			using namespace FileParserNs;

			auto file = fileByHeader(fileView, buffer, baseBuffer);
			MemoryView fileBody;

			if (file.header.isExtended()) {
				fileBody.begin = file.header.header.begin + Pi::File::Extended::Header::structure_size;
			} else {
				fileBody.begin = file.header.header.end;
			}
			fileBody.setEnd(buffer.end);

			if (Pi::File::Utils::isZeroLengthFile(fileView)) {
				// nothing to do here
				return file;
			} else if (fileBody.getLength() == 0) {
				DEBUG_INFO_MESSAGE
					DEBUG_PRINT("\tMessage: File without body found.");
					DEBUG_PRINT("\tGUID: ", file->Name);
				DEBUG_END_MESSAGE
			} else if ( Pi::File::Utils::isSectionedFileType(file->Type) ) {
				// fileView.begin is aligned by 8 to FV header -> we can align by 4 from it <=> align by 4 from FV header
				fileBody.begin = ALIGN_PTR4(fileView.begin, fileBody.begin);
				// Find all PI section headers in body of file
				auto sections = Finders::SectionFinder(fileBody);
				if (sections.empty())
				{	// It is sectioned type by UEFI PI : report a warning
					DEBUG_WARNING_MESSAGE
						DEBUG_PRINT("\tMessage: No section found in sectioned file type.");
						DEBUG_PRINT("\tGUID: ", file->Name);
						DEBUG_PRINT("\tFile type: ", Pi::File::Utils::fileTypeToCStr(file->Type));
					DEBUG_END_MESSAGE
				} else { // Run section parsing routine
					processHeaders< PiObject::Section, calcSecLen >(
						SectionParser,
						file, sections, fileBody, empty,
						"\tMessage: Section exceeds file boundary.", ExitCodes::ParseErrorFile
					);
				}
			} else if (file->Type == EFI_FV_FILETYPE_RAW) {
				// Raw file types may contain other volumes
				auto parseResult = FreeSpaceParser(fileBody);
				// Move found objects
				file.objects.insert(
					file.objects.end(),
					std::make_move_iterator(parseResult.begin()),
					std::make_move_iterator(parseResult.end())
				);
			}

			return file;
		}

		namespace SectionParserNs
		{

			static PiObject::Section sectionByHeader(const Pi::Section::Header& sectionView, const MemoryView& buffer, const MemoryView& baseBuffer)
			{
				using namespace PiObject::helper;
				PiObject::Section result(sectionView, buffer, baseBuffer);
				// Set section header type
				result.header.headerType = Pi::Section::Utils::getSize(sectionView) == PROJ_SECTION_MAX_SIZE ? SectionHeader::Extended : SectionHeader::Simple;
				// Set section type
				switch (sectionView->Type)
				{
					case EFI_SECTION_COMPRESSION :              result.header.sectionType = SectionHeader::Compression;         break;
					case EFI_SECTION_GUID_DEFINED :             result.header.sectionType = SectionHeader::GuidDefined;         break;
					case EFI_SECTION_DISPOSABLE :               result.header.sectionType = SectionHeader::Disposable;          break;
					case EFI_SECTION_PE32 :                     result.header.sectionType = SectionHeader::Pe32;                break;
					case EFI_SECTION_PIC :                      result.header.sectionType = SectionHeader::Pic;                 break;
					case EFI_SECTION_TE :                       result.header.sectionType = SectionHeader::Te;                  break;
					case EFI_SECTION_DXE_DEPEX :                result.header.sectionType = SectionHeader::DxeDepex;            break;
					case EFI_SECTION_VERSION :                  result.header.sectionType = SectionHeader::Version;             break;
					case EFI_SECTION_USER_INTERFACE :           result.header.sectionType = SectionHeader::UserInterface;       break;
					case EFI_SECTION_COMPATIBILITY16 :          result.header.sectionType = SectionHeader::Compatibility16;     break;
					case EFI_SECTION_FIRMWARE_VOLUME_IMAGE :    result.header.sectionType = SectionHeader::FirmwareVolumeImage; break;
					case EFI_SECTION_FREEFORM_SUBTYPE_GUID :    result.header.sectionType = SectionHeader::FreeformSubtypeGuid; break;
					case EFI_SECTION_PEI_DEPEX :                result.header.sectionType = SectionHeader::PeiDepex;            break;
					case EFI_SECTION_SMM_DEPEX :                result.header.sectionType = SectionHeader::SmmDepex;            break;
					case SCT_SECTION_POSTCODE :                 result.header.sectionType = SectionHeader::PostcodeSct;         break;
					case INSYDE_SECTION_POSTCODE :              result.header.sectionType = SectionHeader::PostcodeInsyde;      break;
					case EFI_SECTION_RAW :
					default :                                   result.header.sectionType = SectionHeader::Raw;                 break;
				}

				return result;
			}

			inline static Types::length_t calcVolLen(const MemoryView& hdr)
			{
				return Pi::Volume::Utils::getSize(reinterpret_cast<Pi::Volume::Header::const_pointer_t>(hdr.begin));
			}

			static EFI_STATUS decompress(
				Decompression::Decompresser::Decompresser_t decompType, 
				Types::unique_byte_buff_t& source, Types::length_t length, MemoryView& result,
				PiObject::Section& section, PiObject::Section::decomp_data_storage_ptr_t storage,
				const char* compressionTypeName
			) 
			{
				auto decomp = Decompression::AllocDecompresser(decompType);
				Types::length_t dstSize = 0;
				auto err = decomp->GetInfo(source.get(), length, dstSize);
				if (EFI_SUCCESS != err) DEBUG_WARNING_MESSAGE
					DEBUG_PRINT("\tMessage: Error during request for compressed data information.");
					DEBUG_PRINT("\tCompression type: ", compressionTypeName);
					DEBUG_PRINT("\tSection UID: ", section.getUid());
					DEBUG_PRINT("\tError code: ", err);
				DEBUG_END_MESSAGE_AND_EVAL({ return err; })

				Types::unique_byte_buff_t decomp_data;

				err = decomp->Decompress(source.get(), length, decomp_data);
				if (EFI_SUCCESS != err) DEBUG_WARNING_MESSAGE
					DEBUG_PRINT("\tMessage: Error occurred during decompression.");
					DEBUG_PRINT("\tCompression type: ", compressionTypeName);
					DEBUG_PRINT("\tSection UID: ", section.getUid());
					DEBUG_PRINT("\tError code: ", err);
				DEBUG_END_MESSAGE_AND_EVAL({ return err; })

				// Store decompressed data in global storage
				auto store_result = storage->emplace(std::piecewise_construct,
					std::forward_as_tuple(
						section.getUid()
					),
					std::forward_as_tuple(
						decompType,
						dstSize,
						std::move(decomp_data)
					)
				);

				if (!store_result.second) DEBUG_WARNING_MESSAGE
					DEBUG_PRINT("\tMessage: Can't store decompressed data in global storage.");
					DEBUG_PRINT("\tSection UID: ", section.getUid());
				DEBUG_END_MESSAGE_AND_EVAL({ return EFI_OUT_OF_RESOURCES; })

				result = store_result.first->second->memory;
				return EFI_SUCCESS;
			}
		}

		PiObject::Section SectionParser(const Pi::Section::Header& sectionView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty)
		{
			using namespace SectionParserNs;

			auto sectionObj = sectionByHeader(sectionView, buffer, baseBuffer);
			Finders::SectionsVec_t sections;

			// Handle section based on it's type
			{
				MemoryView sectionBody(sectionView.begin + Pi::Section::Utils::getFullSize(sectionView), buffer.end);
				// If section is an encapsulation one => parse it according to it's type 
				if (Pi::Section::Utils::isEncapsulationType(sectionView->Type)) {
					switch (sectionView->Type)
					{
						case EFI_SECTION_COMPRESSION :
						{

						} break;

						case EFI_SECTION_GUID_DEFINED :
						{	// Check known guided sections
							// Obtain section field values
							EFI_GUID section_guid;
							UINT16   section_data_offset = 0;
							UINT16   section_attributes = 0;

							if (Pi::Section::Utils::isExtendedSection(sectionView)) {
								Pi::Section::Extended::GuidDefined sv(sectionView.begin);
								section_guid = sv->SectionDefinitionGuid;
								section_data_offset = sv->DataOffset;
								section_attributes = sv->Attributes;
							} else {
								Pi::Section::GuidDefined sv(sectionView.begin);
								section_guid = sv->SectionDefinitionGuid;
								section_data_offset = sv->DataOffset;
								section_attributes = sv->Attributes;
							}

							if (sectionBody.isOutside(sectionView.begin + section_data_offset)) DEBUG_WARNING_MESSAGE
								DEBUG_PRINT("\tMessage: Found GUID defined section which body is outside of provided buffer.");
								DEBUG_PRINT("\tSection UID: ", sectionObj.getUid());
								DEBUG_PRINT("\tSection data offset: ", section_data_offset);
								DEBUG_PRINT("\tProvided buffer length: ", buffer.getLength());
							DEBUG_END_MESSAGE_AND_EVAL({ return sectionObj; });

							// Obtain pointer to global decompressed data storage
							auto decomp_data_storage = sectionObj.getDecomressedDataStorage();
							if (!decomp_data_storage) DEBUG_WARNING_MESSAGE
								DEBUG_PRINT("\tMessage: Can't access decompressed data storage.");
								DEBUG_PRINT("\tSection UID: ", sectionObj.getUid());
							DEBUG_END_MESSAGE_AND_EVAL({ return sectionObj; })

							sectionBody.begin = sectionView.begin + section_data_offset;
							// Decompresser works with non constant memory
							auto tmp = sectionBody.memcpy();

							if (Guid::NamedGuids::whatNamedGuidRange(section_guid) == Guid::NamedGuids::NamedGuidsRanges::FirmwareSection)
							{	// EFI_GUID SOME_GUID_NAME works on C++11 braced initialization
								MemoryView toProcess;
								bool processBody = false;
								const char* message = nullptr;

								if (section_guid == EFI_GUID LZMA_CUSTOM_DECOMPRESS_GUID)
								{
									auto err = decompress(
										Decompression::Decompresser::Lzma,
										tmp,
										sectionBody.getLength(),
										toProcess,
										sectionObj,
										decomp_data_storage,
										"LZMAF86"
									);

									if (err == EFI_SUCCESS)
									{
										message = "\tMessage: Section exceeds decompressed section boundary.";
										processBody = true;
									}
								}
								else if (section_guid == EFI_GUID LZMAF86_CUSTOM_DECOMPRESS_GUID) 
								{
									auto err = decompress(
										Decompression::Decompresser::Lzma86,
										tmp,
										sectionBody.getLength(),
										toProcess,
										sectionObj,
										decomp_data_storage,
										"LZMAF86"
									);

									if (err == EFI_SUCCESS)
									{
										message = "\tMessage: Section exceeds decompressed section boundary.";
										processBody = true;
									}
								} 
								else if (section_guid == EFI_GUID BROTLI_CUSTOM_DECOMPRESS_GUID) 
								{
									auto err = decompress(
										Decompression::Decompresser::Brotli,
										tmp,
										sectionBody.getLength(),
										toProcess,
										sectionObj,
										decomp_data_storage,
										"BROTLI"
									);

									if (err == EFI_SUCCESS)
									{
										message = "\tMessage: Section exceeds decompressed section boundary.";
										processBody = true;
									}
								} 
								else if (section_guid == EFI_GUID TIANO_CUSTOM_DECOMPRESS_GUID) 
								{
									auto err = decompress(
										Decompression::Decompresser::TianoEdk2,
										tmp,
										sectionBody.getLength(),
										toProcess,
										sectionObj,
										decomp_data_storage,
										"TIANO"
									);

									if (err != EFI_SUCCESS)
									{	// May be compressed with EFI1.1
										err = decompress(
											Decompression::Decompresser::TianoEfi2,
											tmp,
											sectionBody.getLength(),
											toProcess,
											sectionObj,
											decomp_data_storage,
											"EFI1.1"
										);
									}

									if (err == EFI_SUCCESS)
									{
										message = "\tMessage: Section exceeds decompressed section boundary.";
										processBody = true;
									}
								} 
								else if (section_guid == EFI_GUID EFI_CRC32_GUIDED_SECTION_EXTRACTION_GUID) 
								{	// This section body is obtained as plain not compressed data
									// This implementation is copy of : 
									//     https://github.com/tianocore/edk2/blob/master/MdeModulePkg/Library/PeiCrc32GuidedSectionExtractLib/PeiCrc32GuidedSectionExtractLib.c
									// For DXE sections algorithm is the same : 
									//    https://github.com/tianocore/edk2/blob/master/MdeModulePkg/Library/DxeCrc32GuidedSectionExtractLib/DxeCrc32GuidedSectionExtractLib.c
									auto dataSize = Pi::Section::Utils::getSize(sectionObj) - section_data_offset;
									auto checksum = Pi::Section::Utils::isExtendedSection(sectionView) ? Pi::Section::Extended::GuidedCrc32(sectionView.begin)->CRC32Checksum : Pi::Section::GuidedCrc32(sectionView.begin)->CRC32Checksum;
									toProcess.begin = sectionView.begin + section_data_offset;
									toProcess.setEnd(buffer.end);
									auto realChecksum = Checksums::crc32(toProcess, dataSize);
									if (checksum != realChecksum) DEBUG_INFO_MESSAGE
										DEBUG_PRINT("\tMessage: CRC32 GUIDed section have invalid checksum.");
										DEBUG_PRINT("\tExpected checksum: ", checksum);
										DEBUG_PRINT("\tReal checksum: ", realChecksum);
									DEBUG_END_MESSAGE

									message = "\tMessage: Section exceeds section boundary.";
									processBody = true;
								} 
								else if (section_guid == EFI_GUID EFI_CERT_TYPE_RSA2048_SHA256_GUID) 
								{	// This section body is obtained as plain not compressed data
									// This implementation is copy of : 
									//    https://github.com/tianocore/edk2/blob/master/SecurityPkg/Library/PeiRsa2048Sha256GuidedSectionExtractLib/PeiRsa2048Sha256GuidedSectionExtractLib.c
									// For DXE sections algorithm is the same : 
									//    https://github.com/tianocore/edk2/blob/master/SecurityPkg/Library/DxeRsa2048Sha256GuidedSectionExtractLib/DxeRsa2048Sha256GuidedSectionExtractLib.c
									auto dataSize = Pi::Section::Utils::getSize(sectionObj);
									auto secHeaderSize = Pi::Section::Utils::isExtendedSection(sectionView) ? Pi::Section::Extended::GuidedSha256::structure_size : Pi::Section::GuidedSha256::structure_size;
									if (dataSize < secHeaderSize || buffer.isOutside(sectionView.begin + secHeaderSize)) DEBUG_ERROR_MESSAGE
										DEBUG_PRINT("\tMessage: SHA256 GUIDed section have invalid size.");
										DEBUG_PRINT("\tExpected size: at least ", secHeaderSize);
										DEBUG_PRINT("\tReal size: ", dataSize);
									DEBUG_END_MESSAGE_AND_EVAL({ return sectionObj; })
									toProcess.begin = sectionView.begin + secHeaderSize; // section_data_offset is ignored by EDK2 implementation
									toProcess.setEnd(buffer.end);
									// Check section content and print all messages
									Pi::Section::Utils::checkRsa2048Sha256GuidedSection(sectionView, buffer);
									message = "\tMessage: Section exceeds section boundary.";
									processBody = true;
								} 
								else if (section_guid == EFI_GUID EFI_FIRMWARE_CONTENTS_SIGNED_GUID) 
								{

								} else {
									
								}

								if (processBody)
								{   // This section may contain other section in it's data
									sections = Finders::SectionFinder(toProcess);
									if (!sections.empty()) {
										processHeaders< PiObject::Section, FileParserNs::calcSecLen >(
											SectionParser,
											sectionObj, sections, toProcess, empty,
											message, ExitCodes::ParseErrorSection
										);
									}
								}
							}

						} break; 

						case EFI_SECTION_DISPOSABLE :
						{	// This section may contain trash data or other sections => try to find other sections in it
							// sectionView.begin is aligned to 4 byte boundary from FV header
							sectionBody.begin = ALIGN_PTR4(sectionView.begin, sectionBody.begin);
							// Find sections
							sections = Finders::SectionFinder(sectionBody);
							if (!sections.empty()) {
								processHeaders< PiObject::Section, FileParserNs::calcSecLen >(
									SectionParser,
									sectionObj, sections, sectionBody, empty,
									"\tMessage: Section exceeds section boundary.", ExitCodes::ParseErrorSection
								);
							}
						} break;

						default:
						{
							DEBUG_WARNING_MESSAGE
								DEBUG_PRINT("\tMessage: No handler added to this encapsulation section type.");
								DEBUG_PRINT("\tSection type: ", sectionView->Type);
							DEBUG_END_MESSAGE
						} break;
					}
				} else if (sectionView->Type == EFI_SECTION_FIRMWARE_VOLUME_IMAGE) {
					// This section must contain volume
					auto volumeVec = Finders::VolumeFinder(sectionBody);
					if (volumeVec.empty()) {
						DEBUG_WARNING_MESSAGE
							DEBUG_PRINT("\tMessage: Firmware Volume not found in volume image section.");
							DEBUG_PRINT("\tUID: ", sectionObj.getUid());
						DEBUG_END_MESSAGE
					} else {
						processHeaders< PiObject::Volume, calcVolLen >(
							VolumeParser,
							sectionObj, volumeVec, sectionBody, empty,
							"\tMessage: Volume exceeds section boundary.", ExitCodes::ParseErrorSection
						);
					}
				} else if (sectionView->Type == EFI_SECTION_RAW) {
					// Parse as if it is a new image
					auto foundEntries = FreeSpaceParser(sectionBody);
					if (!foundEntries.empty()) {
						// Move found entries to child array
						sectionObj.objects.insert(
							sectionObj.objects.end(),
							std::make_move_iterator(foundEntries.begin()),
							std::make_move_iterator(foundEntries.end())
						);
					}
				}
				// No other section type must be handled in a special way
			}

			return sectionObj;
		}

	}

}