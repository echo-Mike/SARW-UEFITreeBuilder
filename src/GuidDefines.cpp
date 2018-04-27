/// STD
#include <algorithm>
/// PROJECT
#include "GuidDefines.h"

static const Project::Guid::GuidWithName NamedGuidsDB[] =
{   // FV
	PROJ_MAKE_GUID_WITH_NAME1(EFI_FIRMWARE_FILE_SYSTEM_GUID), // + 0
	PROJ_MAKE_GUID_WITH_NAME1(EFI_FIRMWARE_FILE_SYSTEM2_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_FIRMWARE_FILE_SYSTEM3_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_APPLE_BOOT_VOLUME_FILE_SYSTEM_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_APPLE_BOOT_VOLUME_FILE_SYSTEM2_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_INTEL_FILE_SYSTEM_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_INTEL_FILE_SYSTEM2_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_SONY_FILE_SYSTEM_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_PHOENIX_FILE_SYSTEM_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_PHOENIX_FILE_SYSTEM2_GUID), // +9
#define PROJ_KNOWN_FFS_GUIDS_END 10
	PROJ_MAKE_GUID_WITH_NAME1(EFI_PHOENIX_NVRAM_EVSA_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_PHOENIX_NVRAM_EVSA2_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_PHOENIX_NVRAM_NVAR_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(PROJ_UNKNOWN_GUID), // + 13
#	define PROJ_NAMED_FV_GUIDS_END 13
	// FF
	PROJ_MAKE_GUID_WITH_NAME1(PEI_APRIORI_FILE_NAME_GUID), // + 14
	PROJ_MAKE_GUID_WITH_NAME1(PEI_APRIORI_FILE_NAME2_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_DXE_APRIORI_FILE_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_FFS_VOLUME_TOP_FILE_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(PROJ_UNKNOWN_GUID), // + 18
#	define PROJ_NAMED_FF_GUIDS_END 18
	// FS
	PROJ_MAKE_GUID_WITH_NAME1(LZMA_CUSTOM_DECOMPRESS_GUID), // + 19
	PROJ_MAKE_GUID_WITH_NAME1(LZMAF86_CUSTOM_DECOMPRESS_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(BROTLI_CUSTOM_DECOMPRESS_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(TIANO_CUSTOM_DECOMPRESS_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_CRC32_GUIDED_SECTION_EXTRACTION_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_CERT_TYPE_RSA2048_SHA256_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_FIRMWARE_CONTENTS_SIGNED_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(PROJ_UNKNOWN_GUID), // + 26
#	define PROJ_NAMED_FS_GUIDS_END 26
	// OTHER
	PROJ_MAKE_GUID_WITH_NAME1(EFI_CERT_TYPE_PKCS7_GUID), // + 27
	PROJ_MAKE_GUID_WITH_NAME1(PROJ_UNKNOWN_GUID), // + 28
#	define PROJ_NAMED_OTHER_GUIDS_END 28
	// END
	PROJ_MAKE_GUID_WITH_NAME1(PROJ_UNKNOWN_GUID), // + 29
	PROJ_MAKE_GUID_WITH_NAME1(PROJ_TEST_GUID)
};

namespace Project
{

	namespace Guid
	{

		namespace NamedGuids
		{

			const GuidWithName* begin() { return NamedGuidsDB; }

			const GuidWithName* FvGuids() { return NamedGuidsDB; }

			const GuidWithName* FfsGuidsEnd() { return NamedGuidsDB + PROJ_KNOWN_FFS_GUIDS_END; }

			const GuidWithName* FvGuidsEnd() { return NamedGuidsDB + PROJ_NAMED_FV_GUIDS_END; }

			const GuidWithName* FfGuids() { return NamedGuidsDB + PROJ_NAMED_FV_GUIDS_END + 1; }

			const GuidWithName* FfGuidsEnd() { return NamedGuidsDB + PROJ_NAMED_FF_GUIDS_END; }

			const GuidWithName* FsGuids() { return NamedGuidsDB + PROJ_NAMED_FF_GUIDS_END + 1; }

			const GuidWithName* FsGuidsEnd() { return NamedGuidsDB + PROJ_NAMED_FS_GUIDS_END; }

			const GuidWithName* OtherGuids() { return NamedGuidsDB + PROJ_NAMED_FS_GUIDS_END + 1; }

			const GuidWithName* OtherGuidsEnd() { return NamedGuidsDB + PROJ_NAMED_OTHER_GUIDS_END; }

			const GuidWithName* end() { return NamedGuidsDB + (sizeof(NamedGuidsDB) / sizeof(GuidWithName) - 2); }

			const GuidWithName* testGuid() { return NamedGuidsDB + (sizeof(NamedGuidsDB) / sizeof(GuidWithName) - 1); }

		}

		FindGUIDResult::FindGUIDResult_t findGuid(const EFI_GUID* guid, Types::const_void_ptr_t buffer, Types::length_t length, FindGUIDResult::result_t& result)
		{
			result.begin = nullptr;
			result.offset = 0;
			if (length < sizeof(EFI_GUID)) {
				return FindGUIDResult::BufferToSmall;
			}
			
			Types::const_byte_ptr_t guidBegin = reinterpret_cast<Types::const_byte_ptr_t>(guid);
			Types::const_byte_ptr_t guidEnd = guidBegin + sizeof(EFI_GUID);
			Types::const_byte_ptr_t buffBegin = reinterpret_cast<Types::const_byte_ptr_t>(buffer);
			Types::const_byte_ptr_t buffEnd = buffBegin + length;

			result.begin = std::search(buffBegin, buffEnd, guidBegin, guidEnd);

			if (result.begin == buffEnd) {
				result.begin = nullptr;
				return FindGUIDResult::NotFound;
			} else {
				result.offset = result.begin - buffBegin;
				return FindGUIDResult::Found;
			}
		}

	}

}