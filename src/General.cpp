/// STD
#include <exception>
/// PROJECT
#include "General.hpp"

namespace Project
{
	void printArgs(int argc, char* argv[]) 
	{
		DEBUG_INFO_MESSAGE
			for (auto index = 0; index < argc; index++)
				DEBUG_PRINT("\t", index, ") Argument: ", argv[index]);
		DEBUG_END_MESSAGE
	}

	namespace Checksums
	{
		std::uint16_t calc16(Types::const_pointer_t ptr, Types::length_t length)
		{
			std::uint16_t result = 0;
			auto* data = reinterpret_cast<const std::uint16_t*>(ptr);
			if (length % sizeof(std::uint16_t) != 0)
			{
				throw std::invalid_argument("Invalid length of data");
			}
			for (std::size_t index = 0; index < length / sizeof(std::uint16_t); ++index)
			{
				result += data[index];
			}
			return static_cast<std::uint16_t>(-result);
		}

		Types::hash_t hash(Types::const_pointer_t ptr, Types::length_t length)
		{	// For C++17 std::string_view must be used
			std::hash<std::string> h;
			return h(std::string(reinterpret_cast<const char*>(ptr), length));
		}
	}
}