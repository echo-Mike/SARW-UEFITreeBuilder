#pragma once
#ifndef PI_PARSERS_HPP__
#define PI_PARSERS_HPP__ "0.0.0@PiParsers.hpp"

/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"
#include "PiObjectRepresentation.hpp"

namespace Project
{

	namespace Parsers
	{

		PiObject::Section SectionParser(const Pi::Section::Header& sectionView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty);

		PiObject::File FileParser(const Pi::File::Header& fileView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty);

		PiObject::Volume VolumeParser(const Pi::Volume::Header& volumeView, const MemoryView& buffer, const MemoryView& baseBuffer);

	}

}

#endif