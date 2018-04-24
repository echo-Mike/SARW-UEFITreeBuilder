#pragma once
#ifndef GUID_DEFINES_H__
#define GUID_DEFINES_H__ "0.0.0@GuidDefines.h"

/// PROJECT
#include "ProjectGlobalDefines.h"

PROJ_EXTERN_C

#include <Uefi/UefiBaseType.h>

PROJ_EXTERN_C_END

/// GUIDS

// 00000000-0000-0000-0000-000000000000
#define PROJ_UNKNOWN_GUID \
	{ 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }

// 00112233-4455-6677-8899-AABBCCDDEEFF
#define PROJ_TEST_GUID \
	{ 0x00112233, 0x4455, 0x6677, { 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF } }

/// # FV GUIDS

/// ## KNOWN FFS GUIDS

/// UEFI STD

// 7A9354D9-0468-444A-81CE-0BF617D890DF
#define EFI_FIRMWARE_FILE_SYSTEM_GUID \
    { 0x7a9354d9, 0x0468, 0x444a, { 0x81, 0xce, 0x0b, 0xf6, 0x17, 0xd8, 0x90, 0xdf } }
// 8C8CE578-8A3D-4f1C-9935-896185C32DD3
#define EFI_FIRMWARE_FILE_SYSTEM2_GUID \
    { 0x8c8ce578, 0x8a3d, 0x4f1c, { 0x99, 0x35, 0x89, 0x61, 0x85, 0xc3, 0x2d, 0xd3 } }
// 5473C07A-3DCB-4DCA-BD6F-1E9689E7349A
#define EFI_FIRMWARE_FILE_SYSTEM3_GUID \
    { 0x5473c07a, 0x3dcb, 0x4dca, { 0xbd, 0x6f, 0x1e, 0x96, 0x89, 0xe7, 0x34, 0x9a } }

/// UEFI TOOL : https://github.com/LongSoft/UEFITool

// 04ADEEAD-61FF-4D31-B6BA-64F8BF901F5A
#define EFI_APPLE_BOOT_VOLUME_FILE_SYSTEM_GUID \
    { 0x04adeead, 0x61ff, 0x4d31, { 0xb6, 0xba, 0x64, 0xf8, 0xbf, 0x90, 0x1f, 0x5a } }
// BD001B8C-6A71-487B-A14F-0C2A2DCF7A5D
#define EFI_APPLE_BOOT_VOLUME_FILE_SYSTEM2_GUID \
    { 0xbd001b8c, 0x6a71, 0x487b, { 0xa1, 0x4f, 0x0c, 0x2a, 0x2d, 0xcf, 0x7a, 0x5d } }

// AD3FFFFF-D28B-44C4-9F13-9EA98A97F9F0
#define EFI_INTEL_FILE_SYSTEM_GUID \
    { 0xad3fffff, 0xd28b, 0x44c4, { 0x9f, 0x13, 0x9e, 0xa9, 0x8a, 0x97, 0xf9, 0xf0 } }
// D6A1CD70-4B33-4994-A6EA-375F2CCC5437
#define EFI_INTEL_FILE_SYSTEM2_GUID \
    { 0xd6a1cd70, 0x4b33, 0x4994, { 0xa6, 0xea, 0x37, 0x5f, 0x2c, 0xcc, 0x54, 0x37 } }

// 4F494156-AED6-4D64-A537-B8A5557BCEEC
#define EFI_SONY_FILE_SYSTEM_GUID \
    { 0x4f494156, 0xaed6, 0x4d64, { 0xa5, 0x37, 0xb8, 0xa5, 0x55, 0x7b, 0xce, 0xec } }

/// UEFI FIRMWARE PARSER : https://github.com/theopolis/uefi-firmware-parser
/// SKOCHINSKY'S GIST : https://gist.github.com/skochinsky/3b8f948bfef6f8b32c0577ac018bdaf9

// 16B45DA2-7D70-4AEA-A58D-760E9ECB841D
#define EFI_PHOENIX_FILE_SYSTEM_GUID \
    { 0x16b45da2, 0x7d70, 0x4aea, { 0xa5, 0x8d, 0x76, 0x0e, 0x9e, 0xcb, 0x84, 0x1d } }
