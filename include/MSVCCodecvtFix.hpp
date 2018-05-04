#pragma once
#ifndef MSVC_CODECVT_FIX_HPP__
#define MSVC_CODECVT_FIX_HPP__ "0.0.0@MSVCCodecvtFix.hpp"

/// STD
#include <codecvt>
#include <locale>
#include <string>

/**
 *    EXTERNAL CODE USAGE
 *    SOURCE:
 *        https://stackoverflow.com/a/35103224/7920874
 *    AUTHOR:
 *        https://stackoverflow.com/users/754534/jpnotadragon
 *    REASON:
 *        https://social.msdn.microsoft.com/Forums/en-US/8f40dcd8-c67f-4eba-9134-a19b9178e481/vs-2015-rc-linker-stdcodecvt-error?forum=vcgeneral
 */

#if _MSC_VER >= 1900

inline std::string utf16_to_utf8(std::u16string utf16_string)
{
	std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
	auto p = reinterpret_cast<const int16_t *>(utf16_string.data());
	return convert.to_bytes(p, p + utf16_string.size());
}

#else

inline std::string utf16_to_utf8(std::u16string utf16_string)
{
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
	return convert.to_bytes(utf16_string);
}

#endif

/**
 *    EXTERNAL CODE USAGE END
 */

#endif // !MSVC_CODECVT_FIX_HPP__