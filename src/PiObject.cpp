/// STD
#include <algorithm>
/// PROJECT
#include "PiObjectRepresentation.hpp"

namespace Project
{
	namespace PiObject
	{
		void BaseObject::toJson(nlohmann::json& j) const
		{
			j["uid"] = uid;
			j["position"] = OffsetView(basePtr, memory);
			if (!freeSpace.empty())
			{
				std::vector< OffsetView > freeOffsets;
				freeOffsets.reserve(freeSpace.size());
				for (const auto& v : freeSpace)
					freeOffsets.emplace_back(memory.begin, v);
				j["freeSpace"] = freeOffsets;
			}
		}

		void BaseObject::setUid(const MemoryView& buffer)
		{
			uid = Checksums::hash(buffer, buffer.getLength());
		}
	}
}