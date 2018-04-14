/// STD
#include <cstring>

/// PROJECT
#include "vFFSGuids.hpp"

#define MAKE_FFS_GUID(enum_value, guid_macro) { enum_value, Guid(guid_macro), #guid_macro }

namespace Project
{
	namespace FfsGuids
	{
		const FfsGuid EffsGuids[] =
		{
			MAKE_FFS_GUID(KnownFfsGuids::EFfsV1, EFI_FIRMWARE_FILE_SYSTEM_GUID),
			MAKE_FFS_GUID(KnownFfsGuids::EFfsV2, EFI_FIRMWARE_FILE_SYSTEM2_GUID),
			MAKE_FFS_GUID(KnownFfsGuids::EFfsV3, EFI_FIRMWARE_FILE_SYSTEM3_GUID),
			MAKE_FFS_GUID(KnownFfsGuids::AppleV1, EFI_APPLE_BOOT_VOLUME_FILE_SYSTEM_GUID),
			MAKE_FFS_GUID(KnownFfsGuids::AppleV2, EFI_APPLE_BOOT_VOLUME_FILE_SYSTEM2_GUID),
			MAKE_FFS_GUID(KnownFfsGuids::IntelV1, EFI_INTEL_FILE_SYSTEM_GUID),
			MAKE_FFS_GUID(KnownFfsGuids::IntelV2, EFI_INTEL_FILE_SYSTEM2_GUID),
			MAKE_FFS_GUID(KnownFfsGuids::SonyV1, EFI_SONY_FILE_SYSTEM_GUID),
			MAKE_FFS_GUID(KnownFfsGuids::Unknown, PROJ_UNKNOWN_GUID)
		};

		const FfsGuid& isValidFfsGuid(Types::const_pointer_t ptr)
		{
			for (int index = 0; index < sizeof(EffsGuids) / sizeof(FfsGuid) - 2; ++index)
			{
				if (!std::memcmp( ptr, EffsGuids[index].value.bytes, Guid_size ))
				{
					return EffsGuids[index];
				}
			}
			return EffsGuids[sizeof(EffsGuids) / sizeof(FfsGuid) - 1];
		}

		bool isSameGuid(const EFI_GUID* lhs, const EFI_GUID* rhs)
		{
			return !std::memcmp(lhs, rhs, Guid_size);
		}

		const FfsGuid& getGuidByCode(KnownFfsGuids::KnownFfsGuids_t code)
		{
			switch (code)
			{
				case Project::FfsGuids::KnownFfsGuids::EFfsV1:
					return EffsGuids[0];
					break;
				case Project::FfsGuids::KnownFfsGuids::EFfsV2:
					return EffsGuids[1];
					break;
				case Project::FfsGuids::KnownFfsGuids::EFfsV3:
					return EffsGuids[2];
					break;
				case Project::FfsGuids::KnownFfsGuids::AppleV1:
					return EffsGuids[3];
					break;
				case Project::FfsGuids::KnownFfsGuids::AppleV2:
					return EffsGuids[4];
					break;
				case Project::FfsGuids::KnownFfsGuids::IntelV1:
					return EffsGuids[5];
					break;
				case Project::FfsGuids::KnownFfsGuids::IntelV2:
					return EffsGuids[6];
					break;
				case Project::FfsGuids::KnownFfsGuids::SonyV1:
					return EffsGuids[7];
					break;
				case Project::FfsGuids::KnownFfsGuids::Unknown:
				default:
					return EffsGuids[sizeof(EffsGuids) / sizeof(FfsGuid) - 1];
					break;
			}
		}
	}
}