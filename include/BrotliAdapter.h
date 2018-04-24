#pragma once
#ifndef BROTLI_ADAPTER_H__
#define BROTLI_ADAPTER_H__ "0.0.0@BrotliAdapter.h"

/// PROJECT
#include "ProjectGlobalDefines.h"

PROJ_EXTERN_C

/// UEFI
#include <Uefi/UefiBaseType.h>

#ifndef _SIZE_T_DEFINED
typedef UINTN size_t;
#	define _SIZE_T_DEFINED
#endif 

#ifndef NULL
#	define NULL (0)
#endif 

// UEFI/Library/DebugLib.h
#ifndef ASSERT
#	define ASSERT(expression)
#endif

VOID* 
EFIAPI 
proj_brotli_memcpy(
	VOID* dest, 
	CONST VOID* src, 
	UINTN length
);

VOID*
EFIAPI
proj_brotli_memmove(
	VOID* dest,
	CONST VOID* src,
	UINTN length
);

VOID*
EFIAPI
proj_brotli_memset(
	VOID* buffer,
	UINTN length,
	UINT8 value
);

PROJ_EXTERN_C_END

#endif