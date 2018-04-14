#pragma once
#ifndef SECTION_PARSER_HPP__
#define	SECTION_PARSER_HPP__ "0.0.0@SectionParser.hpp"

/// STD

/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"
#include "PiObjectRepresentation.hpp"

namespace Project
{
	namespace Parsers
	{

		struct SectionParser
		{

			PiObject::Section operator()(const Pi::Section::Header& sectionView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty);

			PiObject::Section operator()(const Pi::Section::Extended::Header& sectionView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty);
		};

	}
}

#endif
