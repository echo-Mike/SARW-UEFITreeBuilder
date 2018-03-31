#pragma once
//STD
#include <cstring>
#include <cstddef>
//UEFI
extern "C" {
    #include "Base.h"
    #include "Uefi.h"
    #include <Pi/PiFirmwareVolume.h>
}

// 7A9354D9-0468-444A-81CE-0BF617D890DF
#define EFI_FIRMWARE_FILE_SYSTEM_GUID \
    { 0x7a9354d9, 0x0468, 0x444a, { 0x81, 0xce, 0x0b, 0xf6, 0x17, 0xd8, 0x90, 0xdf } }
// 8C8CE578-8A3D-4f1C-9935-896185C32DD3
#define EFI_FIRMWARE_FILE_SYSTEM2_GUID \
    { 0x8c8ce578, 0x8a3d, 0x4f1c, { 0x99, 0x35, 0x89, 0x61, 0x85, 0xc3, 0x2d, 0xd3 } }
// 5473C07A-3DCB-4DCA-BD6F-1E9689E7349A
#define EFI_FIRMWARE_FILE_SYSTEM3_GUID \
    { 0x5473c07a, 0x3dcb, 0x4dca, { 0xbd, 0x6f, 0x1e, 0x96, 0x89, 0xe7, 0x34, 0x9a } }

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

namespace Project 
{

    union guid_cast {
        using CharRepr = unsigned char[sizeof(EFI_GUID)];
        CharRepr bytes;
        EFI_GUID guid;
        constexpr guid_cast(EFI_GUID Guid_) : guid(Guid_) {}
    };

    const guid_cast EFFS_GUIDS[] = {
        guid_cast(EFI_FIRMWARE_FILE_SYSTEM2_GUID),
        guid_cast(EFI_APPLE_BOOT_VOLUME_FILE_SYSTEM_GUID),
        guid_cast(EFI_APPLE_BOOT_VOLUME_FILE_SYSTEM2_GUID),
        guid_cast(EFI_INTEL_FILE_SYSTEM_GUID),
        guid_cast(EFI_INTEL_FILE_SYSTEM2_GUID),
        guid_cast(EFI_SONY_FILE_SYSTEM_GUID),
        guid_cast(EFI_FIRMWARE_FILE_SYSTEM3_GUID),
        guid_cast(EFI_FIRMWARE_FILE_SYSTEM_GUID)
    };

    const char* EFFS_GUIDS_NAMES[] = {
        "EFI_FIRMWARE_FILE_SYSTEM2_GUID",
        "EFI_APPLE_BOOT_VOLUME_FILE_SYSTEM_GUID",
        "EFI_APPLE_BOOT_VOLUME_FILE_SYSTEM2_GUID",
        "EFI_INTEL_FILE_SYSTEM_GUID",
        "EFI_INTEL_FILE_SYSTEM2_GUID",
        "EFI_SONY_FILE_SYSTEM_GUID",
        "EFI_FIRMWARE_FILE_SYSTEM3_GUID",
        "EFI_FIRMWARE_FILE_SYSTEM_GUID"
    };

    int isValidEFFSGUID(EFI_FIRMWARE_VOLUME_HEADER* FVHeader) {
        for (std::size_t index = 0; index < 8; ++index)
            if (!std::memcmp(reinterpret_cast<const char*>(FVHeader) + offsetof(EFI_FIRMWARE_VOLUME_HEADER, FileSystemGuid), EFFS_GUIDS[index].bytes, sizeof(EFI_GUID)))
                return index;
        return -1;
    }
}