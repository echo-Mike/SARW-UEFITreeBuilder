#pragma once
#ifndef SIGNATURE_CASTS_HPP__
#define	SIGNATURE_CASTS_HPP__ "0.0.0@SignatureCasts.hpp"

/// STD
#include <cstdlib>
#include <cstdint>

/// PROJECT
#include "ProjectTypes.hpp"

namespace Project
{
	template < std::uint16_t Signature >
	union signature_cast_16
	{
		Types::memory_t bytes[ sizeof(std::uint16_t) / sizeof(Types::memory_t) ];
		std::uint16_t signature = Signature;
	};

	template < std::uint32_t Signature >
	union signature_cast_32
	{
		Types::memory_t bytes[sizeof(std::uint32_t) / sizeof(Types::memory_t)];
		std::uint16_t signature = Signature;
	};

	template < std::uint64_t Signature >
	union signature_cast_64
	{
		Types::memory_t bytes[sizeof(std::uint64_t) / sizeof(Types::memory_t)];
		std::uint16_t signature = Signature;
	};
}

#endif