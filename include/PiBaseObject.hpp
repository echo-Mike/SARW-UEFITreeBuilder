#pragma once
#ifndef PI_BASE_OBJECT_HPP__
#define	PI_BASE_OBJECT_HPP__ "0.0.0@PiBaseObject.hpp"

/// STD
#include <vector>

/// JSON
#include <nlohmann/json.hpp>

/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"
#include "PiObjectRepresentation.hpp"

namespace Project
{
	namespace PiObject
	{

		struct FreeSpace :
			public MemoryView
		{
			typedef MemoryView Base;

			FreeSpace(  Types::memory_t empty,
						Types::const_pointer_t begin_ = nullptr,
						Types::const_pointer_t end_ = nullptr ) : 
				Base(begin_, end_) 
			{
				checkCorruption(empty);
			}

			DefaultCopyableAndMovable(FreeSpace)
			
			~FreeSpace() = default;

			bool checkCorruption(Types::memory_t empty);

			bool isCorrupt() const { return corruptFlag; }
		private:
			bool corruptFlag;
		};

		typedef std::vector< FreeSpace > FreeSpaceVec_t;

		//void to_json(nlohmann::json& j, const FreeSpace& obj);

		struct BaseObject
		{

			BaseObject(const MemoryView& baseBuffer, const MemoryView& myBuffer) :
				basePtr(baseBuffer.begin), memory(myBuffer), uid(0) {}

			DefaultCopyableAndMovable(BaseObject)
			
			virtual ~BaseObject() = default;

			virtual void toJson(nlohmann::json& j) const;

			void setUid(const MemoryView& buffer);

			Types::const_pointer_t basePtr;
			MemoryView memory;
			Types::hash_t uid;
			FreeSpaceVec_t freeSpace;
		};

		void to_json(nlohmann::json& j, const BaseObject& obj) { obj.toJson(j); }

	}
}

#endif