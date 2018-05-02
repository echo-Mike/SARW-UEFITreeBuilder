#include "main.hpp"

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

		if (opts[1].data == -1) DEBUG_INFO_MESSAGE
			DEBUG_PRINT("\tMessage: No output file name provided. Using default.");
			DEBUG_PRINT("\tDefault file name: \"./out.json\"");
		DEBUG_END_MESSAGE_AND_EVAL({
			arguments.inputFilePath = "./out.json";
		}) 
		else 
		{
			arguments.inputFilePath = argv[opts[1].data];
		}
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
	Project::PiObject::object_vec_t parseResult;
	//<Parsing>
	try {
		parseResult = Project::Parsers::FreeSpaceParser(storage.memory);
	}
	catch (const std::exception& e)
	{
		DEBUG_ERROR_MESSAGE
			DEBUG_PRINT("\tMessage: Error occurred during parsing.");
			DEBUG_PRINT("\tException message: ", e.what());
		DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::ParsingError)
	}
	if (parseResult.empty()) {
		std::exit(Project::ExitCodes::NothingFound);
	}
	//</Parsing>
	//<Convert to JSON>
	nlohmann::json j = parseResult;
	//</Convert to JSON>
	//<Output JSON>
	{
		std::ofstream outputFile(arguments.outputFilePath);
		if (!outputFile) {
			std::exit(Project::ExitCodes::CantOpenOutputFile);
		}
		outputFile << j << std::flush;
	}
	//</Output JSON>
	EXITSTOP
	return 0;
}