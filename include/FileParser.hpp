#pragma once
#ifndef FILE_PARSER_HPP__
#define	FILE_PARSER_HPP__ "0.0.0@FileParser.hpp"

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

			PiObject::File operator()(const Pi::File::Header& fileView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty);

			PiObject::File operator()(const Pi::File::Extended::Header& fileView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty);
		};

	}
}

#endif
