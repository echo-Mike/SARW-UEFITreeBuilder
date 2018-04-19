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
	PROJ_MAKE_GUID_WITH_NAME1(EFI_PHOENIX_FILE_SYSTEM2_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_PHOENIX_NVRAM_EVSA_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_PHOENIX_NVRAM_EVSA2_GUID),
	PROJ_MAKE_GUID_WITH_NAME1(EFI_PHOENIX_NVRAM_NVAR_GUID), // + 12
#define PROJ_NAMED_FV_GUIDS_END 13
	// FF
	PROJ_MAKE_GUID_WITH_NAME1(PROJ_UNKNOWN_GUID), // + 13
#define PROJ_NAMED_FF_GUIDS_END 14
	// FS
	PROJ_MAKE_GUID_WITH_NAME1(PROJ_UNKNOWN_GUID), // + 14
#define PROJ_NAMED_FS_GUIDS_END 15
	// OTHER
	PROJ_MAKE_GUID_WITH_NAME1(PROJ_UNKNOWN_GUID), // + 15
#define PROJ_NAMED_OTHER_GUIDS_END 16
	// END
	PROJ_MAKE_GUID_WITH_NAME1(PROJ_UNKNOWN_GUID), // + 16
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

			const GuidWithName* FvGuidsEnd() { return NamedGuidsDB + PROJ_NAMED_FV_GUIDS_END; }

			const GuidWithName* FfGuids() { return NamedGuidsDB + PROJ_NAMED_FV_GUIDS_END; }

			const GuidWithName* FfGuidsEnd() { return NamedGuidsDB + PROJ_NAMED_FF_GUIDS_END; }

			const GuidWithName* FsGuids() { return NamedGuidsDB + PROJ_NAMED_FF_GUIDS_END; }

			const GuidWithName* FsGuidsEnd() { return NamedGuidsDB + PROJ_NAMED_FS_GUIDS_END; }

			const GuidWithName* OtherGuids() { return NamedGuidsDB + PROJ_NAMED_FS_GUIDS_END; }

			const GuidWithName* OtherGuidsEnd() { return NamedGuidsDB + PROJ_NAMED_OTHER_GUIDS_END; }

			const GuidWithName* end() { return NamedGuidsDB + (sizeof(NamedGuidsDB) / sizeof(GuidWithName) - 2); }

			const GuidWithName* testGuid() { return NamedGuidsDB + (sizeof(NamedGuidsDB) / sizeof(GuidWithName) - 1); }

		}

	}

}