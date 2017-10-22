#include <main.hpp>

Arguments::Option opts[] = {
	{"f", "file", Arguments::ArgTypes::Value, 0},
	{nullptr, nullptr, Arguments::ArgTypes::Null, 0}
};

int main(int argc, char* argv[]) 
{
	Project::printArgs(argc, argv);
	Arguments::parseArgs(argc, argv, opts);

	std::fstream _inputFile;
	if (opts[0].data == -1)
		DEBUG_NEW_MESSAGE("MAIN::INPUT_FILE_OPTION")
			DEBUG_PRINT1("\tCan't open input file: No file path provided.");
		DEBUG_END_MESSAGE_AND_EXIT(-1)
	else
		_inputFile.open(argv[opts[0].data], std::fstream::in | std::fstream::binary);

	if (_inputFile) {
		std::cout << "Success\n";
		std::cout << "\tFile path: " << argv[opts[0].data] << std::endl;
		_inputFile.close();
	}

	std::cin.get();
	return 0;
}