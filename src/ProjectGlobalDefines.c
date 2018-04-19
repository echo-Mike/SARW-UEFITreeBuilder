/// STD
#include <stdint.h>
/// PROJECT
#include "ProjectGlobalDefines.h"

/**
 *	EXTERNAL CODE USAGE
 *	SOURCE : 
 *      https://stackoverflow.com/questions/1001307/detecting-endianness-programmatically-in-a-c-program
 *	AUTHOR :
 *      https://stackoverflow.com/users/11465/david-cournapeau
 *	PURPOSE :
 *      Check that machine compiling code using big-endian representation.
 */
int proj_is_big_endian(void)
{
	union {
		uint32_t i;
		uint8_t c[4];
	} bint = { 0x01020304 };
	return bint.c[0] == 1;
}
/**
 *	EXTERNAL CODE USAGE END
 */

void proj_get_uint_n(const void* src, void* dest, unsigned int n)
{
	if (n == 0) return;
	--n;
	for (; n != 0; --n) {
		*((uint8_t*)dest + n) = *((const uint8_t*)(src) + n);
	}
	*((uint8_t*)dest) = *((const uint8_t*)(src));
}

void proj_get_uint16_t(const void* src, void* dest)
{
	*((uint8_t*)dest + 0) = *((const uint8_t*)(src) + 0);
	*((uint8_t*)dest + 1) = *((const uint8_t*)(src) + 1);
}

void proj_get_uint32_t(const void* src, void* dest)
{
	*((uint8_t*)dest + 0) = *((const uint8_t*)(src) + 0);
	*((uint8_t*)dest + 1) = *((const uint8_t*)(src) + 1);
	*((uint8_t*)dest + 2) = *((const uint8_t*)(src) + 2);
	*((uint8_t*)dest + 3) = *((const uint8_t*)(src) + 3);
}

void proj_get_uint64_t(const void* src, void* dest)
{
	*((uint8_t*)dest + 0) = *((const uint8_t*)(src) + 0);
	*((uint8_t*)dest + 1) = *((const uint8_t*)(src) + 1);
	*((uint8_t*)dest + 2) = *((const uint8_t*)(src) + 2);
	*((uint8_t*)dest + 3) = *((const uint8_t*)(src) + 3);
	*((uint8_t*)dest + 4) = *((const uint8_t*)(src) + 4);
	*((uint8_t*)dest + 5) = *((const uint8_t*)(src) + 5);
	*((uint8_t*)dest + 6) = *((const uint8_t*)(src) + 6);
	*((uint8_t*)dest + 7) = *((const uint8_t*)(src) + 7);
}

void proj_get_rev_uint_n(const void* src, void* dest, unsigned int n)
{
	for (unsigned int i = 0; i < n; ++i) {
		*((uint8_t*)dest + i) = *((const uint8_t*)(src) + n - i - 1);
	}
}

void proj_get_rev_uint16_t(const void* src, void* dest)
{
	*((uint8_t*)dest + 0) = *((const uint8_t*)(src) + 1);
	*((uint8_t*)dest + 1) = *((const uint8_t*)(src) + 0);
}

void proj_get_rev_uint32_t(const void* src, void* dest)
{
	*((uint8_t*)dest + 0) = *((const uint8_t*)(src) + 3);
	*((uint8_t*)dest + 1) = *((const uint8_t*)(src) + 2);
	*((uint8_t*)dest + 2) = *((const uint8_t*)(src) + 1);
	*((uint8_t*)dest + 3) = *((const uint8_t*)(src) + 0);
}

void proj_get_rev_uint64_t(const void* src, void* dest)
{
	*((uint8_t*)dest + 0) = *((const uint8_t*)(src) + 7);
	*((uint8_t*)dest + 1) = *((const uint8_t*)(src) + 6);
	*((uint8_t*)dest + 2) = *((const uint8_t*)(src) + 5);
	*((uint8_t*)dest + 3) = *((const uint8_t*)(src) + 4);
	*((uint8_t*)dest + 4) = *((const uint8_t*)(src) + 3);
	*((uint8_t*)dest + 5) = *((const uint8_t*)(src) + 2);
	*((uint8_t*)dest + 6) = *((const uint8_t*)(src) + 1);
	*((uint8_t*)dest + 7) = *((const uint8_t*)(src) + 0);
}