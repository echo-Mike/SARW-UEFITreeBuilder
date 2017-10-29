#include "cPageLoader.hpp"
#include "mDebuglib.hpp"
#include "fArgumentLib.hpp"

#define PAGE_SIZE 0xc000

Arguments::Option opts[] = {
	{"f", "file", Arguments::ArgTypes::Value, 0},
	{nullptr, nullptr, Arguments::ArgTypes::Null, 0}
};

int DefaultConstructor() {
    try {
        Project::PageLoader inputFileReader(PAGE_SIZE);
    }
    catch (...) {
        DEBUG_NEW_MESSAGE("DefaultConstructor")
        DEBUG_END_MESSAGE
        return 0;
    }
    return 1;
}

int ConstructionFromFileName(char* argv[]) {
    try {
        Project::PageLoader inputFileReader(argv[opts[0].data], PAGE_SIZE);
    }
    catch (...) {
        DEBUG_NEW_MESSAGE("ConstructionFromFileName")
        DEBUG_END_MESSAGE
        return 0;
    }
    return 1;
}

int NextPageTest(char* argv[]) {
    try {
        Project::PageLoader inputFileReader(argv[opts[0].data], PAGE_SIZE);
        if (!inputFileReader.nextPage()) return 0;
    }
    catch (...) {
        DEBUG_NEW_MESSAGE("NextPageTest")
        DEBUG_END_MESSAGE
        return 0;
    }
    return 1;
}

int WholeFileReadTest(char* argv[]) {
    try {
        Project::PageLoader inputFileReader(argv[opts[0].data], PAGE_SIZE);
        int _counter = 0;
		while (inputFileReader.nextPage())
            ++_counter;
        DEBUG_NEW_MESSAGE("WholeFileReadTest: ")
            DEBUG_PRINT2("\tTotal pages readed: ", ++_counter);
            DEBUG_PRINT2("\tSize of last page: ", inputFileReader.currentPayload);
        DEBUG_END_MESSAGE
    }
    catch (...) {
        DEBUG_NEW_MESSAGE("WholeFileReadTest")
        DEBUG_END_MESSAGE
        return 0;
    }
    return 1;
}

int mergeWithNextPageTest(char* argv[]) {
    try {
        Project::PageLoader inputFileReader(argv[opts[0].data], PAGE_SIZE);
        for (int i = 0; i < 10; ++i)
            inputFileReader.nextPage();
        auto _buffer = inputFileReader.mergeWithNextPage();
        return _buffer.first == PAGE_SIZE * 2;
    }
    catch (...) {
        DEBUG_NEW_MESSAGE("mergeWithNextPageTest")
        DEBUG_END_MESSAGE
        return 0;
    }
    return 1;
}

int mergeWithNextPageBuferisedTest(char* argv[]) {
    try {
        Project::PageLoader inputFileReader(argv[opts[0].data], PAGE_SIZE);
        inputFileReader.nextPage();
        auto _buffer = inputFileReader.mergeWithNextPage();
        for (int i = 0; i < 10; ++i)
            _buffer = inputFileReader.mergeWithNextPage(std::move(_buffer)); 
        return _buffer.first == PAGE_SIZE * 12;
    }
    catch (...) {
        DEBUG_NEW_MESSAGE("mergeWithNextPageTest")
        DEBUG_END_MESSAGE
        return 0;
    }
    return 1;
}

int main(int argc, char* argv[])
{
    Arguments::parseArgs(argc, argv, opts);
    
    if (opts[0].data == -1)
		DEBUG_NEW_MESSAGE("MAIN::INPUT_FILE_OPTION")
			DEBUG_PRINT1("\tCan't open input file: No file path provided.");
		DEBUG_END_MESSAGE_AND_EXIT(-1)

    int _passedTests = 0;
    _passedTests += DefaultConstructor();
    _passedTests += ConstructionFromFileName(argv);
    _passedTests += NextPageTest(argv);
    _passedTests += WholeFileReadTest(argv);
    _passedTests += mergeWithNextPageTest(argv);
    _passedTests += mergeWithNextPageBuferisedTest(argv);
    
    DEBUG_NEW_MESSAGE("Test statistics: ")
        DEBUG_PRINT3("\tTests passed ", _passedTests, " out of 6");
    DEBUG_END_MESSAGE
	std::cin.get();
	return 0;
}