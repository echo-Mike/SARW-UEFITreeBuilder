#pragma once
#ifndef PI_FILE_UTILS_HPP__
#define PI_FILE_UTILS_HPP__ "0.0.0@PiFileUtils.hpp"

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

				inline Types::length_t getSize(const EFI_FFS_FILE_HEADER* header)
				{
					return  (static_cast<Types::length_t>(header->Size[2]) << 16) +
							(static_cast<Types::length_t>(header->Size[1]) << 8) +
							 static_cast<Types::length_t>(header->Size[0]);
				}
				
				inline Types::length_t getSize(const EFI_FFS_FILE_HEADER2* header)
				{
					return header->ExtendedSize;
				}
				
				inline Types::length_t getSize(const PiObject::File& file)
				{
					return file.header.isExtended() ? getSize(file.header.extended) : getSize(file.header.header);
				}

				inline Types::length_t getSize1(Types::const_pointer_t header)
				{
					return getSize(reinterpret_cast<const EFI_FFS_FILE_HEADER*>(header));
				}

				inline Types::length_t getSize2(Types::const_pointer_t header)
				{
					return reinterpret_cast<const EFI_FFS_FILE_HEADER2*>(header)->ExtendedSize;
				}

				inline Types::length_t getSizeAuto(const EFI_FFS_FILE_HEADER* header)
				{
					return header->Attributes & FFS_ATTRIB_LARGE_FILE ? getSize2(UnifyPtrCast(header)) : getSize(header);
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

				FileTypeRanges::FileTypeRange_t whatTypeRange(Types::memory_t type);

				bool isFileType(Types::memory_t type) { return whatTypeRange(type) != FileTypeRanges::NotAFileType; }

				bool isSectionedFileType(Types::memory_t type);

				const char* fileTypeToCStr(Types::memory_t type);

				inline std::string fileTypeToStr(Types::memory_t type) { return fileTypeToCStr(type); }

			}

		}

	}

}

#endif
