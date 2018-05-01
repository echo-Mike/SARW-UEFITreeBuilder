#pragma once
#ifndef PI_SIMPLE_OBJECTS_HPP__
#define	PI_SIMPLE_OBJECTS_HPP__ "0.0.0@PiSimpleObjects.hpp"

/// PROJECT
#include "PiObject.hpp"

namespace Project
{

	namespace PiObject
	{

		struct FreeSpace :
			public Object
		{
			typedef Object Base;

			static bool IsCorrupted(Types::byte_t empty, const MemoryView& buffer);

		private:

			inline void initialize(Types::byte_t empty)
			{
				if (IsCorrupted(empty, memory)) {
					state |= InconsistencyState::FreeSpaceCorrupted;
				}
				// Hash self representation
				setSimpleUid(*this);
				// Save empty
				std::uint32_t empty_ = empty;
				empty_ <<= PROJ_FREE_SPACE_STATE_EMPTY_START_BIT;
				empty_ &= InconsistencyState::FreeSpaceEmpty;
				state = static_cast<InconsistencyState::InconsistencyState_t>(state | empty_);
			}

		public:

			FreeSpace(Types::byte_t empty, const MemoryView& baseBuffer, const MemoryView& myBuffer) :
				Base(baseBuffer, myBuffer, InconsistencyState::FreeSpaceFlag)
			{
				initialize(empty);
			}

			FreeSpace(Types::byte_t empty, const MemoryView& baseBuffer, MemoryView&& myBuffer) :
				Base(baseBuffer, std::move(myBuffer), InconsistencyState::FreeSpaceFlag)
			{
				initialize(empty);
			}

			DefaultCopyableAndMovable(FreeSpace)

			~FreeSpace() = default;

			// Virtual i-face implementation

			void toJson(nlohmann::json& j) const;

			PROJ_CopyablePiObject(FreeSpace)

			// Class i-face

			inline bool isCorrupted() const { return state & InconsistencyState::FreeSpaceCorrupted; }

		};

		struct Data :
			public Object
		{
			typedef Object Base;

		public:

			Data(const MemoryView& baseBuffer, const MemoryView& myBuffer) :
				Base(baseBuffer, myBuffer, InconsistencyState::DataFlag) 
			{ 
				setSimpleUid(*this); 
			}

			Data(const MemoryView& baseBuffer, MemoryView&& myBuffer) :
				Base(baseBuffer, std::move(myBuffer), InconsistencyState::DataFlag) 
			{
				setSimpleUid(*this);
			}

			DefaultCopyableAndMovable(Data)

			~Data() = default;

			// Virtual i-face implementation

			void toJson(nlohmann::json& j) const;

			PROJ_CopyablePiObject(Data)

		};

	}

}

#endif
