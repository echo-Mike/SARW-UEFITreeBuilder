#pragma once
#ifndef PI_COMPLEX_OBJECTS_HPP__
#define	PI_COMPLEX_OBJECTS_HPP__ "0.0.0@PiComplexObject.hpp"

/// PROJECT
#include "PiObject.hpp"

namespace Project
{

	namespace PiObject
	{

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
			
			// Class i-face

			void toJson(nlohmann::json& j, bool) const;

			template< typename T, typename... Args>
			inline typename std::enable_if< std::is_base_of<Object, T>::value >::type 
				emplace_back(Args&&... args)
			{
				objects.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
			}

			object_vec_t objects;
		};

	}

}

#endif
