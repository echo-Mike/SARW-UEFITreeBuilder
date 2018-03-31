#pragma once
//STD
#include <cstdlib>
#include <cstddef>
//UEFI
extern "C" {
    #include "Base.h"
    #include "Uefi.h"
    #include <Pi/PiFirmwareVolume.h>
    #include <Pi/PiFirmwareFile.h>
}

namespace Project {

    struct MemoryProxy {
        void *begin = nullptr, *end = nullptr;
        std::size_t length = 0;

        explicit MemoryProxy(
            void* begin_ = nullptr, 
            void* end_ = nullptr) :
                begin(begin_), end(end_), length(0)
        {
            if (begin_ && end_) {
                auto *begin__ = reinterpret_cast<std::uint8_t*>(begin_), 
                     *end__ = reinterpret_cast<std::uint8_t*>(end_);
                if (begin__ > end__) {
                    std::swap(begin, end);
                    std::swap(begin__, end__);
                }
                length = end__ - begin__;
            }
        }

        void setEnd(const void* endp_) { 
            end = const_cast<void*>(endp_);
            if (begin && end) {
                auto *begin_ = reinterpret_cast<std::uint8_t*>(begin), 
                     *end_ = reinterpret_cast<std::uint8_t*>(end);
                if (begin_ > end_) std::swap(begin_, end_);
                length = end_ - begin_;
            }
        }

        void setLength(std::size_t length_) { 
            length = length_;
            end = (begin && length_ > 0) ? reinterpret_cast<std::uint8_t*>(begin) + (length - 1) : begin;
        }

        operator char*() const { return reinterpret_cast<char*>(begin); }
        operator void*() const { return begin; }
        operator unsigned char*() const { return reinterpret_cast<unsigned char*>(begin); }        
        operator std::uint8_t*() const { return reinterpret_cast<std::uint8_t*>(begin); }

        operator const char*() const { return reinterpret_cast<const char*>(begin); }
        operator const void*() const { return const_cast<const void*>(begin); }
        operator const unsigned char*() const { return reinterpret_cast<const unsigned char*>(begin); }
        operator const std::uint8_t*() const { return reinterpret_cast<const std::uint8_t*>(begin); }

        MemoryProxy& operator=(const void* ptr_) {
            if (ptr_) {
                begin = const_cast<void*>(ptr_);
                if (length > 0) setLength(length);
            }
            return *this;
        }
    };

    template < typename StructureT >
    struct StructureProxy : public MemoryProxy {
        using StructureType = StructureT;
        StructureType* structure = nullptr;
        std::size_t structureSize = sizeof(StructureType);

        StructureProxy(const void* ptr_) //: MemoryProxy() 
        {
            structure = reinterpret_cast<StructureType*>(const_cast<void*>(ptr_));
            begin = const_cast<void*>(ptr_);
            structureSize = sizeof(StructureType);
        }

        StructureType* operator->() const { return structure; }

        operator StructureType*() const { return structure; }
        operator char*() const { return reinterpret_cast<char*>(structure); }
        operator void*() const { return reinterpret_cast<void*>(structure); }
        operator unsigned char*() const { return reinterpret_cast<unsigned char*>(structure); }        
        operator std::uint8_t*() const { return reinterpret_cast<std::uint8_t*>(structure); }

        operator const StructureType*() const { return const_cast<const StructureType*>(structure); }
        operator const char*() const { return reinterpret_cast<const char*>(structure); }
        operator const void*() const { return reinterpret_cast<const void*>(structure); }
        operator const unsigned char*() const { return reinterpret_cast<const unsigned char*>(structure); }
        operator const std::uint8_t*() const { return reinterpret_cast<const std::uint8_t*>(structure); }

    };

    using PiFirmwareVolumeProxy                         = StructureProxy<EFI_FIRMWARE_VOLUME_HEADER>;
    using PiFirmwareVolumeExtensionHeaderProxy          = StructureProxy<EFI_FIRMWARE_VOLUME_EXT_HEADER>;
    using PiFirmwareVolumeExtensionEntryProxy           = StructureProxy<EFI_FIRMWARE_VOLUME_EXT_ENTRY>;
    using PiFirmwareVolumeExtensionEntryOemTypeProxy    = StructureProxy<EFI_FIRMWARE_VOLUME_EXT_ENTRY_OEM_TYPE>;
    using PiFirmwareVolumeExtensionEntryGuidTypeProxy   = StructureProxy<EFI_FIRMWARE_VOLUME_EXT_ENTRY_GUID_TYPE>;

    using PiFileProxy                       = StructureProxy<EFI_FFS_FILE_HEADER>;
    using PiExtendedFileProxy               = StructureProxy<EFI_FFS_FILE_HEADER2>;

    using PiSectionProxy                    = StructureProxy<EFI_COMMON_SECTION_HEADER>;
    using PiExtendedSectionProxy            = StructureProxy<EFI_COMMON_SECTION_HEADER2>;

