#pragma once
#ifndef GENERAL_HPP__
#define	GENERAL_HPP__ "0.0.0@General.hpp"

/// STD
#include <cstdlib>
#include <cstddef>

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

#endif