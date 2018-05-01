/// STD
#include <cstring>
#include <algorithm>
#include <utility>
#include <set>

/// PROJECT
#include "PiFinders.hpp"
#include "GuidAdvanced.hpp"
#include "PiFileUtils.hpp"
#include "PiSectionUtils.hpp"

namespace Project
{
	namespace Finders
	{
		
		namespace VolumeFinderNs
		{

			static const signature_cast_32<EFI_FVH_SIGNATURE> VolumeSignature;

			typedef std::pair<
				Types::length_t, // Header offset
				Pi::Volume::Header::const_pointer_t // Pointer to first byte of structure
			> FVHeaderData;

			typedef std::vector< FVHeaderData > HeadersVec_t;

			static const std::size_t VolumeSignatureOffset = offsetof(Pi::Volume::Header::value_type, Signature);
		}

		VolumesVec_t VolumeFinder(const MemoryView& buffer)
		{
			using namespace VolumeFinderNs;

			VolumesVec_t result;
			HeadersVec_t headers;
			Types::const_pointer_t current = buffer.begin;

			// Find all occurrence of Firmware volume signature EFI_FVH_SIGNATURE
			while ( ( current = std::search( current,  buffer.end, VolumeSignature.bytes, VolumeSignature.bytes + VolumeSignature.byte_size) ) != buffer.end )
			{
				headers.emplace_back(
					current - buffer.begin - VolumeSignatureOffset,
					reinterpret_cast<Pi::Volume::Header::const_pointer_t>(current - VolumeSignatureOffset)
				);
				++current;
			}

			// Check found headers
			headers.erase(
				std::remove_if(
					headers.begin(),
					headers.end(),
					[&buffer](const FVHeaderData& value) -> bool
					{
						// Obtain pointer to possible FV header
						auto headerPtr = value.second;
						// 1) Check length clue
						if (headerPtr->HeaderLength < Pi::Volume::Header::structure_size ||
							headerPtr->HeaderLength > buffer.getLength() )
							return true;
						// 2) Check volume length sanity clue
						if (headerPtr->FvLength < Pi::Volume::Header::structure_size ||
							headerPtr->FvLength > buffer.getLength())
							return true;
						// 3) Check checksum clue
						{
							Pi::Volume::Header::value_type header_copy = *headerPtr;
							// Zero current checksum
							header_copy.Checksum = 0;
							// Calculate checksum
							auto checkSum = Project::Checksums::calc16( UnifyPtrCast(&header_copy), Pi::Volume::Header::structure_size );
							if (headerPtr->Checksum != checkSum) {
								return true;
							}
						}
						// 4) Check GUID match clue
						/* // Do not check: image may introduce it's own GUIDs
						if (Guid::FFS::isValidFfsGuid(headerPtr->FileSystemGuid))
							return true;
						//*/
						return false;
					}
				),
				headers.end()
			);

			// Sort headers by offset increment
			std::sort(
				headers.begin(), 
				headers.end(), 
				[](const FVHeaderData& a, const FVHeaderData& b) -> bool 
				{
					return a.first < b.first; 
				}
			);

			// Create view entries
			for (const auto& data : headers) {
				result.emplace_back(data.second);
			}

			DEBUG_INFO_MESSAGE
				DEBUG_PRINT("\tMessage: ", result.size() ," firmware volumes found.");
				int counter = 0;
				for (const auto& header : result) 
				{
					if (Guid::NamedGuids::isNamedGuid(header->FileSystemGuid)) {
						DEBUG_PRINT("\t\t", counter, ") ", Guid::NamedGuids::findNamedGuid(header->FileSystemGuid));
					} else {
						DEBUG_PRINT("\t\t", counter, ") Unnamed GUID: ", header->FileSystemGuid);
					}
					++counter;
				}
			DEBUG_END_MESSAGE

			return result;
		}

		namespace FileHeaderStatus
		{
			enum FileHeaderStatus_t : int
			{
				Normal,
				Repeated,
				NotAHeader
			};
		}

