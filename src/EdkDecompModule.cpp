/// PROJECT
#include "DecompressionModule.hpp"

/// TIANO
#include <Tiano/EdkDecompress.h>

namespace Project
{

	namespace Decompression
	{

		EFI_STATUS TianoEdkDecompresser::GetInfo(
			Types::void_ptr_t Source,
			Types::length_t SrcSize,
			Types::length_t& DstSize
		)
		{
			if (SrcSize > MAX_UINT32) {
				return EFI_INVALID_PARAMETER;
			}
			UINT32 ScratchSize = 0, DstSize_ = 0;
			auto status = TianoGetInfo(Source, static_cast<UINT32>(SrcSize), &DstSize_, &ScratchSize);
			DstSize = DstSize_;
			return status;
		}

		EFI_STATUS TianoEdkDecompresser::Decompress(
			Types::void_ptr_t Source,
			Types::length_t SrcSize,
			Types::unique_byte_buff_t& Destination
		)
		{
			if (SrcSize > MAX_UINT32) {
				return EFI_INVALID_PARAMETER;
			}
			UINT32 ScratchSize = 0, DstSize = 0;
			auto status = TianoGetInfo(Source, static_cast<UINT32>(SrcSize), &DstSize, &ScratchSize);
			if (status != EFI_SUCCESS) {
				return status;
			}
			Types::unique_byte_buff_t scratchBuff = std::make_unique<Types::unique_byte_buff_t::element_type[]>(ScratchSize);
			Destination = std::make_unique<Types::unique_byte_buff_t::element_type[]>(DstSize);
			status = TianoDecompress(
				Source,
				static_cast<UINT32>(SrcSize),
				Destination.get(),
				DstSize,
				scratchBuff.get(),
				ScratchSize
			);
			if (status != EFI_SUCCESS) {
				Destination.reset(nullptr);
			}
			return status;
		}


		EFI_STATUS TianoEfiDecompresser::GetInfo(
			Types::void_ptr_t Source,
			Types::length_t SrcSize,
			Types::length_t& DstSize
		)
		{
			if (SrcSize > MAX_UINT32) {
				return EFI_INVALID_PARAMETER;
			}
			UINT32 ScratchSize = 0, DstSize_ = 0;
			auto status = EfiGetInfo(Source, static_cast<UINT32>(SrcSize), &DstSize_, &ScratchSize);
			DstSize = DstSize_;
			return status;
		}

		EFI_STATUS TianoEfiDecompresser::Decompress(
			Types::void_ptr_t Source,
			Types::length_t SrcSize,
			Types::unique_byte_buff_t& Destination
		)
		{
			if (SrcSize > MAX_UINT32) {
				return EFI_INVALID_PARAMETER;
			}
			UINT32 ScratchSize = 0, DstSize = 0;
			auto status = EfiGetInfo(Source, static_cast<UINT32>(SrcSize), &DstSize, &ScratchSize);
			if (status != EFI_SUCCESS) {
				return status;
			}
			Types::unique_byte_buff_t scratchBuff = std::make_unique<Types::unique_byte_buff_t::element_type[]>(ScratchSize);
			Destination = std::make_unique<Types::unique_byte_buff_t::element_type[]>(DstSize);
			status = EfiDecompress(
				Source,
				static_cast<UINT32>(SrcSize),
				Destination.get(),
				DstSize,
				scratchBuff.get(),
				ScratchSize
			);
			if (status != EFI_SUCCESS) {
				Destination.reset(nullptr);
			}
			return status;
		}

	}

}