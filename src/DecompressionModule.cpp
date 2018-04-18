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
				case Project::Decompression::Decompresser::TianoEdk		:	return std::make_unique<TianoEdkDecompresser>();
				case Project::Decompression::Decompresser::TianoEfi		:	return std::make_unique<TianoEfiDecompresser>();
				case Project::Decompression::Decompresser::TianoEdk2	:	return std::make_unique<TianoEdk2Decompresser>();
				case Project::Decompression::Decompresser::TianoEfi2	:	return std::make_unique<TianoEfi2Decompresser>();
				case Project::Decompression::Decompresser::Lzma			:	return std::make_unique<LzmaDecompresser>();
				case Project::Decompression::Decompresser::Lzma2		:	return std::make_unique<Lzma2Decompresser>();
				case Project::Decompression::Decompresser::Lzma86		:	return std::make_unique<Lzma86Decompresser>();
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