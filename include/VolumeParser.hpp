#pragma once
#ifndef VOLUME_PARSER_HPP__
#define	VOLUME_PARSER_HPP__ "0.0.0@VolumeParser.hpp"

/// STD

/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"
#include "PiObjectRepresentation.hpp"

namespace Project
{
	namespace Parsers
	{

		struct VolumeParser
		{

			PiObject::Volume operator()(const Pi::Volume::Header& volumeView, const MemoryView& buffer, const MemoryView& baseBuffer);

		};

	}
}

#endif
