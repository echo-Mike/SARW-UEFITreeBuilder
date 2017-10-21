#include <main.hpp>

int main(int argc, char* argv[]) 
{
	int _value = 1;
	std::cout << _value + _value << std::endl;
	for (auto _index = 0; _index < argc; _index++)
		std::cout << argv[_index] << std::endl;
	std::cin.get();
	return 1;
}