/// PROJECT
#include "DecompressionModule.hpp"

namespace Project
{

	namespace Decompression
	{

		std::unique_ptr<BaseDecompresser> AllocDecompresser(Decompresser::Decompresser_t type)
		{
			switch (type)
			{
				case Project::Decompression::Decompresser::TianoEdk		:	return std::make_unique<TianoEdkDecompressor>();
				case Project::Decompression::Decompresser::TianoEdk2	:	return std::make_unique<TianoEdk2Decompressor>();
				case Project::Decompression::Decompresser::Lzma			:	return std::make_unique<LzmaDecompressor>();
				case Project::Decompression::Decompresser::Lzma2		:	return std::make_unique<Lzma2Decompressor>();
				case Project::Decompression::Decompresser::Lzma86		:	return std::make_unique<Lzma86Decompressor>();
				case Project::Decompression::Decompresser::Unknown		:
				default	: {
					DEBUG_ERROR_MESSAGE
						DEBUG_PRINT("\tMessage: Attempt to allocate decompresser with undefined type.");
						DEBUG_PRINT("\tProvided type value: ", type);
					DEBUG_END_MESSAGE
					return nullptr;
				}
			}
		}

	}

}