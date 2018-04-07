#include "main.hpp"

#define EXITSTOP std::cin.get();

int main(int argc, char* argv[]) 
{
	//<Arguments reading>
	{
		Project::printArgs(argc, argv);
		Arguments::parseArgs(argc, argv, opts);

		if (opts[0].data == -1)
			DEBUG_NEW_MESSAGE("ERROR::Main")
				DEBUG_PRINT1("\tMessage: Argument passing lib doesn't grub input file path from input arguments.");
			DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::NoInputFilePath)

		arguments.inputFilePath = argv[opts[0].data];
	}
	//</Arguments reading>
	//<Input file reading>
	{
		std::ifstream inputFile( arguments.inputFilePath, 
			std::fstream::binary | std::fstream::in | std::fstream::ate );
		
		if (!inputFile)
			DEBUG_NEW_MESSAGE("ERROR::Main")
				DEBUG_PRINT1("\tMessage: Provided input file can't be opened to read content.");
				DEBUG_PRINT2("\tFile path: ", arguments.inputFilePath);
			DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::CantOpenInputFile)

		storage.setLength(inputFile.tellg());
		if (inputFile.tellg() <= 0)
			DEBUG_NEW_MESSAGE("ERROR::Main")
				DEBUG_PRINT1("\tMessage: Invalid file size returned by \'tellg\' call.");
				DEBUG_PRINT2("\tFile size: ", storage.length);
				DEBUG_PRINT2("\t'tellg' call result: ", inputFile.tellg());
			DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::InvalidFileSize)
		
		storage = new char[storage.length];
		std::memset(storage, 0, storage.length);
		inputFile.seekg(std::fstream::beg);

		inputFile.read(storage, storage.length);
		if (inputFile.gcount() != storage.length)
			DEBUG_NEW_MESSAGE("ERROR::Main")
				DEBUG_PRINT1("\tMessage: Some error occurred during file reading.");
				DEBUG_PRINT2("\tFile size: ", storage.length);
				DEBUG_PRINT2("\tBytes read: ", inputFile.gcount());
			DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::FileSizeAndReadCountDontMatch)
		
		inputFile.close();
	}
	//</Input file reading>
	EXITSTOP
	return 0;
}