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

		struct FileParser
		{

			PiObject::Section operator()(const Pi::Section::Header& volumeView, const MemoryView& buffer);

			PiObject::Section operator()(const Pi::Section::Extended::Header& volumeView, const MemoryView& buffer);
		};

	}
}

#endif
