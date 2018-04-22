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
			NoInputFilePath,
			CantOpenInputFile,
			InvalidFileSize,
			FileSizeAndReadCountDontMatch,
			ParseErrorFreeSpace = 100,
			ParseErrorVolume = 200,
			ParseErrorFile = 300,
			ParseErrorSection = 400
		};
	}
}


#endif