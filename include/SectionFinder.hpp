#pragma once
#ifndef SECTION_FINDER_HPP__
#define	SECTION_FINDER_HPP__ "0.0.0@SectionFinder.hpp"

/// STD
#include <vector>

/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"

namespace Project
{
	namespace Finders
	{

		typedef std::vector< Pi::Section::Header > SectionsVec_t;

		struct SectionFinder
		{

			SectionsVec_t operator()(const MemoryView& buffer, Types::memory_t empty);
		};

	}
}

#endif