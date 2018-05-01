#pragma once
#ifndef PI_FINDERS_HPP__
#define PI_FINDERS_HPP__ "0.0.0@PiFinders.hpp"

/// STD
#include <vector>

/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"

namespace Project
{

	namespace Finders
	{

		/// Header vectors

		typedef std::vector< Pi::Section::Header > SectionsVec_t;

		typedef std::vector< Pi::File::Header > FilesVec_t;

		typedef std::vector< Pi::Volume::Header > VolumesVec_t;

		/// Finder functions

		SectionsVec_t SectionFinder(const MemoryView& buffer);

		FilesVec_t FileFinder(const MemoryView& buffer, Types::byte_t empty);

		VolumesVec_t VolumeFinder(const MemoryView& buffer);

	}

}

#endif