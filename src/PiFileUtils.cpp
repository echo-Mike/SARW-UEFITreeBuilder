/// STD
#include <cstdio>

/// PROJECT
#include "PiFileUtils.hpp"

#define GET_END_PTR(Array, Type) ( Array + ( sizeof(Array) / sizeof(Type) ) )

#define PROJ_FILE_TYPE_TO_STR(MACRO_) { MACRO_, #MACRO_ }
#define PROJ_FILE_TYPE_TO_STR2(MACRO_, Str) { MACRO_, #MACRO_ ## Str }

#define NOT_EFI_FV_FILETYPE (EFI_FV_FILETYPE_OEM_MIN - 1)

namespace Project
{
	namespace Pi 
	{
		namespace File
		{
			namespace Utils
			{
				namespace Helper
				{

					static const Types::memory_t NormalFileTypes[] =
					{
						// EFI_FV_FILETYPE_ALL, // Not used in real files: marker for function calls
						EFI_FV_FILETYPE_RAW,
						EFI_FV_FILETYPE_FREEFORM,
						EFI_FV_FILETYPE_SECURITY_CORE,
						EFI_FV_FILETYPE_PEI_CORE,
						EFI_FV_FILETYPE_DXE_CORE,
						EFI_FV_FILETYPE_PEIM,
						EFI_FV_FILETYPE_DRIVER,
						EFI_FV_FILETYPE_COMBINED_PEIM_DRIVER,
						EFI_FV_FILETYPE_APPLICATION,
						EFI_FV_FILETYPE_SMM,
						EFI_FV_FILETYPE_FIRMWARE_VOLUME_IMAGE,
						EFI_FV_FILETYPE_COMBINED_SMM_DXE,
						EFI_FV_FILETYPE_SMM_CORE,
						EFI_FV_FILETYPE_SMM_STANDALONE,
						EFI_FV_FILETYPE_SMM_CORE_STANDALONE
						// EFI_FV_FILETYPE_FFS_PAD // Must be in FFS file range as EFI_FV_FILETYPE_FFS_MIN
					};

					static const Types::memory_t SectionedFileTypes[] =
					{
						// EFI_FV_FILETYPE_RAW,
						EFI_FV_FILETYPE_FREEFORM,
						// EFI_FV_FILETYPE_SECURITY_CORE,
						EFI_FV_FILETYPE_PEI_CORE,
						EFI_FV_FILETYPE_DXE_CORE,
						EFI_FV_FILETYPE_PEIM,
						EFI_FV_FILETYPE_DRIVER,
						EFI_FV_FILETYPE_COMBINED_PEIM_DRIVER,
						EFI_FV_FILETYPE_APPLICATION,
						EFI_FV_FILETYPE_SMM,
						EFI_FV_FILETYPE_FIRMWARE_VOLUME_IMAGE,
						EFI_FV_FILETYPE_COMBINED_SMM_DXE,
						EFI_FV_FILETYPE_SMM_CORE,
						EFI_FV_FILETYPE_SMM_STANDALONE
						// EFI_FV_FILETYPE_SMM_CORE_STANDALONE,
						// EFI_FV_FILETYPE_FFS_PAD
					};

					struct FileMacroPlusName
					{
						Types::memory_t type;
						const char* name;
					};

