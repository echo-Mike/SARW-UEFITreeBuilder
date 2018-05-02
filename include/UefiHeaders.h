#pragma once
#ifndef UEFI_HEADERS_H__
#define	UEFI_HEADERS_H__ "0.0.0@UefiHeaders.h"

/// PROJECT
#include "ProjectGlobalDefines.h"

#if defined(_MSC_VER)
#	pragma warning( push )  
// For Base.h lines 54-64
#	pragma warning( disable : 4804 )
#endif

PROJ_EXTERN_C

/// UEFI HEADERS
#include <Base.h>
#include <PiPei.h>
#include <PiDxe.h>
#include <Uefi.h>

PROJ_EXTERN_C_END

#if defined(_MSC_VER)
#	pragma warning( pop )  
#endif

#endif