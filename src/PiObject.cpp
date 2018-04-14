/// STD
#include <algorithm>
/// PROJECT
#include "PiObjectRepresentation.hpp"

namespace Project
{
	namespace PiObject
	{

		bool FreeSpace::checkCorruption(Types::memory_t empty)
		{
			corruptFlag = (end != std::find_if_not(
				begin, 
				end, 
				[empty](const Types::memory_t v) -> bool
				{ 
					return v == empty; 
				}
			));
			return corruptFlag;
		}

		//void to_json(nlohmann::json& j, const FreeSpace& obj)
		//{

		//}

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