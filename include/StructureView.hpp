#pragma once
#ifndef STRUCTURE_VIEW_HPP__
#define STRUCTURE_VIEW_HPP__ "0.0.0@StructureView.hpp"

/// STD
#include <type_traits>

/// SNIPPETS
#include <ClassUtilsLib/mClassUtils.hpp>

/// PROJECT
#include "General.hpp"
#include "MemoryView.hpp"

namespace Project
{

	template < typename StructureT >
	struct StructureView :
		public MemoryView
	{
		typedef MemoryView Base;

		typedef typename std::decay<StructureT>::type value_type;

		typedef value_type* pointer_t;

		typedef const value_type* const_pointer_t;

		static const std::size_t structure_size = sizeof(value_type);

		StructureView() : MemoryView() {}

		explicit StructureView(Types::const_pointer_t ptr) : MemoryView(ptr, ptr + structure_size) {}

		explicit StructureView(const_pointer_t ptr) : MemoryView(UnifyPtrCast(ptr), UnifyPtrCast(ptr) + structure_size) {}

		explicit StructureView(const MemoryView& other) : MemoryView(other) {}

		DefaultCopyableAndMovable(StructureView)

		~StructureView() = default;

		StructureView& operator=(const MemoryView& other) { Base::operator=(other);	}

		StructureView& operator=(Types::const_pointer_t ptr)
		{
			if (ptr != Types::const_pointer_t()) {
				begin = ptr;
				setLength(structure_size);
			}
			return *this;
		}

		bool hasView() const { return begin != Types::const_pointer_t(); }

		inline const_pointer_t operator->() const { return reinterpret_cast<const_pointer_t>(begin); }

		inline const_pointer_t get() const { return this->operator->(); }
		
		operator const_pointer_t() const { return get(); }
    };

}

#endif
