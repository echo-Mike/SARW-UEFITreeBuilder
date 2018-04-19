/// STD
#include <exception>
/// PROJECT
#include "General.hpp"
#include "Edk2Crc32.h"

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

		std::uint8_t calc8(Types::const_pointer_t ptr, Types::length_t length)
		{
			std::uint8_t result = 0;
			auto data = reinterpret_cast<const std::uint8_t*>(ptr);
			for (std::size_t index = 0; index < length; ++index)
			{
				result += data[index];
			}
			return static_cast<std::uint8_t>(0x100 - result);
		}

		Types::hash_t hash(Types::const_pointer_t ptr, Types::length_t length)
		{	// For C++17 std::string_view must be used
			std::hash<std::string> h;
			return h(std::string(reinterpret_cast<const char*>(ptr), length));
		}

		std::uint32_t crc32(Types::const_pointer_t ptr, Types::length_t length, std::uint32_t init)
		{
			if (!ptr) {
				DEBUG_ERROR_MESSAGE
					DEBUG_PRINT("\tMessage: Provided ptr to data is nullptr.");
				DEBUG_END_MESSAGE_AND_EVAL({ return 0; })
			}
			if (!length) {
				DEBUG_ERROR_MESSAGE
					DEBUG_PRINT("\tMessage: Can't calculate CRC32 on zero length array.");
				DEBUG_END_MESSAGE_AND_EVAL({ return 0; })
			}
			UINT32 crcOut = 0;
			auto dataPtr = reinterpret_cast<UINT8*>(const_cast<Types::pointer_t>(ptr));
			auto status = CalculateCrc32(dataPtr, length, &crcOut);

			if (status != EFI_SUCCESS) {
				DEBUG_ERROR_MESSAGE
					DEBUG_PRINT("\tMessage: CalculateCrc32 returned unexpected status code.");
					DEBUG_PRINT("\tStatus code: ", status);
				DEBUG_END_MESSAGE_AND_EVAL({ return 0; })
			}
			return crcOut;
		}
	}
}