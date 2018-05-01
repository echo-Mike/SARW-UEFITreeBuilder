#pragma once
#ifndef MEMORY_VIEW_HPP__
#define MEMORY_VIEW_HPP__ "0.0.0@MemoryView.hpp"

/// STD
#include <cstring>

/// SNIPPETS
#include <ClassUtilsLib/mClassUtils.hpp>

/// PROJECT
#include "General.hpp"
#include "ProjectExtendedTypes.hpp"

namespace Project 
{

    struct MemoryView 
	{

        explicit MemoryView(
			Types::const_pointer_t begin_ = nullptr, 
			Types::const_pointer_t end_ = nullptr
		) : begin(begin_), end(end_)
        {
            if ( begin_ != Types::const_pointer_t() && end_ != Types::const_pointer_t() ) 
			{
                if (begin_ > end_) {
                    std::swap(begin, end);
                }
            }
        }

		DefaultCopyableAndMovable(MemoryView)
		
		~MemoryView() = default;

		// Class i-face

		inline bool checkPointers() const { return begin != Types::const_pointer_t() && end != Types::const_pointer_t(); }

		inline bool isInside(const void* const ptr) const { return begin <= reinterpret_cast<Types::const_pointer_t>(ptr) && reinterpret_cast<Types::const_pointer_t>(ptr) < end; }

		inline bool isOutside(const void* const ptr) const { return !isInside(ptr); }

		inline Types::length_t getLength() const { return end - begin; }

		inline Types::unique_memo_buff_t memcpy() const
		{
			Types::unique_memo_buff_t result(new Types::memory_t[getLength()]);
			std::memcpy(result.get(), begin, getLength() * sizeof(Types::memory_t));
			return result;
		}

        bool setEnd(Types::const_pointer_t newEnd) 
		{
			if (newEnd == Types::const_pointer_t()) {
				return false;
			}
            end = newEnd;
            if (checkPointers()) {
				if (begin > end) {
					std::swap(begin, end);
				}
            }
			return true;
        }

        inline void setLength(Types::length_t length) { end = begin != Types::const_pointer_t() ? begin + length : begin; }

		// Conversions and operators

        operator Types::const_pointer_t() const { return begin; }
        operator const char*() const { return reinterpret_cast<const char*>(begin); }
        operator const void*() const { return reinterpret_cast<const void*>(begin); }

		MemoryView& operator=(Types::const_pointer_t ptr)
		{
            if (ptr != Types::const_pointer_t()) 
			{
				Types::length_t length = getLength();
                begin = ptr;
                setLength(length);
            }
            return *this;
        }

		inline bool operator<(const MemoryView& other) noexcept { return begin < other.begin; }

		// Class members

		Types::const_pointer_t begin; //!< Pointer to beginning of viewed data
		Types::const_pointer_t end;   //!< Pass-the-end pointer of viewed data

    };

	struct OffsetView
	{
		OffsetView(Types::const_pointer_t dataPtr, const MemoryView& mv) :
			offset(0), length(0), valid(false)
		{	// Check that pointers are valid
			if (dataPtr == Types::const_pointer_t() || !mv.checkPointers())
				return; 
			if (dataPtr < mv.begin)
				return;
			offset = mv.begin - dataPtr;
			length = mv.getLength();
			valid = true;
		}

		DefaultCopyableAndMovable(OffsetView)
		
		~OffsetView() = default;

		static MemoryView transform(Types::const_pointer_t dataPtr, const OffsetView& ov)
		{
			MemoryView result;
			if (!ov.valid || dataPtr == Types::const_pointer_t())
				return result;
			result.begin = dataPtr + ov.offset;
			result.setLength(ov.length);
			return result;
		}

		MemoryView transform(Types::const_pointer_t dataPtr) { return transform(dataPtr, *this); }

		Types::length_t offset;
		Types::length_t length;
		bool valid;
	};

}
#endif