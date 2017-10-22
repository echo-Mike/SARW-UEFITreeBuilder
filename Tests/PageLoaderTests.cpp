#include "cPageLoader.hpp"
#include "mDebuglib.hpp"
#include "fArgumentLib.hpp"

#define PAGE_SIZE 0xc000

Arguments::Option opts[] = {
	{"f", "file", Arguments::ArgTypes::Value, 0},
	{nullptr, nullptr, Arguments::ArgTypes::Null, 0}
};

int DefaultConstructorTest() {
    try {
        Project::PageLoader inputFileReader(PAGE_SIZE);
    }
    catch (...) {
        return 0;
    }
    return 1;
}

int main(int argc, char* argv[])
{
    Arguments::parseArgs(argc, argv, opts);
    
    int _testCounter;

    Project::PageLoader inputFileReader(PAGE_SIZE);

    if (opts[0].data == -1)
		DEBUG_NEW_MESSAGE("MAIN::INPUT_FILE_OPTION")
			DEBUG_PRINT1("\tCan't open input file: No file path provided.");
		DEBUG_END_MESSAGE_AND_EXIT(-1)
	else
		inputFileReader.open(argv[opts[0].data], std::fstream::in | std::fstream::binary);

    if (inputFileReader) {
		std::cout << "Success\n";
		std::cout << "\tFile path: " << argv[opts[0].data] << std::endl;
		
		if (inputFileReader.nextPage())
			std::cout << "Page read success" << std::endl;
		
		int _counter = 0;
		while (inputFileReader.nextPage())
			++_counter;

		std::cout << "Total pages: " << _counter + 1 << "\n";
		std::cout << "Last page size: " << inputFileReader.currentPayload << std::endl;

		inputFileReader.close();
	}

	std::cin.get();
	return 0;
}