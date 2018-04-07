#pragma once
#ifndef PROJECT_TYPES_HPP__
#define PROJECT_TYPES_HPP__ "0.0.0@ProjectTypes.hpp"

/// STD
#include <cstdlib>
#include <cstddef>
#include <cstdint>

namespace Project
{

	namespace Types
	{

		typedef std::uint8_t memory_t;

		typedef memory_t* pointer_t;

		typedef const memory_t* const_pointer_t;

		typedef std::size_t length_t;
	}
}

#endif