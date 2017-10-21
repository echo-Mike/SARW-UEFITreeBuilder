#include <main.hpp>
#include <mutex>

int main(int argc, char* argv[]) 
{
	Project::printArgs(argc, argv);
	std::cin.get();
	return 0;
}