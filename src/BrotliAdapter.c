/// STD
#include <string.h>
/// PROJECT
#include "BrotliAdapter.h"

VOID* 
EFIAPI 
proj_brotli_memcpy(
	VOID* dest,
	CONST VOID* src,
	UINTN length
)
{
	return memcpy(dest, src, length);
}

VOID*
EFIAPI
proj_brotli_memmove(
	VOID* dest,
	CONST VOID* src,
	UINTN length
)
{
	return memmove(dest, src, length);
}

VOID*
EFIAPI
proj_brotli_memset(
	VOID* buffer,
	UINTN length,
	UINT8 value
)
{
	return memset(buffer, value, length);
}