    using PiCompatibility16SectionProxy     = StructureProxy<EFI_COMPATIBILITY16_SECTION>;
    using PiCompressionSectionProxy         = StructureProxy<EFI_COMPRESSION_SECTION>;
    using PiDisposableSectionProxy          = StructureProxy<EFI_DISPOSABLE_SECTION>;
    using PiDxeDepexSectionProxy            = StructureProxy<EFI_DXE_DEPEX_SECTION>;
    using PiFirmwareVolumeImageSectionProxy = StructureProxy<EFI_FIRMWARE_VOLUME_IMAGE_SECTION>;
    using PiFreeformSubtypeGuidSectionProxy = StructureProxy<EFI_FREEFORM_SUBTYPE_GUID_SECTION>;
    using PiGuidDefinedSectionProxy         = StructureProxy<EFI_GUID_DEFINED_SECTION>;
    using PiPe32SectionProxy                = StructureProxy<EFI_PE32_SECTION>;
    using PiPeiDepexSectionProxy            = StructureProxy<EFI_PEI_DEPEX_SECTION>;
    using PiPicSectionProxy                 = StructureProxy<EFI_PIC_SECTION>;
    using PiTeSectionProxy                  = StructureProxy<EFI_TE_SECTION>;
    using PiRawSectionProxy                 = StructureProxy<EFI_RAW_SECTION>;
    using PiSmmDepexSectionProxy            = StructureProxy<EFI_SMM_DEPEX_SECTION>;
    using PiUserInterfaceSectionProxy       = StructureProxy<EFI_USER_INTERFACE_SECTION>;
    using PiVersionSectionProxy             = StructureProxy<EFI_VERSION_SECTION>;

    using PiCompatibility16Section2Proxy    = StructureProxy<EFI_COMPATIBILITY16_SECTION2>;
    using PiCompressionSection2Proxy        = StructureProxy<EFI_COMPRESSION_SECTION2>;
    using PiDisposableSection2Proxy         = StructureProxy<EFI_DISPOSABLE_SECTION2>;
    using PiDxeDepexSection2Proxy           = StructureProxy<EFI_DXE_DEPEX_SECTION2>;
    using PiFirmwareVolumeImageSection2Proxy= StructureProxy<EFI_FIRMWARE_VOLUME_IMAGE_SECTION2>;
    using PiFreeformSubtypeGuidSection2Proxy= StructureProxy<EFI_FREEFORM_SUBTYPE_GUID_SECTION2>;
    using PiGuidDefinedSection2Proxy        = StructureProxy<EFI_GUID_DEFINED_SECTION2>;
    using PiPe32Section2Proxy               = StructureProxy<EFI_PE32_SECTION2>;
    using PiPeiDepexSection2Proxy           = StructureProxy<EFI_PEI_DEPEX_SECTION2>;
    using PiPicSection2Proxy                = StructureProxy<EFI_PIC_SECTION2>;
    using PiTeSection2Proxy                 = StructureProxy<EFI_TE_SECTION2>;
    using PiRawSection2Proxy                = StructureProxy<EFI_RAW_SECTION2>;
    using PiSmmDepexSection2Proxy           = StructureProxy<EFI_SMM_DEPEX_SECTION2>;
    using PiUserInterfaceSection2Proxy      = StructureProxy<EFI_USER_INTERFACE_SECTION2>;
    using PiVersionSection2Proxy            = StructureProxy<EFI_VERSION_SECTION2>;
}

namespace Offsets {

    struct PiFVHearedOffsets {
        std::size_t 
            ZeroVector,
            FileSystemGuid,
            FvLength,
            Signature,
            Attributes,
            HeaderLength,
            Checksum,
            ExtHeaderOffset,
            Reserved,
            Revision,
            BlockMap;
        
        constexpr PiFVHearedOffsets() :
            ZeroVector(offsetof(EFI_FIRMWARE_VOLUME_HEADER, ZeroVector)),
            FileSystemGuid(offsetof(EFI_FIRMWARE_VOLUME_HEADER, FileSystemGuid)),
            FvLength(offsetof(EFI_FIRMWARE_VOLUME_HEADER, FvLength)),
            Signature(offsetof(EFI_FIRMWARE_VOLUME_HEADER, Signature)),
            Attributes(offsetof(EFI_FIRMWARE_VOLUME_HEADER, Attributes)),
            HeaderLength(offsetof(EFI_FIRMWARE_VOLUME_HEADER, HeaderLength)),
            Checksum(offsetof(EFI_FIRMWARE_VOLUME_HEADER, Checksum)),
            ExtHeaderOffset(offsetof(EFI_FIRMWARE_VOLUME_HEADER, ExtHeaderOffset)),
            Reserved(offsetof(EFI_FIRMWARE_VOLUME_HEADER, Reserved)),
            Revision(offsetof(EFI_FIRMWARE_VOLUME_HEADER, Revision)),
            BlockMap(offsetof(EFI_FIRMWARE_VOLUME_HEADER, BlockMap)) {}
    };

    struct PiFileHeaderOffsets {
        std::size_t 
            Name,
            IntegrityCheck,
            Type,
            Attributes,
            Size,
            State,
            ExtendedSize;

        constexpr PiFileHeaderOffsets() :
            Name(offsetof(EFI_FFS_FILE_HEADER, Name)),
            IntegrityCheck(offsetof(EFI_FFS_FILE_HEADER, IntegrityCheck)),
            Type(offsetof(EFI_FFS_FILE_HEADER, Type)),
            Attributes(offsetof(EFI_FFS_FILE_HEADER, Attributes)),
            Size(offsetof(EFI_FFS_FILE_HEADER, Size)),
            State(offsetof(EFI_FFS_FILE_HEADER, State)),
            ExtendedSize(offsetof(EFI_FFS_FILE_HEADER2, ExtendedSize)) {}
    };

    struct PiSectionHeaderOffsets {
        std::size_t
            Size,
            Type,
            ExtendedSize;
        
        constexpr PiSectionHeaderOffsets() :
            Size(offsetof(EFI_COMMON_SECTION_HEADER, Size)),
            Type(offsetof(EFI_COMMON_SECTION_HEADER, Type)),
            ExtendedSize(offsetof(EFI_COMMON_SECTION_HEADER2, ExtendedSize)) {}
    };
}