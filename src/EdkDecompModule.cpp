/// PROJECT
#include "DecompressionModule.hpp"

/// TIANO
extern "C"
{
	#include <Tiano/Edk/Decompress.h>
}


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

		}

		EFI_STATUS TianoEdkDecompresser::Decompress(
			Types::void_ptr_t Source,
			Types::length_t SrcSize,
			Types::unique_buffer_t& Destination
		)
		{

		}

	}

}