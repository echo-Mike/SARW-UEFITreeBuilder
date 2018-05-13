#pragma once
#ifndef FORMATED_PRINT_HPP__
#define	FORMATED_PRINT_HPP__ "0.0.0@FormatedPrint.hpp"

/// STD
#include <cstdio>

#define PROJ_UINT64_FORMAT(value) "%llu (%#.16llX)", (value), (value)
#define PROJ_UINT32_FORMAT(value) "%u (%#.8X)", (value), (value)
#define PROJ_UINT16_FORMAT(value) "%hu (%#.4hX)", (value), (value)
#define PROJ_UINT8_FORMAT(value) "%hhu (%#.2hhX)", (value), (value)

#define PROJ_INT64_FORMAT(value) "%lld (%#.16llX)", (value), (value)
#define PROJ_INT32_FORMAT(value) "%d (%#.8X)", (value), (value)
#define PROJ_INT16_FORMAT(value) "%hd (%#.4hX)", (value), (value)
#define PROJ_INT8_FORMAT(value) "%hhd (%#.2hhX)", (value), (value)

#define PROJ_GUID_FORMAT(...) "%.8X-%.4hX-%.4hX-%.4hX-%.12llX", __VA_ARGS__

#define PROJ_16BYTE_FORMAT(...) "%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX", __VA_ARGS__

#define PROJ_SNPRINTF(buffer, format, ...) ::std::snprintf(buffer, sizeof(buffer), format( __VA_ARGS__ ) )

#endif