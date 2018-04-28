#pragma once
#ifndef BROTLI_ADAPTER_H__
#define BROTLI_ADAPTER_H__ "0.0.0@BrotliAdapter.h"

/// PROJECT
#include "ProjectGlobalDefines.h"

PROJ_EXTERN_C

/// UEFI
#include <Uefi/UefiBaseType.h>

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

EFI_STATUS
EFIAPI
proj_brotli_GetInfo(
	IN  CONST VOID  *Source,
	IN  UINT32      SourceSize,
	OUT UINT32      *DestinationSize,
	OUT UINT32      *ScratchSize
);

EFI_STATUS
EFIAPI
proj_brotli_Decompress(
	IN CONST VOID  *Source,
	IN UINTN       SourceSize,
	IN OUT VOID    *Destination,
	IN OUT VOID    *Scratch
);

PROJ_EXTERN_C_END

#endif