#pragma once
#ifndef DECOMPRESSION_MODULE_HPP__
#define	DECOMPRESSION_MODULE_HPP__ "0.0.0@DecompressionModule.hpp"

/// STD
#include <memory>

/// UEFI HEADERS
extern "C"
{
#include <Base.h>
#include <PiPei.h>
#include <PiDxe.h>
#include <Uefi.h>
}

/// SNIPPETS
#include <DebugLib/mDebugLib.hpp>
#include <ClassUtilsLib/mClassUtils.hpp>

/// PROJECT
#include "ProjectTypes.hpp"
#include "ExitCodes.hpp"

namespace Project
{

	namespace Types
	{

		typedef std::unique_ptr<Types::memory_t[]> unique_buffer_t;
	}

	namespace Decompression
	{

		namespace Decompresser
		{

			enum Decompresser_t
			{
				Unknown,
				TianoEdk,
				TianoEdk2,
				Lzma,
				Lzma2,
				Lzma86
			};

		}

		struct BaseDecompresser
		{

			BaseDecompresser(Decompresser::Decompresser_t type_ = Decompresser::Unknown) : type(type_) {}

			DefaultCopyableAndMovable(BaseDecompresser)

			virtual ~BaseDecompresser() {};

			virtual EFI_STATUS GetInfo(
				Types::void_ptr_t Source,
				Types::length_t SrcSize,
				Types::length_t& DstSize
			) = 0;

			virtual EFI_STATUS Decompress(
				Types::void_ptr_t Source,
				Types::length_t SrcSize,
				Types::unique_buffer_t& Destination
			) = 0;

			const Decompresser::Decompresser_t type;
		};

		struct TianoEdkDecompresser :
			public BaseDecompresser
		{

			typedef BaseDecompresser Base;

			TianoEdkDecompresser() : Base(Decompresser::TianoEdk) {}

			DefaultCopyableAndMovable(TianoEdkDecompresser)

			~TianoEdkDecompresser() {};

			EFI_STATUS GetInfo(
				Types::void_ptr_t Source,
				Types::length_t SrcSize,
				Types::length_t& DstSize
			);

			EFI_STATUS Decompress(
				Types::void_ptr_t Source,
				Types::length_t SrcSize,
				Types::unique_buffer_t& Destination
			);

		};

		struct TianoEdk2Decompresser :
			public BaseDecompresser
		{

			typedef BaseDecompresser Base;

			TianoEdk2Decompresser() : Base(Decompresser::TianoEdk2) {}

			DefaultCopyableAndMovable(TianoEdk2Decompresser)

			~TianoEdk2Decompresser() {};

			EFI_STATUS GetInfo(
				Types::void_ptr_t Source,
				Types::length_t SrcSize,
				Types::length_t& DstSize
			);

			EFI_STATUS Decompress(
				Types::void_ptr_t Source,
				Types::length_t SrcSize,
				Types::unique_buffer_t& Destination
			);

		};

		struct LzmaDecompresser :
			public BaseDecompresser
		{

			typedef BaseDecompresser Base;

			LzmaDecompresser() : Base(Decompresser::Lzma) {}

			DefaultCopyableAndMovable(LzmaDecompresser)

			 ~LzmaDecompresser() {};

			 EFI_STATUS GetInfo(
				Types::void_ptr_t Source,
				Types::length_t SrcSize,
				Types::length_t& DstSize
			);

			 EFI_STATUS Decompress(
				Types::void_ptr_t Source,
				Types::length_t SrcSize,
				 Types::unique_buffer_t& Destination
			);

		};

		struct Lzma2Decompresser :
			public BaseDecompresser
		{

			typedef BaseDecompresser Base;

			Lzma2Decompresser() : Base(Decompresser::Lzma2) {}

			DefaultCopyableAndMovable(Lzma2Decompresser)

			 ~Lzma2Decompresser() {};

			 EFI_STATUS GetInfo(
				Types::void_ptr_t Source,
				Types::length_t SrcSize,
				Types::length_t& DstSize
			);

			 EFI_STATUS Decompress(
				Types::void_ptr_t Source,
				Types::length_t SrcSize,
				 Types::unique_buffer_t& Destination
			);

		};

		struct Lzma86Decompresser :
			public BaseDecompresser
		{

			typedef BaseDecompresser Base;

			Lzma86Decompresser() : Base(Decompresser::Lzma86) {}

			DefaultCopyableAndMovable(Lzma86Decompresser)

			 ~Lzma86Decompresser() {};

			 EFI_STATUS GetInfo(
				Types::void_ptr_t Source,
				Types::length_t SrcSize,
				Types::length_t& DstSize
			);

			 EFI_STATUS Decompress(
				Types::void_ptr_t Source,
				Types::length_t SrcSize,
				Types::unique_buffer_t& Destination
			);

		};

		std::unique_ptr<BaseDecompresser> AllocDecompresser(Decompresser::Decompresser_t type);

	}

}

#endif