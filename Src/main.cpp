#include <main.hpp>

Arguments::Option opts[] = {
	{"f", "file", Arguments::ArgTypes::Value, 0},
	{nullptr, nullptr, Arguments::ArgTypes::Null, 0}
};

int main(int argc, char* argv[]) 
{
	Project::printArgs(argc, argv);
	Arguments::parseArgs(argc, argv, opts);

	std::cin.get();
	return 0;
}