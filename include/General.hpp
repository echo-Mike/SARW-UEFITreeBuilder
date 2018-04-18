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
	}
}

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

#define UnifyPtrCast(Ptr) (reinterpret_cast<::Project::Types::const_pointer_t>((Ptr)))

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