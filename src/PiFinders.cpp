/// STD
#include <cstring>
#include <algorithm>
#include <utility>

/// PROJECT
#include "PiFinders.hpp"
#include "vFFSGuids.hpp"

// Not defined if UEFI headers but defined in specification
#define EFI_FV_FILETYPE_SMM_STANDALONE 0x0E
#define EFI_FV_FILETYPE_SMM_CORE_STANDALONE 0x0F

#define FFS_ATTRIB_SET_TO_ZERO 0x80

#define EFI_FILE_STATE_MASK (~(static_cast<UINT8>( \
	EFI_FILE_HEADER_CONSTRUCTION | \
	EFI_FILE_HEADER_VALID | \
	EFI_FILE_DATA_VALID | \
	EFI_FILE_MARKED_FOR_UPDATE | \
	EFI_FILE_DELETED | \
	EFI_FILE_HEADER_INVALID \
)))

namespace Project
{
	namespace Finders
	{
		
		namespace VolumeFinderNs
		{
			typedef std::pair<
				Types::length_t, // Header offset
				Pi::Volume::Header::const_pointer_t // Pointer to first byte of structure
			> FVHeaderData;

			typedef std::vector< FVHeaderData > HeadersVec_t;

			static const std::size_t VolumeSignatureOffset = offsetof(Pi::Volume::Header::value_type, Signature);
		}

		VolumesVec_t VolumeFinder::operator()(const MemoryView& buffer)
		{
			using namespace VolumeFinderNs;

			VolumesVec_t result;
			HeadersVec_t headers;
			Types::const_pointer_t current = buffer.begin;

			// Find all occurrence of Firmware volume signature EFI_FVH_SIGNATURE
			while ( ( current = std::search( current,  buffer.end, VolumeSignature.bytes, VolumeSignature.bytes + sizeof(EFI_FVH_SIGNATURE)) ) != buffer.end )
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
						if (headerPtr->HeaderLength < Pi::Volume::Header::structure_size) 
							return true;
						// 2) Check volume length sanity clue
						if (headerPtr->FvLength > buffer.getLength())
							return true;
						// 3) Check checksum clue
						{
							Pi::Volume::Header::value_type header_copy = *headerPtr;
							// Zero current checksum
							header_copy.Checksum = 0;
							// Calculate checksum
							auto checkSum = Project::Checksums::calc16( 
								reinterpret_cast<Types::const_pointer_t>(&header_copy), 
								Pi::Volume::Header::structure_size
							);
							if (headerPtr->Checksum != checkSum) {
								return true;
							}
						}
						// 4) Check GUID match clue
						if ( FfsGuids::isValidFfsGuid(&headerPtr->FileSystemGuid).code == FfsGuids::KnownFfsGuids::Unknown )
							return true;
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
			std::for_each(
				headers.begin(),
				headers.end(),
				[&result](const FVHeaderData& a) { result.emplace_back(a.second); }
			);

			DEBUG_INFO_MESSAGE
				DEBUG_PRINT("\tMessage: ", result.size() ," firmware volumes found.");
				int counter = 0;
				for (const auto& header : result) 
				{
					DEBUG_PRINT("\t\t", counter, ") GUID: ", FfsGuids::isValidFfsGuid(&header->FileSystemGuid).name);
					++counter;
				}
			DEBUG_END_MESSAGE

			return result;
		}

		namespace FileFinderNs
		{

			static const Types::memory_t FileTypes_[] =
			{
				// EFI_FV_FILETYPE_ALL, // Not used in real files: marker for function calls
				EFI_FV_FILETYPE_RAW,
				EFI_FV_FILETYPE_FREEFORM,
				EFI_FV_FILETYPE_SECURITY_CORE,
				EFI_FV_FILETYPE_PEI_CORE,
				EFI_FV_FILETYPE_DXE_CORE,
				EFI_FV_FILETYPE_PEIM,
				EFI_FV_FILETYPE_DRIVER,
				EFI_FV_FILETYPE_COMBINED_PEIM_DRIVER,
				EFI_FV_FILETYPE_APPLICATION,
				EFI_FV_FILETYPE_SMM,
				EFI_FV_FILETYPE_FIRMWARE_VOLUME_IMAGE,
				EFI_FV_FILETYPE_COMBINED_SMM_DXE,
				EFI_FV_FILETYPE_SMM_CORE,
				EFI_FV_FILETYPE_SMM_STANDALONE,
				EFI_FV_FILETYPE_SMM_CORE_STANDALONE,
				EFI_FV_FILETYPE_FFS_PAD
			};

			struct FileTypeFiller
			{

				static std::size_t getTypesCount() { return sizeof(FileTypes) / sizeof(Types::memory_t); }

