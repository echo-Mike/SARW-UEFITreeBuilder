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
	namespace helper
	{
		struct AbstractInheritance
		{
			virtual AbstractInheritance* copy() const = 0;

			virtual AbstractInheritance* move() = 0;

			virtual ~AbstractInheritance() {}
		};

		template < typename T >
		struct InheritanceFilter :
			public AbstractInheritance,
			public T
		{};
	}

	typedef helper::AbstractInheritance* AbstInhPtr_t;

	template < typename StructureT, typename BaseTagT = void >
	struct StructureView :
		public MemoryView,
		public helper::InheritanceFilter< BaseTagT >
	{
		typedef MemoryView Base;

		typedef BaseTagT BaseTag;
		
		typedef helper::InheritanceFilter< BaseTagT > AbstractBase;

		typedef std::decay<StructureT>::type value_type;

		typedef value_type* pointer_t;

		typedef const value_type* const_pointer_t;

		static const std::size_t structure_size = sizeof(value_type);

		StructureView() : MemoryView() {}

		explicit StructureView(Types::const_pointer_t ptr) : MemoryView(ptr, ptr + structure_size) {}

		explicit StructureView(const_pointer_t ptr) : MemoryView(UnifyPtrCast(ptr), UnifyPtrCast(ptr) + structure_size) {}

		explicit StructureView(const MemoryView& other) : MemoryView(other) {}

		DefaultCopyableAndMovable(StructureView)

		~StructureView() = default;

		StructureView& operator=(const MemoryView& other) { return Base::operator=(other); }

		StructureView& operator=(Types::const_pointer_t ptr) 
		{
			if (ptr != Types::const_pointer_t()) {
				begin = ptr;
				setLength(structure_size);
			}
			return *this; 
		}

		bool hasView() const { return begin != Types::const_pointer_t(); }

		const_pointer_t operator->() const { return begin; }

		const_pointer_t get() const { return this->operator->(); }

		operator const_pointer_t() const { return get(); }

		AbstractBase* copy() const { return new StructureView(*this); }

		AbstractBase* move() { return new StructureView(std::move(*this)); }
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

		const_pointer_t operator->() const { return begin; }

		const_pointer_t get() const { return this->operator->(); }

        operator const_pointer_t() const { return get(); }
    };
}

#endif
