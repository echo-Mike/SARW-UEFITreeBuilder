/// PROJECT
#include <Brotli/BrotliDecompressLibInternal.h>

VOID* 
EFIAPI 
proj_brotli_memcpy(
	VOID* dest,
	CONST VOID* src,
	UINTN length
)
{
	for (UINTN i = 0; i < length; ++i) {
		*((UINT8*)dest + i) = *((const UINT8*)src + i);
	}
	return dest;
}

VOID*
EFIAPI
proj_brotli_memmove(
	VOID* dest,
	CONST VOID* src,
	UINTN length
)
{
	if (dest == src) return dest;
	if (dest < src)
		return proj_brotli_memcpy(dest, src, length);
	else {
		for (UINTN i = length; i != 0; --i) {
			*((UINT8*)dest + i - 1) = *((const UINT8*)src + i - 1);
		}
	}
	return dest;
}

VOID*
EFIAPI
proj_brotli_memset(
	VOID* buffer,
	UINTN length,
	UINT8 value
)
{
	for (UINTN i = 0; i < length; ++i) {
		*((UINT8*)buffer + i) = value;
	}
	return buffer;
}

EFI_STATUS
EFIAPI
proj_brotli_GetInfo(
	IN  CONST VOID  *Source,
	IN  UINT32      SourceSize,
	OUT UINT32      *DestinationSize,
	OUT UINT32      *ScratchSize
)
{
	return BrotliUefiDecompressGetInfo(Source, SourceSize, DestinationSize, ScratchSize);
}

EFI_STATUS
EFIAPI
proj_brotli_Decompress(
	IN CONST VOID  *Source,
	IN UINTN       SourceSize,
	IN OUT VOID    *Destination,
	IN OUT VOID    *Scratch
)
{
	return BrotliUefiDecompress(Source, SourceSize, Destination, Scratch);
}