				static Types::const_pointer_t begin()
				{
					static const FileTypeFiller instance;
					return FileTypes;
				}

				static Types::const_pointer_t end() { return FileTypes + getTypesCount(); }

			private:

				static Types::memory_t FileTypes[
					sizeof(FileTypes_) / sizeof(Types::memory_t) +
						EFI_FV_FILETYPE_OEM_MAX - EFI_FV_FILETYPE_OEM_MIN + 1 +
						EFI_FV_FILETYPE_DEBUG_MAX - EFI_FV_FILETYPE_DEBUG_MIN + 1 +
						EFI_FV_FILETYPE_FFS_MAX - EFI_FV_FILETYPE_FFS_MIN + 1
				];

				FileTypeFiller()
				{
					Types::pointer_t OemPtr = FileTypes + sizeof(FileTypes_) / sizeof(Types::memory_t);
					Types::pointer_t DebugPtr = OemPtr + EFI_FV_FILETYPE_OEM_MAX - EFI_FV_FILETYPE_OEM_MIN + 1;
					Types::pointer_t FfsPtr = DebugPtr + EFI_FV_FILETYPE_DEBUG_MAX - EFI_FV_FILETYPE_DEBUG_MIN + 1;
					Types::pointer_t EndPtr = FileTypes + sizeof(FileTypes) / sizeof(Types::memory_t);

					std::memcpy(FileTypes, FileTypes_, sizeof(FileTypes_) * sizeof(Types::memory_t));
					std::iota(OemPtr, DebugPtr, EFI_FV_FILETYPE_OEM_MIN);
					std::iota(DebugPtr, FfsPtr, EFI_FV_FILETYPE_DEBUG_MIN);
					std::iota(FfsPtr, EndPtr, EFI_FV_FILETYPE_FFS_MIN);
				}
			};
		}

		FilesVec_t FileFinder::operator()(const MemoryView& buffer, Types::memory_t empty)
		{
			using namespace FileFinderNs;

			FilesVec_t result;
			EFI_FFS_FILE_HEADER tmpHeader;
			EFI_FFS_FILE_HEADER2 tmpExtHeader;

			// buffer.begin is aligned to 8 byte from FV beginning
			auto header = reinterpret_cast<const EFI_FFS_FILE_HEADER*>(buffer.begin);
			
			// Proper file header rule set
			auto isProperHeader = [&, empty](const EFI_FFS_FILE_HEADER* h) -> bool
			{
				{	// 1) Check file type
					auto b = FileTypeFiller::begin();
					auto e = FileTypeFiller::end();
					if ( std::find(b, e, static_cast<Types::memory_t>(h->Type)) == e ) {
						return false;
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
						return false;
					}
				}
				
				{	// 3) Check attributes
					if ( h->Attributes & FFS_ATTRIB_SET_TO_ZERO ) {
						return false;
					}
				}

				{	// 4) Check size
					if ( !(h->Attributes & FFS_ATTRIB_LARGE_FILE) ) {
						auto fileSize = Pi::File::getSize(h);
						if (fileSize < Pi::File::Header::structure_size ||
							fileSize > buffer.getLength() - (UnifyPtrCast(h) - buffer.begin) ) 
						{
							return false;
						}
					}
				}
				
				{	// 5) Check state
					if ( (h->State ^ (UINT8)empty) & EFI_FILE_STATE_MASK ) {
						return false;
					}
				}

				// 6) Check data alignment : may be performed(UEFITool performs it)

				return true;
			};
			
			// Check and advance loop
			while (UnifyPtrCast(header) + Pi::File::Header::structure_size < buffer.end)
			{
				if (isProperHeader(header))
				{	// File found: save it, advance to size of file and align to 8 bytes boundary
					result.emplace_back( header );
					// Obtain file size based on it's type
					std::size_t toAdvance = header->Attributes & FFS_ATTRIB_LARGE_FILE ? Pi::File::getSize2(UnifyPtrCast(header)) : Pi::File::getSize(header);
					// Advance ptr to specified size
					header = ADVANCE_PTR_(header, const EFI_FFS_FILE_HEADER*, toAdvance);
					// Align it to 8 byte boundary from beginning of buffer
					header = ALIGN_PTR_T8(buffer.begin, header, const EFI_FFS_FILE_HEADER*);
				} else { // File not found: advance to 8 bytes
					header = ADVANCE_PTR8(header, const EFI_FFS_FILE_HEADER*);
				}
			}
			
			return result;
		}

		namespace SectionFinderNs
		{

		}

		SectionsVec_t SectionFinder::operator()(const MemoryView& buffer, Types::memory_t empty)
		{
			using namespace SectionFinderNs;

			SectionsVec_t result;
		}

	}
}