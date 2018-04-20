#pragma once
#ifndef GENERAL_HPP__
#define	GENERAL_HPP__ "0.0.0@General.hpp"

/// STD
#include <cstdlib>
#include <cstddef>
#include <string>

/// SNIPPETS
#include <DebugLib/mDebugLib.hpp>

/// UEFI HEADERS
#include "UefiHeaders.h"

/// PROJECT
#include "ProjectTypes.hpp"
#include "ExitCodes.hpp"
#include "SignatureCasts.hpp"

namespace Project
{
	void printArgs(int argc, char* argv[]);

	namespace Checksums
	{

		std::uint16_t calc16(Types::const_pointer_t ptr, Types::length_t length);

		std::uint8_t calc8(Types::const_pointer_t ptr, Types::length_t length);

		Types::hash_t hash(Types::const_pointer_t ptr, Types::length_t length);

		std::uint32_t crc32(Types::const_pointer_t ptr, Types::length_t length, std::uint32_t init);
	}
}

#define PROJ_1B    ( 1ul )
#define PROJ_2B    ( 1ul << 1 )
#define PROJ_4B    ( 1ul << 2 )
#define PROJ_8B    ( 1ul << 3 )
#define PROJ_16B   ( 1ul << 4 )
#define PROJ_32B   ( 1ul << 5 )
#define PROJ_64B   ( 1ul << 6 )
#define PROJ_128B  ( 1ul << 7 )
#define PROJ_256B  ( 1ul << 8 )
#define PROJ_512B  ( 1ul << 9 )
#define PROJ_1KB   ( 1ul << 10 )
#define PROJ_2KB   ( 1ul << 11 )
#define PROJ_4KB   ( 1ul << 12 )
#define PROJ_8KB   ( 1ul << 13 )
#define PROJ_16KB  ( 1ul << 14 )
#define PROJ_32KB  ( 1ul << 15 )
#define PROJ_64KB  ( 1ul << 16 )
#define PROJ_128KB ( 1ul << 17 )
#define PROJ_256KB ( 1ul << 18 )
#define PROJ_512KB ( 1ul << 19 )
#define PROJ_1MB   ( 1ul << 20 )
#define PROJ_2MB   ( 1ul << 21 )
#define PROJ_4MB   ( 1ul << 22 )
#define PROJ_8MB   ( 1ul << 23 )
#define PROJ_16MB  ( 1ul << 24 )

#define PROJ_MAX_FFS2_SIZE ( PROJ_16MB - 1 )

#define ALIGN_(Value, Type, powOf2) ( ( (Value) + (Type)(powOf2 - 1) ) & ~( (Type)(powOf2 - 1) ) )
#define ALIGN2(Value, Type) ALIGN_(Value, Type, 2)
#define ALIGN4(Value, Type) ALIGN_(Value, Type, 4)
#define ALIGN8(Value, Type) ALIGN_(Value, Type, 8)
#define ALIGN16(Value, Type) ALIGN_(Value, Type, 16)
#define ALIGN32(Value, Type) ALIGN_(Value, Type, 32)
#define ALIGN64(Value, Type) ALIGN_(Value, Type, 64)
#define ALIGN128(Value, Type) ALIGN_(Value, Type, 128)
#define ALIGN256(Value, Type) ALIGN_(Value, Type, 256)
#define ALIGN512(Value, Type) ALIGN_(Value, Type, 512)
#define ALIGN1024(Value, Type) ALIGN_(Value, Type, 1024)

#ifndef UnifyPtrCast
#	define UnifyPtrCast(Ptr) (reinterpret_cast<::Project::Types::const_pointer_t>((Ptr)))
#endif

#define ALIGNPTR_(BasePtr, Ptr, powOf2) ( UnifyPtrCast((BasePtr)) + ALIGN_( ( UnifyPtrCast((Ptr)) - UnifyPtrCast((BasePtr)) ), std::size_t, powOf2 ) )
#define ALIGN_PTR4(BasePtr, Ptr) ALIGNPTR_(BasePtr, Ptr, 4)
#define ALIGN_PTR8(BasePtr, Ptr) ALIGNPTR_(BasePtr, Ptr, 8)

#define ALIGN_PTR_T_(BasePtr, Ptr, Type, powOf2) reinterpret_cast<Type>( ALIGNPTR_( (BasePtr), (Ptr), (powOf2) ) )
#define ALIGN_PTR_T4(BasePtr, Ptr, Type) ALIGN_PTR_T_(BasePtr, Ptr, Type, 4)
#define ALIGN_PTR_T8(BasePtr, Ptr, Type) ALIGN_PTR_T_(BasePtr, Ptr, Type, 8)

#define ADVANCE_PTR_(Ptr, Type, Count) reinterpret_cast<Type>( UnifyPtrCast((Ptr)) + (Count) )
#define ADVANCE_PTR4(Ptr, Type) ADVANCE_PTR_((Ptr), Type, 4)
#define ADVANCE_PTR8(Ptr, Type) ADVANCE_PTR_((Ptr), Type, 8)
#endif