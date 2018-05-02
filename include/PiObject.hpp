#pragma once
#ifndef PI_OBJECT_HPP__
#define	PI_OBJECT_HPP__ "0.0.0@PiObject.hpp"

/// STD
#include <memory>
#include <vector>
#include <utility>
#include <type_traits>

/// JSON
#include <nlohmann/json.hpp>

/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"

namespace Project
{

	namespace PiObject
	{

		namespace InconsistencyState
		{
			enum InconsistencyState_t : std::uint32_t
			{
				Unknown        = 0x00000000,
				FreeSpaceFlag  = 0x80000000,
				DataFlag       = 0x40000000,
				SectionFlag    = 0x20000000,
				FileFlag       = 0x10000000,
				VolumeFlag     = 0x08000000,

				TypeBits       = FreeSpaceFlag | DataFlag | SectionFlag | FileFlag | VolumeFlag,

				FreeSpaceNormal    = FreeSpaceFlag,
				FreeSpaceCorrupted = FreeSpaceFlag | 0x00000001,
				FreeSpaceEmpty     = 0x0000FF00,
#define PROJ_FREE_SPACE_STATE_EMPTY_START_BIT (8)

				DataNormal         = DataFlag,
				DataInvalidVolume  = DataFlag | 0x00000001,
				DataInvalidFile    = DataFlag | 0x00000002,
				DataInvalidSection = DataFlag | 0x00000004,

				SectionNormal = SectionFlag,

				FileNormal = FileFlag,

				VolumeNormal                = VolumeFlag,
				VolumeAlreadyParsed         = VolumeFlag | 0x00000001,
				VolumeBlockMapCollision     = VolumeFlag | 0x00000002,
				VolumeInvalidExtendedHeader = VolumeFlag | 0x00000004,
				VolumeUnknownFileSystem     = VolumeFlag | 0x00000008
			};
		}

		struct Object;

		typedef std::unique_ptr< Object > unique_object_ptr_t;

		struct Object
		{

			Object( const MemoryView& baseBuffer, const MemoryView& myBuffer,
				    InconsistencyState::InconsistencyState_t init_state = InconsistencyState::Unknown ) :
				baseBegin(baseBuffer.begin), memory(myBuffer),
				uid(0), state(init_state)
			{}

			Object( const MemoryView& baseBuffer, MemoryView&& myBuffer,
				    InconsistencyState::InconsistencyState_t init_state = InconsistencyState::Unknown ) :
				baseBegin(baseBuffer.begin), memory(std::move(myBuffer)),
				uid(0), state(init_state)
			{}

			DefaultCopyableAndMovable(Object)

			virtual ~Object() = default;

			virtual void toJson(nlohmann::json& j) const;

			virtual unique_object_ptr_t copy() { return std::make_unique<Object>(*this); };

			inline Types::hash_t getUid() const { return uid; }

			inline bool operator<(const Object& other) noexcept { return memory < other.memory; }

		protected:

			void setUid(const MemoryView& buffer);

			void setUid(MemoryView&& buffer);

			inline friend void setSimpleUid(Object& obj);

		private:
			Types::const_pointer_t baseBegin;
			Types::hash_t uid;
		public:
			MemoryView memory;
			InconsistencyState::InconsistencyState_t state;
		};


#define PROJ_BaseCopyablePiObject( TypeName ) \
virtual unique_object_ptr_t copy() \
{ \
	static_assert( \
		::std::is_base_of<\
			::Project::PiObject::Object, \
			TypeName \
		>::value, \
		"Type: " #TypeName " must be derived form PiObject::Object" \
	); \
	return unique_object_ptr_t(::std::make_unique<TypeName>(*this)); \
}

#define PROJ_CopyablePiObject( TypeName ) \
unique_object_ptr_t copy() \
{ \
	static_assert( \
		::std::is_base_of<\
			::Project::PiObject::Object, \
			TypeName \
		>::value, \
		"Type: " #TypeName " must be derived form PiObject::Object" \
	); \
	return unique_object_ptr_t(::std::make_unique<TypeName>(*this)); \
}

		inline void setSimpleUid(Object& obj) { obj.setUid(MemoryView(UnifyPtrCast(&obj), UnifyPtrCast(&obj) + sizeof(Object))); }

		typedef std::vector< unique_object_ptr_t > object_vec_t;

	}

}

inline Project::PiObject::InconsistencyState::InconsistencyState_t& 
operator|=(
	Project::PiObject::InconsistencyState::InconsistencyState_t& a,
	const Project::PiObject::InconsistencyState::InconsistencyState_t b
)
{
	using namespace Project::PiObject::InconsistencyState;
	if ((a & TypeBits) ^ (b & TypeBits)) DEBUG_WARNING_MESSAGE
		DEBUG_PRINT("\tMessage: Can't assign new state. Type bits differ.");
		DEBUG_PRINT("\tOriginal state: ", a);
		DEBUG_PRINT("\tNew state: ", b);
	DEBUG_END_MESSAGE_AND_EVAL({ return a; })
	a = static_cast<Project::PiObject::InconsistencyState::InconsistencyState_t>(a | b);
	return a;
}

#endif