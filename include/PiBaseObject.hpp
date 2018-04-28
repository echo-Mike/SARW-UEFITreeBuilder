#pragma once
#ifndef PI_BASE_OBJECT_HPP__
#define	PI_BASE_OBJECT_HPP__ "0.0.0@PiBaseObject.hpp"

/// STD
#include <memory>
#include <vector>
#include <utility>

/// JSON
#include <nlohmann/json.hpp>

/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"

namespace Project
{

	namespace Pi
	{

		namespace Volume
		{
			void to_json(nlohmann::json& j, const Header& hdr);
		}

		namespace File
		{
			void to_json(nlohmann::json& j, const Header& hdr);
		}

		namespace Section
		{
			void to_json(nlohmann::json& j, const Header& hdr);
		}

	}

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
	return unique_object_ptr_t(new TypeName(*this)); \
}

#define PROJ_CopyablePiObject( TypeName ) \
unique_object_ptr_t copy() \
{ \
	return unique_object_ptr_t(new TypeName(*this)); \
}


		inline void setSimpleUid(Object& obj) { obj.setUid(MemoryView(UnifyPtrCast(&obj), UnifyPtrCast(&obj) + sizeof(Object))); }


		inline void to_json(nlohmann::json& j, const Object& obj) { (&obj)->toJson(j); }


		inline void to_json(nlohmann::json& j, const unique_object_ptr_t& obj_ptr) { obj_ptr->toJson(j); }


		struct FreeSpace :
			public Object
		{
			typedef Object Base;

			static bool IsCorrupted(Types::memory_t empty, const MemoryView& buffer);

		private:

			void initialize(Types::memory_t empty)
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

			FreeSpace(Types::memory_t empty, const MemoryView& baseBuffer, const MemoryView& myBuffer) :
				Base(baseBuffer, myBuffer, InconsistencyState::FreeSpaceFlag)
			{
				initialize(empty);
			}

			FreeSpace(Types::memory_t empty, const MemoryView& baseBuffer, MemoryView&& myBuffer) :
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


		typedef std::vector< unique_object_ptr_t > object_vec_t;


		struct ComplexObject :
			public Object
		{

			typedef Object Base;

			ComplexObject( const MemoryView& baseBuffer, const MemoryView& myBuffer, 
				           InconsistencyState::InconsistencyState_t init_state = InconsistencyState::Unknown ) :
				Base(baseBuffer, myBuffer, init_state) {}

			ComplexObject( const MemoryView& baseBuffer, MemoryView&& myBuffer, 
				           InconsistencyState::InconsistencyState_t init_state = InconsistencyState::Unknown ) :
				Base(baseBuffer, std::move(myBuffer), init_state) {}

			ComplexObject(const ComplexObject& other) : Base(other)
			{
				objects.reserve(other.objects.size());
				for (const auto& v : other.objects) {
					objects.emplace_back(v->copy());
				}
			}

			ComplexObject& operator=(const ComplexObject& other)
			{
				if (this == &other)
					return *this;
				objects.clear();
				objects.reserve(other.objects.size());
				for (const auto& v : other.objects) {
					objects.emplace_back(v->copy());
				}
				Base::operator=(other);
				return *this;
			}

			DefaultMovable(ComplexObject)

			virtual ~ComplexObject() = default;

			// Virtual i-face implementation

			virtual void toJson(nlohmann::json& j) const;

			PROJ_BaseCopyablePiObject(ComplexObject)

			template< typename T, typename... Args>
			inline std::enable_if< std::is_base_of<Object, T>::value >::type 
				emplace_back(Args&&... args)
			{
				objects.emplace_back(new T(std::forward<Args>(args)...));
			}

			object_vec_t objects;
		};

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