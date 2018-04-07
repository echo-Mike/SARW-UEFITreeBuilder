#pragma once
#ifndef MEMORY_VIEW_HPP__
#define MEMORY_VIEW_HPP__ "0.0.0@cMemoryView.hpp"

/// SNIPPETS
#include <ClassUtilsLib/mClassUtils.hpp>

/// PROJECT
#include "General.hpp"

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

		bool checkPointers() const { return begin != Types::const_pointer_t() && end != Types::const_pointer_t(); }

		Types::length_t getLength() const { return end - begin; }

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

        void setLength(Types::length_t length) { end = begin != Types::const_pointer_t() ? begin + length : begin; }

        operator Types::const_pointer_t() const { return begin; }
        operator const char*() const { return reinterpret_cast<const char*>(begin); }
        operator const void*() const { return reinterpret_cast<const void*>(begin); }
        operator const unsigned char*() const { return reinterpret_cast<const unsigned char*>(begin); }

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

		Types::const_pointer_t begin; //!< Pointer to beginning of viewed data
		Types::const_pointer_t end;   //!< Pass-the-end pointer of viewed data

    };
}
#endif