// E360BDBA-C3CE-46BE-8F37-B231E5CB9F35
#define EFI_PHOENIX_FILE_SYSTEM2_GUID \
    { 0xe360bdba, 0xc3ce, 0x46be, { 0x8f, 0x37, 0xb2, 0x31, 0xe5, 0xcb, 0x9f, 0x35 } }

/// KNOWN FFS GUIDS ##

/// ## KNOWN FV NOT FFS GUIDS

/// UEFI FIRMWARE PARSER : https://github.com/theopolis/uefi-firmware-parser
/// SKOCHINSKY'S GIST : https://gist.github.com/skochinsky/3b8f948bfef6f8b32c0577ac018bdaf9

// FFF12B8D-7696-4C8B-A985-2747075B4F50
#define EFI_PHOENIX_NVRAM_EVSA_GUID \
	{ 0xfff12b8d, 0x7696, 0x4c8b, { 0xa9, 0x85, 0x27, 0x47, 0x07, 0x5b, 0x4f, 0x50 } }
// 00504624-8A59-4EEB-BD0F-6B36E96128E0
#define EFI_PHOENIX_NVRAM_EVSA2_GUID \
	{ 0x00504624, 0x8a59, 0x4eeb, { 0xbd, 0x0f, 0x6b, 0x36, 0xe9, 0x61, 0x28, 0xe0 } }
// CEF5B9A3-476D-497F-9FDC-E98143E0422C
#define EFI_PHOENIX_NVRAM_NVAR_GUID \
	{ 0xcef5b9a3, 0x476d, 0x497f, { 0x9f, 0xdc, 0xe9, 0x81, 0x43, 0xe0, 0x42, 0x2c } }

/// KNOWN FV NOT FFS GUIDS ##

/// FV GUIDS #

/// # FF GUIDS

/// ## APRIORI FILE GUIDS

/// UEFI PI SPEC : Volume 1: Pre - EFI Initialization Core Interface page 64

/**	
 * There is a controversy because GUID in UEFI PI is defined as
 * { 0x1b45cc0a, 0x156a, 0x428a, 0xaf62, 0x49, 0x86, 0x4d, 0xa0, 0xe6, 0xe6 } 
 * which means that for different endianness platforms different GUIDs will be produced.
 * We define both variants for big- and little-endian platforms.
 */
/// AS DEFINED IN UEFITOOL : ffs.h
// 1B45CC0A-156A-428A-AF62-49864DA0E6E6
#define PEI_APRIORI_FILE_NAME_GUID \
	{ 0x1b45cc0a, 0x156a, 0x428a, { 0xaf, 0x62, 0x49, 0x86, 0x4d, 0xa0, 0xe6, 0xe6 } }
/// AS DEFINED IN EDK2 : https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Guid/AprioriFileName.h
// 1B45CC0A-156A-428A-62AF-49864DA0E6E6
#define PEI_APRIORI_FILE_NAME2_GUID \
	{ 0x1b45cc0a, 0x156a, 0x428a, { 0x62, 0xaf, 0x49, 0x86, 0x4d, 0xa0, 0xe6, 0xe6 } }

/// EDK2 : https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Guid/Apriori.h

// FC510EE7-FFDC-11D4-BD41-0080C73C8881
#define EFI_DXE_APRIORI_FILE_GUID \
	{ 0xfc510ee7, 0xffdc, 0x11d4, { 0xbd, 0x41, 0x00, 0x80, 0xc7, 0x3c, 0x88, 0x81 } }

/// APRIORI FILE GUIDS ##

/// UEFI PI SPEC : Volume 3 : Shared Architectural Elements page 39

// 1BA0062E-C779-4582-8566-336AE8F78F09
#define EFI_FFS_VOLUME_TOP_FILE_GUID \
	{ 0x1BA0062E, 0xC779, 0x4582, { 0x85, 0x66, 0x33, 0x6A, 0xE8, 0xF7, 0x8F, 0x09 } }

/// FF GUIDS #

/// # FS GUIDS

/// ## COMPRESSED SECTION GUIDS

/// EDK2 SOURCE : https://github.com/tianocore/edk2/blob/master/MdeModulePkg/Include/Guid/LzmaDecompress.h

// EE4E5898-3914-4259-9D6E-DC7BD79403CF
#define LZMA_CUSTOM_DECOMPRESS_GUID  \
	{ 0xEE4E5898, 0x3914, 0x4259, { 0x9D, 0x6E, 0xDC, 0x7B, 0xD7, 0x94, 0x03, 0xCF } }
