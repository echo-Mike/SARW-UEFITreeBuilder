/// PROJECT
#include "DecompressionModule.hpp"

/// LZMA
#include <LZMA/LzmaDec.h>
#include <LZMA/Lzma2Dec.h>
#include <LZMA/Lzma86.h>

#define LZMA_HEADER_SIZE (LZMA_PROPS_SIZE + 8)

namespace Project
{

	namespace Decompression
	{

		namespace Helper
		{

			Types::length_t LzmaGetDecodedSizeOfBuf(Types::const_byte_ptr_t EncodedData)
			{
				Types::length_t DecodedSize = 0;
				// Parse header
				for (INT32 Index = LZMA_PROPS_SIZE + 7; Index >= LZMA_PROPS_SIZE; --Index) {
					DecodedSize <<= 8;
					DecodedSize += EncodedData[Index];
				}
				return DecodedSize;
			}

			static void * AllocForLzma(ISzAllocPtr p, size_t size) { (void)p; return malloc(size); }
			static void FreeForLzma(ISzAllocPtr p, void *address) { (void)p; free(address); }
			static ISzAlloc SzAllocForLzma = { &AllocForLzma, &FreeForLzma };
		}

		EFI_STATUS LzmaDecompresser::GetInfo(
			Types::void_ptr_t Source,
			Types::length_t SrcSize,
			Types::length_t& DstSize
		)
		{
			using namespace Helper;
			if (SrcSize < LZMA_HEADER_SIZE) {
				return EFI_INVALID_PARAMETER;
			}
			DstSize = LzmaGetDecodedSizeOfBuf(reinterpret_cast<Types::byte_ptr_t>(Source));
			return EFI_SUCCESS;

		}

		EFI_STATUS LzmaDecompresser::Decompress(
			Types::void_ptr_t Source,
			Types::length_t SrcSize,
			Types::unique_byte_buff_t& Destination
		)
		{
			using namespace Helper;

			ELzmaStatus LzmaStatus;
			SizeT       DecodedBufSize;
			SizeT       EncodedDataSize;

			DecodedBufSize = LzmaGetDecodedSizeOfBuf(reinterpret_cast<Types::byte_ptr_t>(Source));
			EncodedDataSize = static_cast<SizeT>(SrcSize - LZMA_HEADER_SIZE);

			auto dataBegin = reinterpret_cast<Types::const_byte_ptr_t>(Source);
			dataBegin += LZMA_HEADER_SIZE;

			Destination.reset(new Types::byte_t[DecodedBufSize]);

			auto status = LzmaDecode(
				reinterpret_cast<Byte *>(Destination.get()),
				&DecodedBufSize,
				dataBegin,
				&EncodedDataSize,
				reinterpret_cast<Byte *>(Source),
				LZMA_PROPS_SIZE,
				LZMA_FINISH_END,
				&LzmaStatus,
				&SzAllocForLzma
			);

			if (status == SZ_OK) {
				DEBUG_INFO_MESSAGE
					DEBUG_PRINT("\tMessage: LzmaDecode returned status.");
					switch (LzmaStatus)
					{
					case LZMA_STATUS_FINISHED_WITH_MARK :           DEBUG_PRINT("\tStatus: LZMA_STATUS_FINISHED_WITH_MARK"); break;
					case LZMA_STATUS_NOT_FINISHED :                 DEBUG_PRINT("\tStatus: LZMA_STATUS_NOT_FINISHED"); break;
					case LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK :  DEBUG_PRINT("\tStatus: LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK"); break;
					case LZMA_STATUS_NOT_SPECIFIED :                DEBUG_PRINT("\tUnexpected status: LZMA_STATUS_NOT_SPECIFIED"); break;
					case LZMA_STATUS_NEEDS_MORE_INPUT :             DEBUG_PRINT("\tUnexpected status: LZMA_STATUS_NEEDS_MORE_INPUT"); break;
					default:                                        DEBUG_PRINT("\tStatus: Unknown [ ", LzmaStatus, " ]"); break;
					}
				DEBUG_END_MESSAGE
			} else {
				DEBUG_WARNING_MESSAGE
					DEBUG_PRINT("\tMessage: LzmaDecode returned an error.");
					switch (status)
					{
					case SZ_ERROR_DATA:        DEBUG_PRINT("\tError: SZ_ERROR_DATA"); break;
					case SZ_ERROR_MEM:         DEBUG_PRINT("\tError: SZ_ERROR_MEM"); break;
					case SZ_ERROR_UNSUPPORTED: DEBUG_PRINT("\tError: SZ_ERROR_UNSUPPORTED"); break;
					case SZ_ERROR_INPUT_EOF:   DEBUG_PRINT("\tError: SZ_ERROR_INPUT_EOF"); break;
					default:                   DEBUG_PRINT("\tError: Unknown [ ", status, " ]"); break;
					}
				DEBUG_END_MESSAGE
			}

			return status == SZ_OK ? EFI_SUCCESS : EFI_INVALID_PARAMETER;
		}

