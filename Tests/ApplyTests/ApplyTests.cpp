#include <iostream>
#include <iomanip>

extern "C"
{
#include <Base.h>
#include <PiPei.h>
#include <PiDxe.h>
#include <Uefi.h>
}

// 7A9354D9-0468-444A-81CE-0BF617D890DF
#define EFI_FIRMWARE_FILE_SYSTEM_GUID \
    { 0x7a9354d9, 0x0468, 0x444a, { 0x81, 0xce, 0x0b, 0xf6, 0x17, 0xd8, 0x90, 0xdf } }

// 00000000-0000-0000-0000-000000000000
#define PROJ_UNKNOWN_GUID \
	{ 0x00000000, 0x0000, 0x0000, { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }

// AAAABBBB-CCCC-DDDD-EEEE-FFFF00001111
#define PROJ_SUPER_GUID \
	{ 0xAAAABBBB, 0xCCCC, 0xDDDD, { 0xEE, 0xEE, 0xFF, 0xFF, 0x00, 0x00, 0x11, 0x11 } }

std::ostream& operator<<(std::ostream& out, const EFI_GUID& guid)
{
	out << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << guid.Data1 << '-';
	out << std::setw(4) << guid.Data2 << '-';
	out << std::setw(4) << guid.Data3 << '-';
	out << std::setw(4) << ((static_cast<std::uint16_t>(guid.Data4[0]) << 8) + guid.Data4[1]) << '-';
	std::size_t last =  (static_cast<std::size_t>(guid.Data4[2]) << 40) |
						(static_cast<std::size_t>(guid.Data4[3]) << 32) |
						(static_cast<std::size_t>(guid.Data4[4]) << 24) |
						(static_cast<std::size_t>(guid.Data4[5]) << 16) |
						(static_cast<std::size_t>(guid.Data4[6]) <<  8) |
						 static_cast<std::size_t>(guid.Data4[7]);
	out << std::setw(12) << last;
	return out;
}

int main(void)
{
	EFI_GUID a = EFI_FIRMWARE_FILE_SYSTEM_GUID;
	EFI_GUID b = PROJ_UNKNOWN_GUID;
	EFI_GUID c = PROJ_SUPER_GUID;

	std::cout << a << '\n';
	std::cout << b << '\n';
	std::cout << c << '\n';

	return 1;
}