// D42AE6BD-1352-4BFB-909A-CA72A6EAE889
#define LZMAF86_CUSTOM_DECOMPRESS_GUID  \
	{ 0xD42AE6BD, 0x1352, 0x4bfb, { 0x90, 0x9A, 0xCA, 0x72, 0xA6, 0xEA, 0xE8, 0x89 } }

/// EDK2 SOURCE : https://github.com/tianocore/edk2/blob/master/MdeModulePkg/MdeModulePkg.dec

// 3D532050-5CDA-4FD0-879E-0F7F630D5AFB
#define BROTLI_CUSTOM_DECOMPRESS_GUID  \
	{ 0x3D532050, 0x5CDA, 0x4FD0, { 0x87, 0x9E, 0x0F, 0x7F, 0x63, 0x0D, 0x5A, 0xFB } }

/// EDK2 SOURCE : https://github.com/tianocore/edk2/blob/master/IntelFrameworkModulePkg/Include/Guid/TianoDecompress.h

// A31280AD-481E-41B6-95E8-127F4C984779
#define TIANO_CUSTOM_DECOMPRESS_GUID  \
	{ 0xA31280AD, 0x481E, 0x41B6, { 0x95, 0xE8, 0x12, 0x7F, 0x4C, 0x98, 0x47, 0x79 } }

/// COMPRESSED SECTION GUIDS ##

/// EDK2 SOURCE : https://github.com/tianocore/edk2/blob/master/MdeModulePkg/Include/Guid/Crc32GuidedSectionExtraction.h

// FC1BCDB0-7D31-49AA-936A-A4600D9DD083
#define EFI_CRC32_GUIDED_SECTION_EXTRACTION_GUID \
	{ 0xFC1BCDB0, 0x7D31, 0x49aa, { 0x93, 0x6A, 0xA4, 0x60, 0x0D, 0x9D, 0xD0, 0x83 } }

/// EDK2 SOURCE : https://github.com/tianocore/edk2/blob/master/BaseTools/Source/C/Include/Common/UefiMultiPhase.h

// A7717414-C616-4977-9420-844712A735BF
#define EFI_CERT_TYPE_RSA2048_SHA256_GUID \
	{ 0xa7717414, 0xc616, 0x4977, { 0x94, 0x20, 0x84, 0x47, 0x12, 0xa7, 0x35, 0xbf } }

/// UEFI PI : Volume 3 : Shared Architectural Elements page 57
/// EDK2 SOURCE : https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Guid/FirmwareContentsSigned.h

// 0F9D89E8-9259-4F76-A5AF-0C89E34023DF
#define EFI_FIRMWARE_CONTENTS_SIGNED_GUID \
	{ 0x0f9d89e8, 0x9259, 0x4f76, { 0xa5, 0xaf, 0x0c, 0x89, 0xe3, 0x40, 0x23, 0xdf } }

/// FS GUIDS #

/// # OTHER GUIDS

/// EDK2 SOURCE : https://github.com/tianocore/edk2/blob/master/MdePkg/Include/Guid/FirmwareContentsSigned.h

// 4AAFD29D-68DF-49EE-8AA9-347D375665A7
#define EFI_CERT_TYPE_PKCS7_GUID  \
	{ 0x4aafd29d, 0x68df, 0x49ee, { 0x8a, 0xa9, 0x34, 0x7d, 0x37, 0x56, 0x65, 0xa7 } }

/// OTHER GUIDS #

/// GUIDS

#ifdef PROJ_CPP_CONTEXT
/// STD
// For std::memcmp
#include <cstring>
// For std::lexicographical_compare and std::find
#include <algorithm>

/// PROJECT
#include "ProjectTypes.hpp"

#ifndef UnifyPtrCast
#	define UnifyPtrCast(Ptr) (reinterpret_cast<::Project::Types::const_pointer_t>((Ptr)))
#endif

namespace Project
{

	namespace Guid
	{

		typedef EFI_GUID type;

		inline Types::const_pointer_t begin(const type& guid) { return reinterpret_cast<Types::const_pointer_t>(&guid); }

		static const Types::length_t struct_size = sizeof(EFI_GUID);

		inline Types::const_pointer_t end(const type& guid) { return reinterpret_cast<Types::const_pointer_t>(&guid) + struct_size; }

