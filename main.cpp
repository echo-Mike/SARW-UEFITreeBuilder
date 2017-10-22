#include <main.hpp>

Arguments::Option opts[] = {
	{"f", "file", Arguments::ArgTypes::Value, 0},
	{nullptr, nullptr, Arguments::ArgTypes::Null, 0}
};

int main(int argc, char* argv[]) 
{
	Project::printArgs(argc, argv);
	Arguments::parseArgs(argc, argv, opts);

	std::fstream _inputFile(argv[opts[0].data], std::fstream::in);
	if (_inputFile) {
		std::cout << "Success\n";
		std::cout << "\tFile path: " << argv[opts[0].data] << std::endl;
		_inputFile.close();
	}

	std::cin.get();
	return 0;
}