		FilesVec_t FileFinder(const MemoryView& buffer, Types::byte_t empty)
		{
			FilesVec_t result;
			EFI_FFS_FILE_HEADER tmpHeader;
			EFI_FFS_FILE_HEADER2 tmpExtHeader;
			std::set< EFI_GUID > guidCollection;

			// buffer.begin is aligned to 8 byte from FV beginning
			auto header = reinterpret_cast<const EFI_FFS_FILE_HEADER*>(buffer.begin);
			
			// Proper file header rule set
			auto isProperHeader = [&, empty](const EFI_FFS_FILE_HEADER* h) -> FileHeaderStatus::FileHeaderStatus_t
			{
				{	// 1) Check file type
					if ( !Pi::File::Utils::isFileType(h->Type) ) {
						return FileHeaderStatus::NotAHeader;
					}
				}

				{	// 2) Check file integrity
					std::uint8_t checkSum = 0;
					if ( h->Attributes & FFS_ATTRIB_LARGE_FILE ) {
						tmpExtHeader = *reinterpret_cast<const EFI_FFS_FILE_HEADER2*>(h);
						tmpExtHeader.IntegrityCheck.Checksum.File = 0;
						tmpExtHeader.IntegrityCheck.Checksum.Header = 0;
						tmpExtHeader.State = 0;
						checkSum = Checksums::calc8(UnifyPtrCast(&tmpExtHeader), Pi::File::Extended::Header::structure_size);
					} else {
						tmpHeader = *h;
						tmpHeader.IntegrityCheck.Checksum.File = 0;
						tmpHeader.IntegrityCheck.Checksum.Header = 0;
						tmpHeader.State = 0;
						checkSum = Checksums::calc8(UnifyPtrCast(&tmpHeader), Pi::File::Header::structure_size);
					}

					if ( checkSum != h->IntegrityCheck.Checksum.Header ) {
						return FileHeaderStatus::NotAHeader;;
					}
				}
				
				{	// 3) Check attributes
					if ( h->Attributes & FFS_ATTRIB_SET_TO_ZERO ) {
						return FileHeaderStatus::NotAHeader;;
					}
				}

				{	// 4) Check size
					Types::length_t fileSize = 0, struct_size = 0;
					if ( h->Attributes & FFS_ATTRIB_LARGE_FILE ) {
						fileSize = Pi::File::Utils::getSize2(UnifyPtrCast(h));
						struct_size = Pi::File::Extended::Header::structure_size;
					} else {
						fileSize = Pi::File::Utils::getSize(h);
						struct_size = Pi::File::Header::structure_size;
					}

					if (fileSize < struct_size ||
						fileSize > buffer.getLength() - (UnifyPtrCast(h) - buffer.begin))
					{
						return FileHeaderStatus::NotAHeader;;
					}
				}
				
				{	// 5) Check state
					if ( (h->State ^ (UINT8)empty) & EFI_FILE_STATE_MASK ) {
						return FileHeaderStatus::NotAHeader;;
					}
				}

				{	// 6) Check repeated GUID
					if (h->Type != EFI_FV_FILETYPE_FFS_PAD && !guidCollection.count(h->Name)) {
						return FileHeaderStatus::Repeated;
					}
				}

				// 7) Check data alignment : may be performed (UEFITool performs it)

				return FileHeaderStatus::Normal;
			};
			
			// Check and advance loop
			while (UnifyPtrCast(header) + Pi::File::Header::structure_size < buffer.end)
			{
				switch ( isProperHeader(header) )
				{
					case FileHeaderStatus::Normal :
						{	// File found: save it, advance to size of file and align to 8 bytes boundary
							result.emplace_back(header);
							// Add GUID to collection
							guidCollection.insert(header->Name);
							// Obtain file size based on it's type
							std::size_t toAdvance = Pi::File::Utils::getSizeAuto(header);
							// Advance ptr to specified size
							header = ADVANCE_PTR_(header, const EFI_FFS_FILE_HEADER*, toAdvance);
							// Align it to 8 byte boundary from beginning of buffer
							header = ALIGN_PTR_T8(buffer.begin, header, const EFI_FFS_FILE_HEADER*);
						} break;
					case FileHeaderStatus::NotAHeader :
						// File not found: advance to 8 bytes
						header = ADVANCE_PTR8(header, const EFI_FFS_FILE_HEADER*);
						break;
					case FileHeaderStatus::Repeated :
						{	// Report about repeated GUID
							DEBUG_INFO_MESSAGE
								OffsetView ov(buffer.begin, Pi::File::Header(header));
								DEBUG_PRINT("\tMessage: File with repeated GUID found.");
								DEBUG_PRINT("\tFile GUID: ", header->Name);
								DEBUG_PRINT("\tOffset in inspected buffer: ", ov.offset);
							DEBUG_END_MESSAGE
							// Obtain file size based on it's type
							std::size_t toAdvance = Pi::File::Utils::getSizeAuto(header);
							// Advance ptr to specified size
							header = ADVANCE_PTR_(header, const EFI_FFS_FILE_HEADER*, toAdvance);
							// Align it to 8 byte boundary from beginning of buffer
							header = ALIGN_PTR_T8(buffer.begin, header, const EFI_FFS_FILE_HEADER*);
						} break;
					default:
						DEBUG_ERROR_MESSAGE
							DEBUG_PRINT("\tMessage: Unexpected execution path.");
						DEBUG_END_MESSAGE_AND_EXIT(ExitCodes::ParseErrorFile)
						break;
				}
			}
			
			return result;
		}

		SectionsVec_t SectionFinder(const MemoryView& buffer)
		{
			SectionsVec_t result;
			// buffer.begin is aligned to 4 byte from FV beginning
			auto header = reinterpret_cast<const EFI_COMMON_SECTION_HEADER*>(buffer.begin);

			// Proper section header rule set
			auto isProperHeader = [&](const EFI_COMMON_SECTION_HEADER* h) -> bool
			{
				// 1) Check section type
				{
					if (!Pi::Section::Utils::isSectionType(h->Type)) {
						return false;
					}
				}

				// 2) Check section size
				{
					Types::length_t sectionSize = Pi::Section::Utils::getSizeAuto(h), 
									struct_size = Pi::Section::Utils::getHeaderSize(h);
					if (sectionSize < struct_size ||
						sectionSize > buffer.getLength() - (UnifyPtrCast(h) - buffer.begin))
					{
						return false;
					}
				}

				return true;
			};

			// Check and advance loop
			while (UnifyPtrCast(header) + Pi::Section::Header::structure_size < buffer.end)
			{
				if (isProperHeader(header))
				{	// File found: save it, advance to size of section and align to 4 byte boundary
					result.emplace_back(header);
					// Obtain section size based on it's type
					std::size_t toAdvance = Pi::Section::Utils::getSizeAuto(header);
					// Advance ptr to specified size
					header = ADVANCE_PTR_(header, const EFI_COMMON_SECTION_HEADER*, toAdvance);
					// Align it to 4 byte boundary from beginning of buffer
					header = ALIGN_PTR_T4(buffer.begin, header, const EFI_COMMON_SECTION_HEADER*);
				} else {
					// Section not found: advance to 4 bytes
					header = ADVANCE_PTR4(header, const EFI_COMMON_SECTION_HEADER*);
				}
			}
			
			return result;
		}

	}
}