#pragma once
#ifndef GUID_ADVANCED_HPP__
#define GUID_ADVANCED_HPP__ "0.0.0@GuidAdvanced.hpp"

/// STD
#include <iostream>

/// PROJECT
#include "GuidDefines.h"
#include "MemoryView.hpp"

std::ostream& operator<<(std::ostream& out, const EFI_GUID& guid);
std::ostream& operator<<(std::ostream& out, const Project::Guid::GuidWithName& guid);

namespace Project
{

	namespace Guid
	{

		namespace FFS
		{

			inline bool isValidFfsGuid(const EFI_GUID* ptr)
			{
				auto ptr_ = &NamedGuids::findNamedGuid(ptr);
				return NamedGuids::FvGuids() <= ptr_ && ptr_ < NamedGuids::FfsGuidsEnd();
			}
			inline bool isValidFfsGuid(const EFI_GUID& guid) { return isValidFfsGuid(&guid); }
			inline bool isValidFfsGuid(Types::const_pointer_t ptr) { return isValidFfsGuid(reinterpret_cast<const EFI_GUID*>(ptr)); }

		}

		inline FindGUIDResult::FindGUIDResult_t findGuid(const EFI_GUID* guid, const MemoryView& mv, FindGUIDResult::result_t& result)
		{
			return findGuid(guid, mv.begin, mv.getLength(), result);
		}
		inline FindGUIDResult::FindGUIDResult_t findGuid(const EFI_GUID& guid, const MemoryView& mv, FindGUIDResult::result_t& result)
		{
			return findGuid(&guid, mv.begin, mv.getLength(), result);
		}

	}

}

#endif