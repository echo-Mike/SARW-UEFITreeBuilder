#pragma once
#ifndef PI_FILE_UTILS_HPP__
#define PI_FILE_UTILS_HPP__ "0.0.0@PiFileUtils.hpp"

/// STD
#include <string>
/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"
#include "PiFileObject.hpp"

// Not defined if UEFI headers but defined in specification
#ifndef EFI_FV_FILETYPE_SMM_STANDALONE
#	define EFI_FV_FILETYPE_SMM_STANDALONE 0x0E
#endif
#ifndef EFI_FV_FILETYPE_SMM_CORE_STANDALONE
#	define EFI_FV_FILETYPE_SMM_CORE_STANDALONE 0x0F
#endif

// Mask for zero bit in file attributes
#define FFS_ATTRIB_SET_TO_ZERO 0x80

#ifndef FFS_ATTRIB_DATA_ALIGNMENT2
#	define FFS_ATTRIB_DATA_ALIGNMENT2 0x02
#endif

#ifndef PROJ_CUSTOM_FILE_DATA_ALIGNMENT
#	define FFS_FILE_DATA_ALIGNMENT_1    0x00
#	define FFS_FILE_DATA_ALIGNMENT_16   0x08
#	define FFS_FILE_DATA_ALIGNMENT_128  0x10
#	define FFS_FILE_DATA_ALIGNMENT_512  0x18
#	define FFS_FILE_DATA_ALIGNMENT_1K   0x20
#	define FFS_FILE_DATA_ALIGNMENT_4K   0x28
#	define FFS_FILE_DATA_ALIGNMENT_32K  0x30
#	define FFS_FILE_DATA_ALIGNMENT_64K  0x38
#	define FFS_FILE_DATA_ALIGNMENT_128K 0x00
#	define FFS_FILE_DATA_ALIGNMENT_256K 0x08
#	define FFS_FILE_DATA_ALIGNMENT_512K 0x10
#	define FFS_FILE_DATA_ALIGNMENT_1M   0x18
#	define FFS_FILE_DATA_ALIGNMENT_2M   0x20
#	define FFS_FILE_DATA_ALIGNMENT_4M   0x28
#	define FFS_FILE_DATA_ALIGNMENT_8M   0x30
#	define FFS_FILE_DATA_ALIGNMENT_16M  0x38
#endif

// Mask for bits that must be in EFI_FVB_ERASE_POLARITY 
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

	namespace Pi
	{

		namespace File
		{

			namespace Utils
			{

				inline Types::length_t getSize(const Pi::File::Header& header)
				{
					return  (static_cast<Types::length_t>(header->Size[2]) << 16) +
							(static_cast<Types::length_t>(header->Size[1]) << 8) +
							 static_cast<Types::length_t>(header->Size[0]);
				}

				inline Types::length_t getSize(const Pi::File::Extended::Header& header)
				{
					return header->ExtendedSize;
				}

				inline Types::length_t getSize(Pi::File::Header::const_pointer_t header)
				{
					return  (static_cast<Types::length_t>(header->Size[2]) << 16) +
							(static_cast<Types::length_t>(header->Size[1]) << 8) +
							 static_cast<Types::length_t>(header->Size[0]);
				}
				
				inline Types::length_t getSize(Pi::File::Extended::Header::const_pointer_t header)
				{
					return header->ExtendedSize;
				}

				inline Types::length_t getSize1(Types::const_pointer_t header)
				{
					return getSize(reinterpret_cast<const EFI_FFS_FILE_HEADER*>(header));
				}

				inline Types::length_t getSize2(Types::const_pointer_t header)
				{
					return reinterpret_cast<const EFI_FFS_FILE_HEADER2*>(header)->ExtendedSize;
				}
				
				inline Types::length_t getSize(const PiObject::File& file)
				{
					return file.header.isExtended() ? getSize2(UnifyPtrCast(file.header.header.get())) : getSize1(UnifyPtrCast(file.header.header.get()));
				}

				inline Types::length_t getSizeAuto(Pi::File::Header::const_pointer_t header)
				{
					return header->Attributes & FFS_ATTRIB_LARGE_FILE ? getSize2(UnifyPtrCast(header)) : getSize(header);
				}

				inline Types::length_t getHeaderSize(Pi::File::Header::const_pointer_t header)
				{
					return header->Attributes & FFS_ATTRIB_LARGE_FILE ? Pi::File::Extended::Header::structure_size : Pi::File::Header::structure_size;
				}

				namespace FileTypeRanges
				{
					enum FileTypeRange_t : int
					{
						NotAFileType,
						Normal,
						OEM,
						Debug,
						FFS
					};
				}

				FileTypeRanges::FileTypeRange_t whatTypeRange(Types::byte_t type);

				inline bool isZeroLengthFile(Pi::File::Header::const_pointer_t header)
				{
					auto length = getSizeAuto(header);
					return length == (header->Attributes & FFS_ATTRIB_LARGE_FILE ? Pi::File::Extended::Header::structure_size : Pi::File::Header::structure_size);
				}

				bool isFileType(Types::byte_t type) { return whatTypeRange(type) != FileTypeRanges::NotAFileType; }

				bool isSectionedFileType(Types::byte_t type);

				const char* fileTypeToCStr(Types::byte_t type);

				inline std::string fileTypeToStr(Types::byte_t type) { return fileTypeToCStr(type); }

				std::string getAtributeString(Pi::File::Header::const_pointer_t header, bool alternative = false);

				std::string getStateString(Pi::File::Header::const_pointer_t header, bool alternative = false);

			}

		}

	}

}

#endif
