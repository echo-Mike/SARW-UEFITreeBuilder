#include <main.hpp>

#define EXITSTOP std::cin.get();

#define FV_Signature_Offset (sizeof(UINT8[16]) + sizeof(EFI_GUID) + sizeof(UINT64))

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
	char* storage = nullptr;
	char* storageBeg = nullptr;
	char* storageEnd = nullptr;
	std::size_t fileSize = 0;
	//<Input file reading>

	{
		std::ifstream inputFile( arguments.inputFilePath, 
			std::fstream::binary | std::fstream::in | std::fstream::ate );
		
		if (!inputFile)
			DEBUG_NEW_MESSAGE("ERROR::Main")
				DEBUG_PRINT1("\tMessage: Provided input file can't be opend to read content.");
				DEBUG_PRINT2("\tFile path: ", arguments.inputFilePath);
			DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::CANT_OPEN_INPUT_FILE)

		fileSize = inputFile.tellg();
		if (fileSize <= 0)
			DEBUG_NEW_MESSAGE("ERROR::Main")
				DEBUG_PRINT1("\tMessage: Invalid file size returned by \'tellg\' call.");
				DEBUG_PRINT2("\tFile size: ", fileSize);
				DEBUG_PRINT2("\t'tellg' call result: ", inputFile.tellg());
			DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::INVALID_FILE_SIZE)
		
		storage = new char[fileSize];
		std::memset(storage, 0, fileSize);
		inputFile.seekg(std::fstream::beg);

		inputFile.read(storage, fileSize);
		if (inputFile.gcount() != fileSize)
			DEBUG_NEW_MESSAGE("ERROR::Main")
				DEBUG_PRINT1("\tMessage: Some error occured during file reading.");
				DEBUG_PRINT2("\tFile size: ", fileSize);
				DEBUG_PRINT2("\tBytes raded: ", inputFile.gcount());
			DEBUG_END_MESSAGE_AND_EXIT(Project::ExitCodes::FILE_SIZE_AND_READ_COUNT_DONT_MATCH)
		
		inputFile.close();
	}

	//</Input file reading>
	storageBeg = storage;
	storageEnd = storage + fileSize;
	char FVHeaderSignature[] = "\x00\x00\x00\x00";
	Project::FVHeaderStorage FVHeaders;
	//<FV headers search>

	{
		char* current = storageBeg;
		
		//Not ugly but unsafe way: (livetime of union created by cast is undetermined)
		std::memcpy(FVHeaderSignature, Project::cast_signature_32<EFI_FVH_SIGNATURE>().bytes, 4);
		//The ugly but safe way:
		//*(reinterpret_cast<unsigned int*>(FVHeaderSignature)) = EFI_FVH_SIGNATURE;
		while ( (current = std::search(current, storageEnd, FVHeaderSignature, FVHeaderSignature + 4)) != storageEnd ) 
		{
			FVHeaders.emplace_back(
				static_cast<std::size_t>(current - storageBeg), 
				reinterpret_cast<Project::FVHeaderPtr>(current - FV_Signature_Offset)
			);
			++current;
		}

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
	}
	
	//</FV headers search>

	EXITSTOP
	return 0;
}