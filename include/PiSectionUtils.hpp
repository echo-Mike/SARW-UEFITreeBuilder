#pragma once
#ifndef PI_SECTION_UTILS_HPP__
#define PI_SECTION_UTILS_HPP__ "0.0.0@PiSectionUtils.hpp"

/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"
#include "PiSectionObject.hpp"

// Max size of EFI_COMMON_SECTION_HEADER : it will become EFI_COMMON_SECTION_HEADER2 with it
#define PROJ_SECTION_MAX_SIZE (0xFFFFFF)

// Not defined if UEFI headers but defined in UEFITool source code
/**
 *	EXTERNAL CODE USAGE:
 *	AUTHOR:
 *		Nikolaj Schlej
 *	PURPOSE:
 *		Defines section types known to be used in some UEFI images.
 *	BEGIN:
 */
#define SCT_SECTION_POSTCODE	0xF0 // Specific to Phoenix SCT images
#define INSYDE_SECTION_POSTCODE	0x20 // Specific to Insyde images
/**
 *	END
 */

// Set structure packing
#pragma pack(push,1)

extern "C" {

// Define section structures that corresponds to SCT_SECTION_POSTCODE and INSYDE_SECTION_POSTCODE

typedef struct {
	EFI_COMMON_SECTION_HEADER CommonHeader;
	UINT32   Postcode;
} POSTCODE_SECTION;

typedef struct {
	EFI_COMMON_SECTION_HEADER2 CommonHeader;
	UINT32   Postcode;
} POSTCODE_SECTION2;

// Special section structure for GUIDed sections with GUID : 
// EFI_CRC32_GUIDED_SECTION_EXTRACTION_GUID

typedef struct {
	EFI_GUID_DEFINED_SECTION  GuidedSectionHeader; ///< EFI guided section header
	UINT32                    CRC32Checksum;       ///< 32bit CRC check sum
} CRC32_SECTION_HEADER;

typedef struct {
	EFI_GUID_DEFINED_SECTION2 GuidedSectionHeader; ///< EFI guided section header
	UINT32                    CRC32Checksum;       ///< 32bit CRC check sum
} CRC32_SECTION2_HEADER;

// Special section structure for GUIDed sections with GUID : 
// EFI_CERT_TYPE_RSA2048_SHA256_GUID

typedef struct {
	EFI_GUID_DEFINED_SECTION        GuidedSectionHeader;     ///< EFI guided section header
	EFI_CERT_BLOCK_RSA_2048_SHA256  CertBlockRsa2048Sha256;  ///< RSA 2048-bit Signature
} RSA_2048_SHA_256_SECTION_HEADER;

typedef struct {
	EFI_GUID_DEFINED_SECTION2       GuidedSectionHeader;     ///< EFI guided section header
	EFI_CERT_BLOCK_RSA_2048_SHA256  CertBlockRsa2048Sha256;  ///< RSA 2048-bit Signature
} RSA_2048_SHA_256_SECTION2_HEADER;

}

// Clear structure packing
#pragma pack(pop)

namespace Project
{

	namespace Pi
	{

		namespace Section
		{

			typedef StructureView<POSTCODE_SECTION, SectionTag> Postcode;
			typedef StructureView<CRC32_SECTION_HEADER, SectionTag> GuidedCrc32;
			typedef StructureView<RSA_2048_SHA_256_SECTION_HEADER, SectionTag> GuidedSha256;

			namespace Extended
			{

				typedef StructureView<POSTCODE_SECTION2, SectionExtTag> Postcode;
				typedef StructureView<CRC32_SECTION2_HEADER, SectionExtTag> GuidedCrc32;
				typedef StructureView<RSA_2048_SHA_256_SECTION2_HEADER, SectionExtTag> GuidedSha256;
			}

			namespace Utils
			{

				inline Types::length_t getSize(const Pi::Section::Header& header)
				{
					return  (static_cast<Types::length_t>(header->Size[2]) << 16) +
							(static_cast<Types::length_t>(header->Size[1]) << 8) +
							 static_cast<Types::length_t>(header->Size[0]);
				}

				inline Types::length_t getSize(const Pi::Section::Extended::Header& header)
				{
					return header->ExtendedSize;
				}

				inline Types::length_t getSize(Pi::Section::Header::const_pointer_t header)
				{
					return  (static_cast<Types::length_t>(header->Size[2]) << 16) +
							(static_cast<Types::length_t>(header->Size[1]) << 8) +
							 static_cast<Types::length_t>(header->Size[0]);
				}

				inline Types::length_t getSize(Pi::Section::Extended::Header::const_pointer_t header)
				{
					return header->ExtendedSize;
				}

				inline Types::length_t getSize(const PiObject::Section& sec)
				{
					return sec.header.isExtended() ? getSize2(sec.header.header.begin) : getSize(sec.header.header);
				}

				inline Types::length_t getSize1(Types::const_pointer_t header)
				{
					return getSize(reinterpret_cast<Pi::Section::Header::const_pointer_t>(header));
				}

				inline Types::length_t getSize2(Types::const_pointer_t header)
				{
					return reinterpret_cast<Pi::Section::Extended::Header::const_pointer_t>(header)->ExtendedSize;
				}

				inline Types::length_t getSizeAuto(Pi::Section::Header::const_pointer_t header)
				{
					return getSize(header) == PROJ_SECTION_MAX_SIZE ? getSize2(UnifyPtrCast(header)) : getSize(header);
				}

				Types::length_t getFullSize(Pi::Section::Header::const_pointer_t header);

				bool isSectionType(Types::memory_t type);

				bool isEncapsulationType(Types::memory_t type);

				inline bool isExtendedSection(Pi::Section::Header::const_pointer_t header)
				{
					return getSize(header) == PROJ_SECTION_MAX_SIZE;
				}

				const char* sectionTypeToCStr(Types::memory_t type);

				inline std::string sectionTypeToStr(Types::memory_t type) { return sectionTypeToCStr(type); }

				bool checkRsa2048Sha256GuidedSection(const Pi::Section::Header& sectionView, const MemoryView& buffer);
			}

		}

	}

}
#endif