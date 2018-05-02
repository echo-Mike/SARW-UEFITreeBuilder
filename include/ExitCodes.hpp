#pragma once
#ifndef EXIT_CODES_HPP__
#define	EXIT_CODES_HPP__ "0.0.0@ExitCodes.hpp"

/// STD
#include <cstdlib>
#include <cstdint>

namespace Project
{
	namespace ExitCodes 
	{
		enum ExitCodes_t : int
		{
			Normal = 0,
			NothingFound = 1,
			NoInputFilePath,
			CantOpenInputFile,
			CantOpenOutputFile,
			InvalidFileSize,
			FileSizeAndReadCountDontMatch,
			ParsingError,
			ParseErrorFreeSpace = 100,
			ParseErrorVolume = 200,
			ParseErrorFile = 300,
			ParseErrorSection = 400,
		};
	}
}


#endif