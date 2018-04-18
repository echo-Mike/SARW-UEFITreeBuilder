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

		/// Size

		typedef std::size_t length_t;

		typedef signed long long index_t;

		typedef length_t count_t;

		/// Hash

		typedef std::size_t hash_t;

		/// Void

		typedef void void_t;

		typedef void_t* void_ptr_t;

		typedef const void_t* const_void_ptr_t;

		/// Byte

		typedef std::uint8_t byte_t;

		typedef byte_t* byte_ptr_t;

		typedef const byte_t* const_byte_ptr_t;

		/// Memory

		typedef byte_t memory_t;

		typedef memory_t* pointer_t;

		typedef const memory_t* const_pointer_t;

	}
}

#endif