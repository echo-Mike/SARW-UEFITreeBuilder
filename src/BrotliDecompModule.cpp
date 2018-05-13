/// PROJECT
#include "DecompressionModule.hpp"
#include "BrotliAdapter.h"

namespace Project
{

	namespace Decompression
	{

		EFI_STATUS BrotliDecompresser::GetInfo(
			Types::const_void_ptr_t Source,
			Types::length_t SrcSize,
			Types::length_t& DstSize
		)
		{
			UINT32 DstSize_ = 0, ScratchSize_ = 0, SrcSize_ = SrcSize;
			auto status = proj_brotli_GetInfo(
				Source,
				SrcSize_,
				&DstSize_,
				&ScratchSize_
			);

			if (status == EFI_SUCCESS) {
				DstSize = DstSize_;
				return EFI_SUCCESS;
			} else {
				DstSize = 0;
				return EFI_INVALID_PARAMETER;
			}

		}

		EFI_STATUS BrotliDecompresser::Decompress(
			Types::const_void_ptr_t Source,
			Types::length_t SrcSize,
			Types::unique_byte_buff_t& Destination
		)
		{
			UINT32 DstSize_ = 0, ScratchSize_ = 0, SrcSize_ = SrcSize;
			auto status = proj_brotli_GetInfo(
				Source,
				SrcSize_,
				&DstSize_,
				&ScratchSize_
			);

			if (status != EFI_SUCCESS) {
				return EFI_INVALID_PARAMETER;
			}

			Types::unique_byte_buff_t scratchBuff = std::make_unique<Types::unique_byte_buff_t::element_type[]>(ScratchSize_);
			Destination = std::make_unique<Types::unique_byte_buff_t::element_type[]>(DstSize_);

			status = proj_brotli_Decompress(
				Source,
				SrcSize_,
				Destination.get(),
				scratchBuff.get()
			);

			return status;
		}
	}

}