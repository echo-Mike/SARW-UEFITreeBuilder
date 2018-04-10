#pragma once
#ifndef STRUCTURE_VIEW_HPP__
#define STRUCTURE_VIEW_HPP__ "0.0.0@cStructureView.hpp"

/// STD
#include <type_traits>

/// SNIPPETS
#include <ClassUtilsLib/mClassUtils.hpp>

/// PROJECT
#include "General.hpp"
#include "cMemoryView.hpp"

namespace Project
{
	template < typename StructureT, typename BaseTagT = void>
	struct StructureView :
		public MemoryView, 
		public BaseTagT
	{
		typedef MemoryView Base;

		typedef BaseTagT BaseTag;

		typedef std::decay<StructureT>::type value_type;

		typedef value_type* pointer_t;

		typedef const value_type* const_pointer_t;

		static const std::size_t structure_size = sizeof(value_type);

		explicit StructureView(Types::const_pointer_t ptr) : MemoryView(ptr, ptr + structure_size) {}

		explicit StructureView(const MemoryView& other) : MemoryView(other) {}

		DefaultCopyableAndMovable(StructureView)

		~StructureView() = default;

		StructureView& operator=(const MemoryView& other) { Base::operator=(other); }

		bool hasView() const { return begin != Types::const_pointer_t(); }

		const_pointer_t operator->() const { return begin; }

		const_pointer_t get() const { return this->operator->(); }

		operator const_pointer_t() const { return get(); }
	};

	template < typename StructureT >
	struct StructureView<StructureT, void> :
		public MemoryView
	{
		typedef MemoryView Base;

		typedef std::decay<StructureT>::type value_type;

		typedef value_type* pointer_t;

		typedef const value_type* const_pointer_t;

		static const std::size_t structure_size = sizeof(value_type);

		explicit StructureView(Types::const_pointer_t ptr) : MemoryView(ptr, ptr + structure_size) {}

		explicit StructureView(const MemoryView& other) : MemoryView(other) {}

		DefaultCopyableAndMovable(StructureView)

		~StructureView() = default;

		StructureView& operator=(const MemoryView& other) { Base::operator=(other);	}

		bool hasView() const { return begin != Types::const_pointer_t(); }

		const_pointer_t operator->() const { return begin; }

		const_pointer_t get() const { return this->operator->(); }

        operator const_pointer_t() const { return get(); }

    };
}

#endif