		/*
		EFI_STATUS Lzma2Decompresser::GetInfo(
			Types::void_ptr_t Source,
			Types::length_t SrcSize,
			Types::length_t& DstSize
		)
		{

		}

		EFI_STATUS Lzma2Decompresser::Decompress(
			Types::void_ptr_t Source,
			Types::length_t SrcSize,
			Types::unique_byte_buff_t& Destination
		)
		{

		}
		//*/

		EFI_STATUS Lzma86Decompresser::GetInfo(
			Types::const_void_ptr_t Source,
			Types::length_t SrcSize,
			Types::length_t& DstSize
		)
		{
			UInt64 DstSize_ = 0;
			SizeT SrcSize_ = SrcSize;
			auto status = Lzma86_GetUnpackSize(
				reinterpret_cast<const Byte *>(Source),
				SrcSize_,
				&DstSize_
			);

			if (status == SZ_OK) {
				DstSize = DstSize_;
				return EFI_SUCCESS;
			} else {
				DstSize = 0;
				return EFI_INVALID_PARAMETER;
			}

		}

		EFI_STATUS Lzma86Decompresser::Decompress(
			Types::const_void_ptr_t Source,
			Types::length_t SrcSize,
			Types::unique_byte_buff_t& Destination
		)
		{
			UInt64 DstSize = 0;
			SizeT SrcSize_ = SrcSize;
			auto status = Lzma86_GetUnpackSize(
				reinterpret_cast<const Byte *>(Source),
				SrcSize_,
				&DstSize
			);

			if (status != SZ_OK) {
				return EFI_INVALID_PARAMETER;
			}

			Destination.reset(new Types::byte_t[DstSize]);

			status = Lzma86_Decode(
				reinterpret_cast<Byte *>(Destination.get()),
				&DstSize,
				reinterpret_cast<const Byte *>(Source),
				&SrcSize_
			);

			if (status != SZ_OK)
			{
				DEBUG_WARNING_MESSAGE
					DEBUG_PRINT("\tMessage: Lzma86_Decode returned an error.");
					switch (status) 
					{
						case SZ_ERROR_DATA :        DEBUG_PRINT("\tError: SZ_ERROR_DATA"); break;
						case SZ_ERROR_MEM :         DEBUG_PRINT("\tError: SZ_ERROR_MEM"); break;
						case SZ_ERROR_UNSUPPORTED : DEBUG_PRINT("\tError: SZ_ERROR_UNSUPPORTED"); break;
						case SZ_ERROR_INPUT_EOF :   DEBUG_PRINT("\tError: SZ_ERROR_INPUT_EOF"); break;
						default :                   DEBUG_PRINT("\tError: Unknown [ ", status, " ]"); break;
					}
				DEBUG_END_MESSAGE
			}

			return status == SZ_OK ? EFI_SUCCESS : EFI_INVALID_PARAMETER;
		}
	}

}