#pragma once
#ifndef GUID_ADVANCED_HPP__
#define GUID_ADVANCED_HPP__ "0.0.0@GuidAdvanced.hpp"

/// STD
#include <iostream>

std::ostream& operator<<(std::ostream& out, const EFI_GUID& guid);
std::ostream& operator<<(std::ostream& out, const Project::Guid::GuidWithName& guid);

/// JSON
#include <nlohmann/json.hpp>

void to_json(nlohmann::json& j, const EFI_GUID& guid);
void to_json(nlohmann::json& j, const Project::Guid::GuidWithName& guid);

/// PROJECT
#include "GuidDefines.h"

namespace Project
{

	namespace Guid
	{

		namespace FFS
		{

			inline bool isValidFfsGuid(const EFI_GUID* ptr)
			{
				auto& ref = NamedGuids::findNamedGuid(ptr);
				return NamedGuids::FvGuids() <= &ref && &ref < NamedGuids::FvGuidsEnd();
			}
			inline bool isValidFfsGuid(const EFI_GUID& guid) { return isValidFfsGuid(&guid); }
			inline bool isValidFfsGuid(Types::const_pointer_t ptr) { return isValidFfsGuid(reinterpret_cast<const EFI_GUID*>(ptr)); }

		}

	}

}

#endif