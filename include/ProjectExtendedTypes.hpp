#pragma once
#ifndef PROJECT_EXTENDED_TYPES_HPP__
#define PROJECT_EXTENDED_TYPES_HPP__ "0.0.0@ProjectExtendedTypes.hpp"

/// STD
#include <memory>

/// PROJECT
#include "ProjectTypes.hpp"

namespace Project
{

	namespace Types
	{

		/// unique_ptr

		typedef std::unique_ptr<Types::memory_t[]> unique_memo_buff_t;

		typedef std::unique_ptr<Types::byte_t[]>   unique_byte_buff_t;

	}
}

#endif