#pragma once
#ifndef FILE_FINDER_HPP__
#define	FILE_FINDER_HPP__ "0.0.0@FileFinder.hpp"

/// STD
#include <vector>

/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"

namespace Project
{
	namespace Finders
	{

		typedef std::vector< Pi::File::Header > FilesVec_t;

		struct FileFinder
		{

			FilesVec_t operator()(const MemoryView& buffer, Types::memory_t empty);
		};

	}
}

#endif