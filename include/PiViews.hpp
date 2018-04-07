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
			
			namespace Extended
			{
				typedef StructureView<EFI_FFS_FILE_HEADER2> Header;
			}
		}

		namespace Section
		{
			typedef StructureView<EFI_COMMON_SECTION_HEADER> Header;
			typedef StructureView<EFI_COMPATIBILITY16_SECTION> Compatibility16;
			typedef StructureView<EFI_COMPRESSION_SECTION> Compression;
			typedef StructureView<EFI_DISPOSABLE_SECTION> Disposable;
			typedef StructureView<EFI_DXE_DEPEX_SECTION> DxeDepex;
			typedef StructureView<EFI_FIRMWARE_VOLUME_IMAGE_SECTION> FirmwareVolumeImage;
			typedef StructureView<EFI_FREEFORM_SUBTYPE_GUID_SECTION> FreeformSubtypeGuid;
			typedef StructureView<EFI_GUID_DEFINED_SECTION> GuidDefined;
			typedef StructureView<EFI_PE32_SECTION> Pe32;
			typedef StructureView<EFI_PEI_DEPEX_SECTION> PeiDepex;
			typedef StructureView<EFI_PIC_SECTION> Pic;
			typedef StructureView<EFI_TE_SECTION> Te;
			typedef StructureView<EFI_RAW_SECTION> Raw;
			typedef StructureView<EFI_SMM_DEPEX_SECTION> SmmDepex;
			typedef StructureView<EFI_USER_INTERFACE_SECTION> UserInterface;
			typedef StructureView<EFI_VERSION_SECTION> Version;
			
			namespace Extended
			{
				typedef StructureView<EFI_COMMON_SECTION_HEADER2> Header;
				typedef StructureView<EFI_COMPATIBILITY16_SECTION2> Compatibility16;
				typedef StructureView<EFI_COMPRESSION_SECTION2> Compression;
				typedef StructureView<EFI_DISPOSABLE_SECTION2> Disposable;
				typedef StructureView<EFI_DXE_DEPEX_SECTION2> DxeDepex;
				typedef StructureView<EFI_FIRMWARE_VOLUME_IMAGE_SECTION2> FirmwareVolumeImage;
				typedef StructureView<EFI_FREEFORM_SUBTYPE_GUID_SECTION2> FreeformSubtypeGuid;
				typedef StructureView<EFI_GUID_DEFINED_SECTION2> GuidDefined;
				typedef StructureView<EFI_PE32_SECTION2> Pe32;
				typedef StructureView<EFI_PEI_DEPEX_SECTION2> PeiDepex;
				typedef StructureView<EFI_PIC_SECTION2> Pic;
				typedef StructureView<EFI_TE_SECTION2> Te;
				typedef StructureView<EFI_RAW_SECTION2> Raw;
				typedef StructureView<EFI_SMM_DEPEX_SECTION2> SmmDepex;
				typedef StructureView<EFI_USER_INTERFACE_SECTION2> UserInterface;
				typedef StructureView<EFI_VERSION_SECTION2> Version;
			}
		}
	}
}

#endif