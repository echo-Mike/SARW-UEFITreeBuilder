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
		typedef std::vector< Pi::Section::Extended::Header > ExtSectionsVec_t;

		struct SectionFinder
		{

			SectionsVec_t operator()(const MemoryView& buffer);

			ExtSectionsVec_t operator()(const MemoryView& buffer, bool);
		};

	}
}

#endif