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
extern "C" 
{
	#include <Base.h>
	#include <PiPei.h>
	#include <PiDxe.h>
	#include <Uefi.h>
}

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

		Types::hash_t hash(Types::const_pointer_t ptr, Types::length_t length);
	}
}

#endif