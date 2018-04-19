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



/// FF GUIDS #

/// # FS GUIDS



/// FS GUIDS #

/// # OTHER GUIDS



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
				auto& ref = findNamedGuid(ptr);
				if ( FvGuids() <= &ref && &ref < FvGuidsEnd()) {
					return NamedGuidsRanges::FirmwareVolume;
				} else if (FfGuids() <= &ref && &ref < FfGuidsEnd()) {
					return NamedGuidsRanges::FirmwareFile;
				} else if (FsGuids() <= &ref && &ref < FsGuidsEnd()) {
					return NamedGuidsRanges::FirmwareSection;
				} else if (OtherGuids() <= &ref && &ref < OtherGuidsEnd()) {
					return NamedGuidsRanges::Other;
				} else {
					return NamedGuidsRanges::Unknown;
				}
			}
			inline NamedGuidsRanges::NamedGuidsRanges_t whatNamedGuidRange(const EFI_GUID& guid) { return whatNamedGuidRange(&guid); }
		};

	}

}

bool operator!=(const EFI_GUID& lhs, const EFI_GUID& rhs) noexcept { return std::memcmp(&lhs, &rhs, sizeof(EFI_GUID)); }

bool operator==(const EFI_GUID& lhs, const EFI_GUID& rhs) noexcept { return !operator!=(lhs, rhs); }

bool operator<(const EFI_GUID& lhs, const EFI_GUID& rhs) noexcept { return std::lexicographical_compare(UnifyPtrCast(&lhs), UnifyPtrCast(&lhs) + sizeof(EFI_GUID), UnifyPtrCast(&rhs), UnifyPtrCast(&rhs) + sizeof(EFI_GUID)); }

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