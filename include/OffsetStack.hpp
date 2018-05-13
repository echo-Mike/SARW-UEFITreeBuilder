#pragma once
#ifndef OFFSET_STACK_HPP__
#define OFFSET_STACK_HPP__ "0.0.0@OffsetStack.hpp"

/// STD
#include <stack>
#include <vector>
#include <numeric>
#include <iterator>

/// SNIPPETS
#include <ClassUtilsLib/mClassUtils.hpp>

/// PROJECT
#include "ProjectTypes.hpp"


namespace Project
{

	class OffsetStack
	{
	public:

		typedef Types::length_t offset_t;
		typedef std::vector<offset_t> offset_vector_t;
		typedef std::stack<offset_vector_t::size_type> base_stack_t;

		constexpr static auto reserved = 64;

		OffsetStack() : currentOffset(0) { offsets.reserve(reserved); }

		DefaultCopyableAndMovable(OffsetStack)

		~OffsetStack() = default;

		inline offset_t push(offset_t offset)
		{
			offsets.emplace_back(offset);
			return currentOffset += offset;
		}

		inline offset_t pop()
		{
			if ( stack.empty() && !offsets.empty() || 
				!stack.empty() &&  offsets.size() != stack.top() )
			{
				auto back = offsets.back();
				offsets.pop_back();
				return currentOffset -= back;
			}
			return currentOffset = 0;
		}

		inline void pushBase()
		{
			stack.push(offsets.size());
			currentOffset = 0;
		}

		inline offset_t popBase()
		{
			if (!stack.empty())
			{
				auto base = stack.top();
				stack.pop();
				offsets.resize(base);
				return currentOffset = std::accumulate( offsets.begin() + (stack.empty() ? 0 : stack.top()), offsets.end(), static_cast<Types::length_t>(0));
			}
			return currentOffset = 0;
		}

		inline offset_t get() const { return currentOffset; }

		inline bool empty() const { return offsets.empty() && stack.empty(); }

		inline bool hasBase() const { return !stack.empty(); }

	private:
		offset_t currentOffset = 0;
		offset_vector_t offsets;
		base_stack_t stack;
	};

}

#endif