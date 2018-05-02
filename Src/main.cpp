#include "main.hpp"
#include "MemoryView.hpp"

#define EXITSTOP std::cin.get();

int main(int argc, char* argv[]) 
{
	//<Arguments reading>
	{
		Project::printArgs(argc, argv);
		Arguments::parseArgs(argc, argv, opts);

		if (opts[0].data == -1) DEBUG_ERROR_MESSAGE
			DEBUG_PRINT("\tMessage: Argument parsing lib does not grub input file path from input arguments.");
		DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::NoInputFilePath)

		arguments.inputFilePath = argv[opts[0].data];
	}
	//</Arguments reading>
	Project::Types::BufferWithView storage;
	//<Input file reading>
	{
		std::ifstream inputFile( arguments.inputFilePath, 
			std::fstream::binary | std::fstream::in | std::fstream::ate );
		
		if (!inputFile) DEBUG_ERROR_MESSAGE
			DEBUG_PRINT("\tMessage: Provided input file can't be opened to read content.");
			DEBUG_PRINT("\tFile path: ", arguments.inputFilePath);
		DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::CantOpenInputFile)

		Project::Types::length_t file_length = inputFile.tellg();
		if (inputFile.tellg() == std::ifstream::pos_type(-1)) DEBUG_ERROR_MESSAGE
			DEBUG_PRINT("\tMessage: Invalid file size returned by \"tellg\" call.");
			DEBUG_PRINT("\t'\"tellg\" call result: ", inputFile.tellg());
		DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::InvalidFileSize)
		
		storage.reset(file_length);
		inputFile.seekg(std::fstream::beg);
		inputFile.read(reinterpret_cast<char*>(storage.get()), storage.getLength());
		if (inputFile.gcount() != storage.getLength()) DEBUG_ERROR_MESSAGE
			DEBUG_PRINT("\tMessage: Some error occurred during file reading.");
			DEBUG_PRINT("\tFile size: ", storage.getLength());
			DEBUG_PRINT("\tBytes read: ", inputFile.gcount());
		DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::FileSizeAndReadCountDontMatch)
		
		inputFile.close();
	}
	//</Input file reading>
	EXITSTOP
	return 0;
}