		struct GuidWithName
		{
			const type value;
			const char* name;
		};

		namespace NamedGuids
		{

			namespace NamedGuidsRanges
			{
				
				enum NamedGuidsRanges_t
				{
					FirmwareVolume,
					FirmwareFile,
					FirmwareSection,
					Other,
					Unknown
				};

			}
			
			const GuidWithName* begin();
			const GuidWithName* FvGuids();
			const GuidWithName* FvGuidsEnd();
			const GuidWithName* FfGuids();
			const GuidWithName* FfGuidsEnd();
			const GuidWithName* FsGuids();
			const GuidWithName* FsGuidsEnd();
			const GuidWithName* OtherGuids();
			const GuidWithName* OtherGuidsEnd();
			const GuidWithName* end();
			const GuidWithName* testGuid();

			inline const GuidWithName& findNamedGuid(const EFI_GUID* ptr)
			{
				return *(
					std::find(
						begin(), 
						end(),
						[ptr](const GuidWithName& gwn) -> bool
						{
							return gwn.value == *ptr;
						}
					)
				);
			}
			inline const GuidWithName& findNamedGuid(const EFI_GUID& guid) { return findNamedGuid(&guid); }

			inline bool isNamedGuid(const EFI_GUID* ptr)
			{
				auto& ref = findNamedGuid(ptr);
				return &ref != end();
			}
			inline bool isNamedGuid(const EFI_GUID& guid) { return isNamedGuid(&guid); }

			inline NamedGuidsRanges::NamedGuidsRanges_t whatNamedGuidRange(const EFI_GUID* ptr)
			{
				auto ptr_ = &findNamedGuid(ptr);
				if ( FvGuids() <= ptr_ && ptr_ < FvGuidsEnd()) {
					return NamedGuidsRanges::FirmwareVolume;
				} else if (FfGuids() <= ptr_ && ptr_ < FfGuidsEnd()) {
					return NamedGuidsRanges::FirmwareFile;
				} else if (FsGuids() <= ptr_ && ptr_ < FsGuidsEnd()) {
					return NamedGuidsRanges::FirmwareSection;
				} else if (OtherGuids() <= ptr_ && ptr_ < OtherGuidsEnd()) {
					return NamedGuidsRanges::Other;
				} else {
					return NamedGuidsRanges::Unknown;
				}
			}
			inline NamedGuidsRanges::NamedGuidsRanges_t whatNamedGuidRange(const EFI_GUID& guid) { return whatNamedGuidRange(&guid); }
		};

		namespace FindGUIDResult
		{

			enum FindGUIDResult_t
			{
				NotFound,
				Found,
				BufferToSmall
			};

			struct result_t
			{
				Types::const_byte_ptr_t begin;
				Types::length_t offset;
			};

		}

		FindGUIDResult::FindGUIDResult_t findGuid(const EFI_GUID* guid, Types::const_void_ptr_t buffer, Types::length_t length, FindGUIDResult::result_t& result);
		inline FindGUIDResult::FindGUIDResult_t findGuid(const EFI_GUID& guid, Types::const_void_ptr_t buffer, Types::length_t length, FindGUIDResult::result_t& result)
		{
			return findGuid(&guid, buffer, length, result);
		}

	}

}

inline bool operator!=(const EFI_GUID& lhs, const EFI_GUID& rhs) noexcept { return std::memcmp(&lhs, &rhs, sizeof(EFI_GUID)); }

inline bool operator==(const EFI_GUID& lhs, const EFI_GUID& rhs) noexcept { return !operator!=(lhs, rhs); }

inline bool operator< (const EFI_GUID& lhs, const EFI_GUID& rhs) noexcept { return std::lexicographical_compare(UnifyPtrCast(&lhs), UnifyPtrCast(&lhs) + sizeof(EFI_GUID), UnifyPtrCast(&rhs), UnifyPtrCast(&rhs) + sizeof(EFI_GUID)); }

#define PROJ_GUID_SIZE (::Project::Guid::struct_size)

#else

typedef struct {
	const EFI_GUID value;
	const char* name;
} GuidWithName;

#define PROJ_GUID_SIZE (sizeof(EFI_GUID))

#endif

#define PROJ_MAKE_GUID_WITH_NAME1(guid_macro) { guid_macro, #guid_macro }

#define PROJ_MAKE_GUID_WITH_NAME2(guid, name) { guid, name }

#endif