					static const FileMacroPlusName FileNames[] =
					{
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_RAW),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_FREEFORM),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_SECURITY_CORE),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_PEI_CORE),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_DXE_CORE),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_PEIM),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_DRIVER),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_COMBINED_PEIM_DRIVER),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_APPLICATION),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_SMM),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_FIRMWARE_VOLUME_IMAGE),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_COMBINED_SMM_DXE),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_SMM_CORE),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_SMM_STANDALONE),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_SMM_CORE_STANDALONE),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_OEM_MIN),
						PROJ_FILE_TYPE_TO_STR2(EFI_FV_FILETYPE_OEM_MIN, " + %X "),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_OEM_MAX),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_DEBUG_MIN),
						PROJ_FILE_TYPE_TO_STR2(EFI_FV_FILETYPE_DEBUG_MIN, " + %X "),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_DEBUG_MAX),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_FFS_MIN),
						PROJ_FILE_TYPE_TO_STR2(EFI_FV_FILETYPE_FFS_MIN, " + %X "),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_FFS_MAX),
						PROJ_FILE_TYPE_TO_STR(EFI_FV_FILETYPE_FFS_PAD),
						PROJ_FILE_TYPE_TO_STR(NOT_EFI_FV_FILETYPE)
					};

					static char filenameBuffer[128];

					std::size_t fileNamePosition(Types::memory_t type)
					{
						switch (type)
						{
							case EFI_FV_FILETYPE_RAW:
								return 0;
							case EFI_FV_FILETYPE_FREEFORM:
								return 1;
							case EFI_FV_FILETYPE_SECURITY_CORE:
								return 2;
							case EFI_FV_FILETYPE_PEI_CORE:
								return 3;
							case EFI_FV_FILETYPE_DXE_CORE:
								return 4;
							case EFI_FV_FILETYPE_PEIM:
								return 5;
							case EFI_FV_FILETYPE_DRIVER:
								return 6;
							case EFI_FV_FILETYPE_COMBINED_PEIM_DRIVER:
								return 7;
							case EFI_FV_FILETYPE_APPLICATION:
								return 8;
							case EFI_FV_FILETYPE_SMM:
								return 9;
							case EFI_FV_FILETYPE_FIRMWARE_VOLUME_IMAGE:
								return 10;
							case EFI_FV_FILETYPE_COMBINED_SMM_DXE:
								return 11;
							case EFI_FV_FILETYPE_SMM_CORE:
								return 12;
							case EFI_FV_FILETYPE_SMM_STANDALONE:
								return 13;
							case EFI_FV_FILETYPE_SMM_CORE_STANDALONE:
								return 14;
							case EFI_FV_FILETYPE_OEM_MIN:
								return 15;
							case EFI_FV_FILETYPE_OEM_MAX:
								return 17;
							case EFI_FV_FILETYPE_DEBUG_MIN:
								return 18;
							case EFI_FV_FILETYPE_DEBUG_MAX:
								return 20;
							case EFI_FV_FILETYPE_FFS_MIN:
								return 21;
							case EFI_FV_FILETYPE_FFS_MAX:
								return 23;
							case NOT_EFI_FV_FILETYPE:
								return sizeof(FileNames) / sizeof(FileMacroPlusName) - 1;
							default:
								return sizeof(FileNames) / sizeof(FileMacroPlusName);
								break;
						}
					}

				}

				FileTypeRanges::FileTypeRange_t whatTypeRange(Types::memory_t type)
				{
					using namespace Helper;

					auto end = GET_END_PTR(NormalFileTypes, Types::memory_t);
					if (end != std::find(NormalFileTypes, end, type)) {
						return FileTypeRanges::Normal;
					} else if (EFI_FV_FILETYPE_OEM_MIN <= type && type <= EFI_FV_FILETYPE_OEM_MAX) {
						return FileTypeRanges::OEM;
					} else if (EFI_FV_FILETYPE_DEBUG_MIN <= type && type <= EFI_FV_FILETYPE_DEBUG_MAX) {
						return FileTypeRanges::Debug;
					} else if (EFI_FV_FILETYPE_FFS_MIN <= type && type <= EFI_FV_FILETYPE_FFS_MAX) {
						return FileTypeRanges::FFS;
					}

					return FileTypeRanges::NotAFileType;
				}

				bool isSectionedFileType(Types::memory_t type)
				{
					using namespace Helper;
					auto end = GET_END_PTR(SectionedFileTypes, Types::memory_t);
					return end != std::find(SectionedFileTypes, end, type);
				}

				const char* fileTypeToCStr(Types::memory_t type)
				{
					using namespace Helper;
					if (type == EFI_FV_FILETYPE_FFS_PAD) {
						return FileNames[sizeof(FileNames) / sizeof(FileMacroPlusName) - 2].name;
					}
					auto end = GET_END_PTR(FileNames, FileMacroPlusName);
					auto pos = fileNamePosition(type);
					if (FileNames + pos != end) {
						if (FileNames[pos].type != type)
							DEBUG_WARNING_MESSAGE
								DEBUG_PRINT("\tMessage: Found type do not correspond to file type.");
								DEBUG_PRINT("\tFile type: ", type);
								DEBUG_PRINT("\tType name found: ", FileNames[pos].name);
							DEBUG_END_MESSAGE

						return FileNames[pos].name;
					} else {
						std::memset(filenameBuffer, 0, sizeof(filenameBuffer) * sizeof(char));
						if (EFI_FV_FILETYPE_OEM_MIN < type && type < EFI_FV_FILETYPE_OEM_MAX) {
							pos = fileNamePosition(EFI_FV_FILETYPE_OEM_MIN) + 1;
							std::snprintf(filenameBuffer, sizeof(filenameBuffer) / sizeof(char), FileNames[pos].name, type - EFI_FV_FILETYPE_OEM_MIN);
							return filenameBuffer;
						} else if (EFI_FV_FILETYPE_DEBUG_MIN < type && type < EFI_FV_FILETYPE_DEBUG_MAX) {
							pos = fileNamePosition(EFI_FV_FILETYPE_DEBUG_MIN) + 1;
							std::snprintf(filenameBuffer, sizeof(filenameBuffer) / sizeof(char), FileNames[pos].name, type - EFI_FV_FILETYPE_DEBUG_MIN);
							return filenameBuffer;
						} else if (EFI_FV_FILETYPE_FFS_MIN < type && type < EFI_FV_FILETYPE_FFS_MAX) {
							pos = fileNamePosition(EFI_FV_FILETYPE_FFS_MIN) + 1;
							std::snprintf(filenameBuffer, sizeof(filenameBuffer) / sizeof(char), FileNames[pos].name, type - EFI_FV_FILETYPE_FFS_MIN);
							return filenameBuffer;
						} else {
							return FileNames[fileNamePosition(NOT_EFI_FV_FILETYPE)].name;
						}
					}
				}

			}
		}
	}
}

#undef NOT_EFI_FV_FILETYPE

#undef PROJ_FILE_TYPE_TO_STR2
#undef PROJ_FILE_TYPE_TO_STR

#undef GET_END_PTR