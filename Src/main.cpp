#include "main.hpp"

#define EXITSTOP std::cin.get();

//#define FV_Signature_Offset (sizeof(UINT8[16]) + sizeof(EFI_GUID) + sizeof(UINT64))

#define FV_Header_Size (sizeof(EFI_FIRMWARE_VOLUME_HEADER))

int main(int argc, char* argv[]) 
{
	//<Arguments reading>
	{
		Project::printArgs(argc, argv);
		Arguments::parseArgs(argc, argv, opts);

		if (opts[0].data == -1)
			DEBUG_NEW_MESSAGE("ERROR::Main")
				DEBUG_PRINT1("\tMessage: Argument pasing lib doesn't grub input file path from input arguments.");
			DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::NO_INPUT_FILE_PATH)

		arguments.inputFilePath = argv[opts[0].data];
	}
	//</Arguments reading>
	Project::MemoryProxy storage;
	//<Input file reading>
	{
		std::ifstream inputFile( arguments.inputFilePath, 
			std::fstream::binary | std::fstream::in | std::fstream::ate );
		
		if (!inputFile)
			DEBUG_NEW_MESSAGE("ERROR::Main")
				DEBUG_PRINT1("\tMessage: Provided input file can't be opend to read content.");
				DEBUG_PRINT2("\tFile path: ", arguments.inputFilePath);
			DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::CANT_OPEN_INPUT_FILE)

		storage.setLength(inputFile.tellg());
		if (inputFile.tellg() <= 0)
			DEBUG_NEW_MESSAGE("ERROR::Main")
				DEBUG_PRINT1("\tMessage: Invalid file size returned by \'tellg\' call.");
				DEBUG_PRINT2("\tFile size: ", storage.length);
				DEBUG_PRINT2("\t'tellg' call result: ", inputFile.tellg());
			DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::INVALID_FILE_SIZE)
		
		storage = new char[storage.length];
		std::memset(storage, 0, storage.length);
		inputFile.seekg(std::fstream::beg);

		inputFile.read(storage, storage.length);
		if (inputFile.gcount() != storage.length)
			DEBUG_NEW_MESSAGE("ERROR::Main")
				DEBUG_PRINT1("\tMessage: Some error occured during file reading.");
				DEBUG_PRINT2("\tFile size: ", storage.length);
				DEBUG_PRINT2("\tBytes raded: ", inputFile.gcount());
			DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::FILE_SIZE_AND_READ_COUNT_DONT_MATCH)
		
		inputFile.close();
	}
	//</Input file reading>
	storage.setLength(storage.length);
	char FVHeaderSignature[] = "\x00\x00\x00\x00";
	Project::FVHeaderStorage FVHeaders;
	//<FV headers search>
	{
		char* current = storage;
		
		//Not ugly but unsafe way: (livetime of union created by cast is undetermined)
		std::memcpy(FVHeaderSignature, Project::cast_signature_32<EFI_FVH_SIGNATURE>().bytes, 4);
		//The ugly but safe way:
		//*(reinterpret_cast<unsigned int*>(FVHeaderSignature)) = EFI_FVH_SIGNATURE;
		while ( (current = std::search(current, static_cast<char*>(storage.end), static_cast<char*>(FVHeaderSignature), FVHeaderSignature + 4)) != storage.end ) 
		{
			FVHeaders.emplace_back(
				static_cast<std::size_t>(current - storage), 
				reinterpret_cast<Project::FVHeaderPtr>(current - Offsets::PiFVHearedOffsets().Signature)
			);
			++current;
		}

		//Check found headers
		FVHeaders.erase(
			std::remove_if(
				FVHeaders.begin(), 
				FVHeaders.end(), 
				[](const Project::FVHeaderData& value) -> bool 
				{
					//obtain pointer to possible FV header
					auto headerPtr = value.second;
					//Check length clue
					if (headerPtr->HeaderLength < FV_Header_Size) return true;
					//Check checksum clue
					{
						auto checkSumBuff = headerPtr->Checksum;
						headerPtr->Checksum = 0;
						std::uint16_t checkSum = Project::calculateChecksum16(headerPtr, FV_Header_Size);
						headerPtr->Checksum = checkSumBuff;
						if (checkSumBuff != checkSum) return true;
					}
					//Check GUID match clue
					if (Project::isValidEFFSGUID(headerPtr) == -1) return true;
					return false;
				}
			),
			FVHeaders.end()
		);

		//Sort headers by offset increment
		FVHeaders.sort([](const Project::FVHeaderData& a, const Project::FVHeaderData& b) -> bool { return a.first < b.first; });

		//Setup pointers to end of each FV 
		auto iter = FVHeaders.begin();
		++iter;
		for (auto& v : FVHeaders) {
			if (iter != FVHeaders.end()) {
				v.second.setLength(iter->first - v.first);
				++iter;
			} else
				v.second.setLength(storage.length - v.first);
		}
	}
	//</FV headers search>
	//<FV header extension headers search>
	{
		
	}
	//</FV header extension headers search>
	EXITSTOP
	return 0;
}