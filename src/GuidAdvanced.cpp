/// STD
#include <iomanip>
#include <cstdio>
/// PROJECT
#include "GuidAdvanced.hpp"

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

std::ostream& operator<<(std::ostream& out, const Project::Guid::GuidWithName& guid)
{
	return out << "{ GUID: " << guid.value << " , Name: " << guid.name << " }";
}

void to_json(nlohmann::json& j, const EFI_GUID& guid)
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
	j = &guidstr;
}

void to_json(nlohmann::json& j, const Project::Guid::GuidWithName& guid)
{
	j["value"] = guid.value;
	j["name"] = guid.name;
}

#undef MAKE_FFS_GUID