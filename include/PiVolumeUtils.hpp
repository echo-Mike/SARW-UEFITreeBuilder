#pragma once
#ifndef PI_VOLUME_UTILS_HPP__
#define PI_VOLUME_UTILS_HPP__ "0.0.0@PiVolumeUtils.hpp"

/// STD
#include <string>
#include <vector>

/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"
#include "PiVolumeObject.hpp"

#ifndef EFI_FV_EXT_TYPE_USED_SIZE_TYPE
#	define EFI_FV_EXT_TYPE_USED_SIZE_TYPE 0x03
#endif

// Set structure packing
#pragma pack(push,1)

extern "C" {

typedef struct {
	EFI_FIRMWARE_VOLUME_EXT_ENTRY Hdr;
	UINT32 UsedSize;
} FIRMWARE_VOLUME_EXT_ENTRY_USED_SIZE_TYPE;

}

// Clear structure packing
#pragma pack(pop)

namespace Project
{

	namespace Pi
	{

		namespace Volume
		{

			namespace Extension
			{

				typedef StructureView<FIRMWARE_VOLUME_EXT_ENTRY_USED_SIZE_TYPE> EntryUsedSize;
			}

			namespace Utils
			{

				Types::length_t getSize(const Pi::Volume::Header::const_pointer_t header);

				inline Types::length_t getSize(const Pi::Volume::Header& header) { return getSize(header.get()); }

				inline Types::length_t getSize(const PiObject::Volume& volume) { return getSize(volume.normalHdr.get()); }
				
				Types::length_t getHeaderSize(const Pi::Volume::Header::const_pointer_t header);

				inline Types::length_t getHeaderSize(const Pi::Volume::Header& header) { return getHeaderSize(header.get()); }

				inline Types::length_t getHeaderSize(const PiObject::Volume& volume) { return getHeaderSize(volume.normalHdr.get()); }

				bool addParsedVolume(const Pi::Volume::Header& header);

				bool isParsedVolume(const Pi::Volume::Header& header);

				std::string getAtributeString(const Pi::Volume::Header::const_pointer_t header, bool alternative = false);
				
				typedef std::vector< Extension::Entry > extended_header_entries_vec_t;

				extended_header_entries_vec_t findExtendedHeaderEntries(const Extension::Header& hdr, const MemoryView& buffer);

				bool hasSizeConflict(const Pi::Volume::Header::const_pointer_t header);

				inline bool hasSizeConflict(const Pi::Volume::Header& header) { return hasSizeConflict(header.get()); }

				inline bool hasSizeConflict(const PiObject::Volume& volume) { return hasSizeConflict(volume.normalHdr.get()); }

				Types::length_t getSizeBM(const Pi::Volume::Header::const_pointer_t header);

				inline bool getSizeBM(const Pi::Volume::Header& header) { return getSizeBM(header.get()); }

				inline bool getSizeBM(const PiObject::Volume& volume) { return getSizeBM(volume.normalHdr.get()); }

			}

		}

	}

}

#endif