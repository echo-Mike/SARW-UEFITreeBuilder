#pragma once
#ifndef PROJECT_GLOBAL_DEFINES_H__
#define PROJECT_GLOBAL_DEFINES_H__ "0.0.0@ProjectGlobalDefines.h"

#ifdef __cplusplus
#	define PROJ_EXTERN_C \
extern "C" \
{
#	define PROJ_EXTERN_C_END \
}
#define PROJ_CPP_CONTEXT
#else
#	define PROJ_EXTERN_C
#	define PROJ_EXTERN_C_END
#endif

PROJ_EXTERN_C

int proj_is_big_endian(void);

void proj_get_uint_n(const void* src, void* dest, unsigned int n);
void proj_get_uint16_t(const void* src, void* dest);
void proj_get_uint32_t(const void* src, void* dest);
void proj_get_uint64_t(const void* src, void* dest);

void proj_get_rev_uint_n(const void* src, void* dest, unsigned int n);
void proj_get_rev_uint16_t(const void* src, void* dest);
void proj_get_rev_uint32_t(const void* src, void* dest);
void proj_get_rev_uint64_t(const void* src, void* dest);

PROJ_EXTERN_C_END

#define PROJ_Rsa2048Sha256_NOT_IMPLEMENTED

#endif