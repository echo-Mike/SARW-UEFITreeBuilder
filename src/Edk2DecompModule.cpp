/// STD
#include <algorithm>
/// PROJECT
#include "DecompressionModule.hpp"
/// TIANO
#include <Tiano/Edk2Decompress.h>

namespace Project
{

	namespace Decompression
	{

		EFI_STATUS TianoEdk2Decompresser::GetInfo(
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

		EFI_STATUS TianoEdk2Decompresser::Decompress(
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

			/**
			 *  During testing was found that non-Tiano compressed data may corrupt memory near scratch buffer during decompression.
			 *	The solution was to allocate a lot more (x5) memory for scratch buffer and detect memory corruption by hand.
			 */
			Types::unique_byte_buff_t scratchBuff = std::make_unique<Types::unique_byte_buff_t::element_type[]>(ScratchSize * 5);
			// Clear memory for scratch buffer
			std::memset(scratchBuff.get(), 0, ScratchSize * 5);
			Destination = std::make_unique<Types::unique_byte_buff_t::element_type[]>(DstSize);

			status = TianoDecompress(
				Source,
				static_cast<UINT32>(SrcSize),
				Destination.get(),
				DstSize,
				scratchBuff.get() + ScratchSize * 2, // Give some space before in case of invalid access
				ScratchSize
			);

			{	// Check memory corruption
				auto left  = std::count(scratchBuff.get(), scratchBuff.get() + ScratchSize * 2, 0),
					 right = std::count(scratchBuff.get() + ScratchSize * 3, scratchBuff.get() + ScratchSize * 5, 0);
				if (left != ScratchSize * 2 || right != ScratchSize * 2) DEBUG_WARNING_MESSAGE
					DEBUG_PRINT("\tMessage: Memory corruption have happened with scratch buffer.");
					DEBUG_PRINT("\tDecompresser: TIANO EDKII");
					DEBUG_PRINT("\tLeft: ", left, " : ", ScratchSize * 2);
					DEBUG_PRINT("\tRight: ", right, " : ", ScratchSize * 2);
				DEBUG_END_MESSAGE_AND_EVAL({
					Destination.reset(nullptr);
					return EFI_INVALID_PARAMETER; 
				})
			}

			if (status != EFI_SUCCESS) {
				Destination.reset(nullptr);
			}
			return status;
		}


		EFI_STATUS TianoEfi2Decompresser::GetInfo(
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

		EFI_STATUS TianoEfi2Decompresser::Decompress(
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