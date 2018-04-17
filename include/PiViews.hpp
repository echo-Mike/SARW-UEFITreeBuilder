#pragma once
#ifndef PI_VIEWS_HPP__
#define PI_VIEWS_HPP__ "0.0.0@PiViews.hpp"

/// PROJECT
#include "cStructureView.hpp"

namespace Project
{

	namespace Pi
	{

		namespace Volume
		{

			typedef StructureView<EFI_FIRMWARE_VOLUME_HEADER> Header;

			typedef StructureView<EFI_FV_BLOCK_MAP_ENTRY> BlockMap;

			namespace Extension
			{

				typedef StructureView<EFI_FIRMWARE_VOLUME_EXT_HEADER> Header;
				typedef StructureView<EFI_FIRMWARE_VOLUME_EXT_ENTRY> Entry;
				typedef StructureView<EFI_FIRMWARE_VOLUME_EXT_ENTRY_OEM_TYPE> EntryOemType;
				typedef StructureView<EFI_FIRMWARE_VOLUME_EXT_ENTRY_GUID_TYPE> EntryGuidType;
			}

		}

		namespace File
		{

			typedef StructureView<EFI_FFS_FILE_HEADER> Header;
			typedef StructureView<EFI_FFS_INTEGRITY_CHECK> IntgrCheck;

			namespace Extended
			{

				typedef StructureView<EFI_FFS_FILE_HEADER2> Header;
			}

		}

		namespace Section
		{

			struct SectionTag {};
			struct SectionExtTag {};

			typedef StructureView<EFI_COMMON_SECTION_HEADER> Header;
			
			typedef StructureView<EFI_COMPATIBILITY16_SECTION, SectionTag> Compatibility16;
			typedef StructureView<EFI_COMPRESSION_SECTION, SectionTag> Compression;
			typedef StructureView<EFI_DISPOSABLE_SECTION, SectionTag> Disposable;
			typedef StructureView<EFI_DXE_DEPEX_SECTION, SectionTag> DxeDepex;
			typedef StructureView<EFI_FIRMWARE_VOLUME_IMAGE_SECTION, SectionTag> FirmwareVolumeImage;
			typedef StructureView<EFI_FREEFORM_SUBTYPE_GUID_SECTION, SectionTag> FreeformSubtypeGuid;
			typedef StructureView<EFI_GUID_DEFINED_SECTION, SectionTag> GuidDefined;
			typedef StructureView<EFI_PE32_SECTION, SectionTag> Pe32;
			typedef StructureView<EFI_PEI_DEPEX_SECTION, SectionTag> PeiDepex;
			typedef StructureView<EFI_PIC_SECTION, SectionTag> Pic;
			typedef StructureView<EFI_TE_SECTION, SectionTag> Te;
			typedef StructureView<EFI_RAW_SECTION, SectionTag> Raw;
			typedef StructureView<EFI_SMM_DEPEX_SECTION, SectionTag> SmmDepex;
			typedef StructureView<EFI_USER_INTERFACE_SECTION, SectionTag> UserInterface;
			typedef StructureView<EFI_VERSION_SECTION, SectionTag> Version;
			
			namespace Extended
			{

				typedef StructureView<EFI_COMMON_SECTION_HEADER2> Header;

				typedef StructureView<EFI_COMPATIBILITY16_SECTION2, SectionExtTag> Compatibility16;
				typedef StructureView<EFI_COMPRESSION_SECTION2, SectionExtTag> Compression;
				typedef StructureView<EFI_DISPOSABLE_SECTION2, SectionExtTag> Disposable;
				typedef StructureView<EFI_DXE_DEPEX_SECTION2, SectionExtTag> DxeDepex;
				typedef StructureView<EFI_FIRMWARE_VOLUME_IMAGE_SECTION2, SectionExtTag> FirmwareVolumeImage;
				typedef StructureView<EFI_FREEFORM_SUBTYPE_GUID_SECTION2, SectionExtTag> FreeformSubtypeGuid;
				typedef StructureView<EFI_GUID_DEFINED_SECTION2, SectionExtTag> GuidDefined;
				typedef StructureView<EFI_PE32_SECTION2, SectionExtTag> Pe32;
				typedef StructureView<EFI_PEI_DEPEX_SECTION2, SectionExtTag> PeiDepex;
				typedef StructureView<EFI_PIC_SECTION2, SectionExtTag> Pic;
				typedef StructureView<EFI_TE_SECTION2, SectionExtTag> Te;
				typedef StructureView<EFI_RAW_SECTION2, SectionExtTag> Raw;
				typedef StructureView<EFI_SMM_DEPEX_SECTION2, SectionExtTag> SmmDepex;
				typedef StructureView<EFI_USER_INTERFACE_SECTION2, SectionExtTag> UserInterface;
				typedef StructureView<EFI_VERSION_SECTION2, SectionExtTag> Version;
			}

		}

	}

}

#endif