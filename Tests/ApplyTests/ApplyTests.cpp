#include <iostream>
#include <iomanip>

extern "C"
{
#include <Base.h>
#include <PiPei.h>
#include <PiDxe.h>
#include <Uefi.h>
#include "ProjectGlobalDefines.h"
}

namespace Project
{
	namespace Types
	{
		typedef const std::uint8_t* const_byte_ptr_t;
	}
}

// 7A9354D9-0468-444A-81CE-0BF617D890DF
#define EFI_FIRMWARE_FILE_SYSTEM_GUID \
    { 0x7a9354d9, 0x0468, 0x444a, { 0x81, 0xce, 0x0b, 0xf6, 0x17, 0xd8, 0x90, 0xdf } }

// 00000000-0000-0000-0000-000000000000
#define PROJ_UNKNOWN_GUID \
	{ 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }

// 00112233-4455-6677-8899-AABBCCDDEEFF
#define PROJ_SUPER_GUID \
	{ 0x00112233, 0x4455, 0x6677, { 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF } }

std::ostream& operator<<(std::ostream& out, const EFI_GUID& guid)
{
	char guidstr[64];
	std::uint32_t first;
	std::uint16_t middle1, middle2, middle3;
	std::uint64_t last = 0;
	auto ptr = reinterpret_cast<Project::Types::const_byte_ptr_t>(&guid);
	proj_get_uint32_t(ptr, &first);
	proj_get_uint16_t(ptr + 4, &middle1);
	proj_get_uint16_t(ptr + 6, &middle2);
	if (proj_is_big_endian()) {
		proj_get_uint16_t(ptr + 8, &middle3);
		proj_get_uint_n(ptr + 10, &last, 6);
	} else {
		proj_get_rev_uint16_t(ptr + 8, &middle3);
		proj_get_rev_uint_n(ptr + 10, &last, 6);
	}
	std::memset(guidstr, 0, sizeof(guidstr));
	std::snprintf(guidstr, sizeof(guidstr), "%.8X-%.4hX-%.4hX-%.4hX-%.12llX", first, middle1, middle2, middle3, last);
	return out << guidstr;
}

int main(void)
{
	//EFI_GUID a = EFI_FIRMWARE_FILE_SYSTEM_GUID;
	//EFI_GUID b = PROJ_UNKNOWN_GUID;
	EFI_GUID c = PROJ_SUPER_GUID;

	// std::cout << (proj_is_big_endian() ? "BIG-ENDIAN\n" : "LITTLE_ENDIAN\n");
	{
		//std::uint16_t a = 0xABCD;
		//std::uint32_t b = 0x12345678;
		std::uint64_t c = 0x0123456789ABCDEF;
		std::uint64_t d = 0;
		
		//proj_get_rev_uint_n(&c, &d, 7);

		//std::cout << std::hex << std::uppercase << std::setfill('0');
		//std::cout << std::setw(16) << d << '\n';
	}

	//std::cout << a << '\n';
	//std::cout << b << '\n';
	std::cout << c << '\n';

	return 1;
}