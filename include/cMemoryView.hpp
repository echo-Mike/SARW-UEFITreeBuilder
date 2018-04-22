#pragma once
#ifndef MEMORY_VIEW_HPP__
#define MEMORY_VIEW_HPP__ "0.0.0@cMemoryView.hpp"

/// SNIPPETS
#include <ClassUtilsLib/mClassUtils.hpp>

/// JSON
#include <nlohmann/json.hpp>

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

		bool isInside(const void* ptr) const { return begin <= reinterpret_cast<Types::const_pointer_t>(ptr) && reinterpret_cast<Types::const_pointer_t>(ptr) < end; }

		bool isOutside(const void* ptr) const { return !isInside(ptr); }

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

		inline bool operator<(const MemoryView& other) noexcept { return begin < other.begin; }

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

	void to_json(nlohmann::json& j, const OffsetView& obj) 
	{ 
		j["offset"] = obj.offset;
		j["length"] = obj.length;
		j["valid"] = obj.valid;
	}
}
#endif