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
        void *begin, *end; 
        std::size_t length;

        explicit MemoryProxy(void* begin_ = nullptr, void* end_ = nullptr) :
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

        void computePayloadLength() {
            if (begin && end) {
                auto *begin_ = reinterpret_cast<std::uint8_t*>(begin), 
                     *end_ = reinterpret_cast<std::uint8_t*>(end);
                if (begin_ > end_) std::swap(begin_, end_);
                length = end_ - begin_;
            }
        }
    };

    struct PiFirmwareVolumeProxy : public MemoryProxy {
        using HeaderType = EFI_FIRMWARE_VOLUME_HEADER;
        HeaderType* header = nullptr;
        std::size_t headerLength = sizeof(HeaderType);

        PiFirmwareVolumeProxy(const HeaderType* ptr_) : MemoryProxy() 
        {
            header = const_cast<HeaderType*>(ptr_);
            begin = header;
            headerLength = sizeof(HeaderType);
        }

        PiFirmwareVolumeProxy(const char* ptr_) : 
            PiFirmwareVolumeProxy(reinterpret_cast<const HeaderType*>(ptr_)) {}

        PiFirmwareVolumeProxy(const std::uint8_t* ptr_) : 
            PiFirmwareVolumeProxy(reinterpret_cast<const HeaderType*>(ptr_)) {}

        PiFirmwareVolumeProxy(const void* ptr_) : 
            PiFirmwareVolumeProxy(reinterpret_cast<const HeaderType*>(ptr_)) {}
        
        HeaderType* operator->() const { return header; }

        operator HeaderType*() const { return header; }
        operator char*() const { return reinterpret_cast<char*>(header); }
        operator void*() const { return reinterpret_cast<void*>(header); }
        operator unsigned char*() const { return reinterpret_cast<unsigned char*>(header); }        
        operator std::uint8_t*() const { return reinterpret_cast<std::uint8_t*>(header); }

        operator const HeaderType*() const { return const_cast<const HeaderType*>(header); }
        operator const char*() const { return reinterpret_cast<const char*>(header); }
        operator const void*() const { return reinterpret_cast<const void*>(header); }
        operator const unsigned char*() const { return reinterpret_cast<const unsigned char*>(header); }
        operator const std::uint8_t*() const { return reinterpret_cast<const std::uint8_t*>(header); }

        struct Offsets {
            const std::size_t 
            ZeroVector = offsetof(EFI_FIRMWARE_VOLUME_HEADER, ZeroVector),
            FileSystemGuid = offsetof(EFI_FIRMWARE_VOLUME_HEADER, FileSystemGuid),
            FvLength = offsetof(EFI_FIRMWARE_VOLUME_HEADER, FvLength),
            Signature = offsetof(EFI_FIRMWARE_VOLUME_HEADER, Signature),
            Attributes = offsetof(EFI_FIRMWARE_VOLUME_HEADER, Attributes),
            HeaderLength = offsetof(EFI_FIRMWARE_VOLUME_HEADER, HeaderLength),
            Checksum = offsetof(EFI_FIRMWARE_VOLUME_HEADER, Checksum),
            ExtHeaderOffset = offsetof(EFI_FIRMWARE_VOLUME_HEADER, ExtHeaderOffset),
            Reserved = offsetof(EFI_FIRMWARE_VOLUME_HEADER, Reserved),
            Revision = offsetof(EFI_FIRMWARE_VOLUME_HEADER, Revision),
            BlockMap = offsetof(EFI_FIRMWARE_VOLUME_HEADER, BlockMap);

            static const Offsets& getOffset() {
                static const Offsets offsets;
                return offsets;
            }
        private:
            Offsets() = default;
        };

        inline static const Offsets& getOffset() { return Offsets::getOffset(); }
    };

    template <typename PiFileHeaderT>
    struct PiFileProxy_ : public MemoryProxy {
        using HeaderType = PiFileHeaderT;
        HeaderType* header = nullptr;
        std::size_t headerLength = sizeof(HeaderType);

        PiFileProxy_(const HeaderType* ptr_) : MemoryProxy() 
        {
            header = const_cast<HeaderType*>(ptr_);
            begin = header;
            headerLength = sizeof(HeaderType);
        }

        PiFileProxy_(const char* ptr_) : 
            PiFileProxy_(reinterpret_cast<const HeaderType*>(ptr_)) {}

        PiFileProxy_(const std::uint8_t* ptr_) : 
            PiFileProxy_(reinterpret_cast<const HeaderType*>(ptr_)) {}

        PiFileProxy_(const void* ptr_) : 
            PiFileProxy_(reinterpret_cast<const HeaderType*>(ptr_)) {}

        HeaderType* operator->() const { return header; }

        operator HeaderType*() const { return header; }
        operator char*() const { return reinterpret_cast<char*>(header); }
        operator void*() const { return reinterpret_cast<void*>(header); }
        operator unsigned char*() const { return reinterpret_cast<unsigned char*>(header); }
        operator std::uint8_t*() const { return reinterpret_cast<std::uint8_t*>(header); }

        operator const HeaderType*() const { return const_cast<const HeaderType*>(header); }
        operator const char*() const { return reinterpret_cast<const char*>(header); }
        operator const void*() const { return reinterpret_cast<const void*>(header); }
        operator const unsigned char*() const { return reinterpret_cast<const unsigned char*>(header); }
        operator const std::uint8_t*() const { return reinterpret_cast<const std::uint8_t*>(header); }

        struct Offsets {
            const std::size_t 
            Name = offsetof(EFI_FFS_FILE_HEADER, Name),
            IntegrityCheck = offsetof(EFI_FFS_FILE_HEADER, IntegrityCheck),
            Type = offsetof(EFI_FFS_FILE_HEADER, Type),
            Attributes = offsetof(EFI_FFS_FILE_HEADER, Attributes),
            Size = offsetof(EFI_FFS_FILE_HEADER, Size),
            State = offsetof(EFI_FFS_FILE_HEADER, State),
            ExtendedSize = offsetof(EFI_FFS_FILE_HEADER2, ExtendedSize);

            static const Offsets& getOffset() {
                static const Offsets offsets;
                return offsets;
            }
        private:
            Offsets() = default;
        };

        inline static const Offsets& getOffset() { return Offsets::getOffset(); }
    };

    using PiFileProxy = PiFileProxy_<EFI_FFS_FILE_HEADER>;
    using PiExtendedFileProxy = PiFileProxy_<EFI_FFS_FILE_HEADER2>;
}
