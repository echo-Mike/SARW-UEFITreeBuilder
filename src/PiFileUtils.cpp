/// STD
#include <cstdio>

/// PROJECT
#include "PiFileUtils.hpp"
#include "PiSectionUtils.hpp"
#include "PiSectionObject.hpp"
#include "MSVCCodecvtFix.hpp"

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

					static const Types::byte_t NormalFileTypes[] =
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

					static const Types::byte_t SectionedFileTypes[] =
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
						Types::byte_t type;
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

					std::size_t fileNamePosition(Types::byte_t type)
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

					static const char* getFileDataAlignment(Pi::File::Header::const_pointer_t header)
					{
						if (header->Attributes & FFS_ATTRIB_DATA_ALIGNMENT2)
						{
							switch (header->Attributes & FFS_ATTRIB_DATA_ALIGNMENT)
							{
								case FFS_FILE_DATA_ALIGNMENT_128K : return "128K";
								case FFS_FILE_DATA_ALIGNMENT_256K : return "256K";
								case FFS_FILE_DATA_ALIGNMENT_512K : return "512K";
								case FFS_FILE_DATA_ALIGNMENT_1M   : return "1M";
								case FFS_FILE_DATA_ALIGNMENT_2M   : return "2M";
								case FFS_FILE_DATA_ALIGNMENT_4M   : return "4M";
								case FFS_FILE_DATA_ALIGNMENT_8M   : return "8M";
								case FFS_FILE_DATA_ALIGNMENT_16M  : return "16M";
								default                           : return "Unknown";
							}
						}
						else
						{
							switch (header->Attributes & FFS_ATTRIB_DATA_ALIGNMENT)
							{
								case FFS_FILE_DATA_ALIGNMENT_1   : return "1B";
								case FFS_FILE_DATA_ALIGNMENT_16  : return "16B";
								case FFS_FILE_DATA_ALIGNMENT_128 : return "128B";
								case FFS_FILE_DATA_ALIGNMENT_512 : return "512B";
								case FFS_FILE_DATA_ALIGNMENT_1K  : return "1K";
								case FFS_FILE_DATA_ALIGNMENT_4K  : return "4K";
								case FFS_FILE_DATA_ALIGNMENT_32K : return "32K";
								case FFS_FILE_DATA_ALIGNMENT_64K : return "64K";
								default                          : return "Unknown";
							}
						}
					}

				}

				FileTypeRanges::FileTypeRange_t whatTypeRange(Types::byte_t type)
				{
					using namespace Helper;

					auto end = GET_END_PTR(NormalFileTypes, Types::byte_t);
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

				bool isSectionedFileType(Types::byte_t type)
				{
					using namespace Helper;
					auto end = GET_END_PTR(SectionedFileTypes, Types::byte_t);
					return end != std::find(SectionedFileTypes, end, type);
				}

				const char* fileTypeToCStr(Types::byte_t type)
				{
					using namespace Helper;
					if (type == EFI_FV_FILETYPE_FFS_PAD) {
						return FileNames[sizeof(FileNames) / sizeof(FileMacroPlusName) - 2].name;
					}
					auto end = GET_END_PTR(FileNames, FileMacroPlusName);
					auto pos = fileNamePosition(type);
					if (FileNames + pos != end) {
						if (FileNames[pos].type != type) DEBUG_WARNING_MESSAGE
							DEBUG_PRINT("\tMessage: Found type do not correspond to file type.");
							DEBUG_PRINT("\tFile type: ", type);
							DEBUG_PRINT("\tType name found: ", FileNames[pos].name);
						DEBUG_END_MESSAGE

						return FileNames[pos].name;
					} else {
						std::memset(filenameBuffer, 0, sizeof(filenameBuffer));
						if (EFI_FV_FILETYPE_OEM_MIN < type && type < EFI_FV_FILETYPE_OEM_MAX) 
						{
							pos = fileNamePosition(EFI_FV_FILETYPE_OEM_MIN) + 1;
							std::snprintf(filenameBuffer, sizeof(filenameBuffer), FileNames[pos].name, type - EFI_FV_FILETYPE_OEM_MIN);
							return filenameBuffer;
						} 
						else if (EFI_FV_FILETYPE_DEBUG_MIN < type && type < EFI_FV_FILETYPE_DEBUG_MAX) 
						{
							pos = fileNamePosition(EFI_FV_FILETYPE_DEBUG_MIN) + 1;
							std::snprintf(filenameBuffer, sizeof(filenameBuffer), FileNames[pos].name, type - EFI_FV_FILETYPE_DEBUG_MIN);
							return filenameBuffer;
						} 
						else if (EFI_FV_FILETYPE_FFS_MIN < type && type < EFI_FV_FILETYPE_FFS_MAX) 
						{
							pos = fileNamePosition(EFI_FV_FILETYPE_FFS_MIN) + 1;
							std::snprintf(filenameBuffer, sizeof(filenameBuffer), FileNames[pos].name, type - EFI_FV_FILETYPE_FFS_MIN);
							return filenameBuffer;
						} 
						else 
						{
							return FileNames[fileNamePosition(NOT_EFI_FV_FILETYPE)].name;
						}
					}
				}

				std::string getAtributeString(Pi::File::Header::const_pointer_t header, bool alternative)
				{
					char strBuffer[256];
					std::memset(strBuffer, 0, sizeof(strBuffer));
					if (alternative)
					{
						std::snprintf(strBuffer, sizeof(strBuffer),
							"LARGE_FILE : %c | ALIGNMENT2 : %c | FIXED : %c | DATA ALIGNMENT : %s | CHECKSUM : %c | SET_TO_ZERO : %c ( %#.2X )",
							header->Attributes & FFS_ATTRIB_LARGE_FILE      ? 'T' : 'F',
							header->Attributes & FFS_ATTRIB_DATA_ALIGNMENT2 ? 'T' : 'F',
							header->Attributes & FFS_ATTRIB_FIXED           ? 'T' : 'F',
							Helper::getFileDataAlignment(header),
							header->Attributes & FFS_ATTRIB_CHECKSUM        ? 'T' : 'F',
							header->Attributes & FFS_ATTRIB_SET_TO_ZERO     ? 'F' : 'T',
							header->Attributes
						);
					}
					else
					{
						std::snprintf(strBuffer, sizeof(strBuffer),
							"%s%s%s%s%sDATA ALIGNMENT : %s ( %#.2X )",
							header->Attributes & FFS_ATTRIB_LARGE_FILE      ? "LARGE_FILE | "      : "",
							header->Attributes & FFS_ATTRIB_DATA_ALIGNMENT2 ? "DATA_ALIGNMENT2 | " : "",
							header->Attributes & FFS_ATTRIB_FIXED           ? "FIXED | "           : "",
							header->Attributes & FFS_ATTRIB_CHECKSUM        ? "CHECKSUM | "        : "",
							header->Attributes & FFS_ATTRIB_SET_TO_ZERO     ? "SET_TO_ONE | "      : "",
							Helper::getFileDataAlignment(header),
							header->Attributes
						);
					}
					return std::string(strBuffer);
				}

				std::string getStateString(Pi::File::Header::const_pointer_t header, bool alternative)
				{
					char strBuffer[256];
					std::memset(strBuffer, 0, sizeof(strBuffer));
					if (alternative)
					{
						std::snprintf(strBuffer, sizeof(strBuffer),
							"HEADER_CONSTRUCTION : %c | HEADER_VALID : %c | DATA_VALID : %c | MARKED_FOR_UPDATE : %c | DELETED : %c | HEADER_INVALID : %c | SET_TO_ERASE_POLARITY : %c ( %#.2X )",
							header->State & EFI_FILE_HEADER_CONSTRUCTION ? 'T' : 'F',
							header->State & EFI_FILE_HEADER_VALID        ? 'T' : 'F',
							header->State & EFI_FILE_DATA_VALID          ? 'T' : 'F',
							header->State & EFI_FILE_MARKED_FOR_UPDATE   ? 'T' : 'F',
							header->State & EFI_FILE_DELETED             ? 'T' : 'F',
							header->State & EFI_FILE_HEADER_INVALID      ? 'T' : 'F',
							header->State & EFI_FILE_STATE_MASK          ? '1' : '0',
							header->State
						);
					}
					else
					{
						std::snprintf(strBuffer, sizeof(strBuffer),
							"%s%s%s%s%s%sSET_TO_ERASE_POLARITY : %c ( %#.2X )",
							header->State & EFI_FILE_HEADER_CONSTRUCTION ? "HEADER_CONSTRUCTION | " : "",
							header->State & EFI_FILE_HEADER_VALID        ? "HEADER_VALID | "        : "",
							header->State & EFI_FILE_DATA_VALID          ? "DATA_VALID | "          : "",
							header->State & EFI_FILE_MARKED_FOR_UPDATE   ? "MARKED_FOR_UPDATE | "   : "",
							header->State & EFI_FILE_DELETED             ? "DELETED | "             : "",
							header->State & EFI_FILE_HEADER_INVALID      ? "HEADER_INVALID | "      : "",
							header->State & EFI_FILE_STATE_MASK          ? '1'                      : '0',
							header->State
						);
					}
					return std::string(strBuffer);
				}

				std::string findFileName(const PiObject::object_vec_t& vec)
				{
					const PiObject::Section* section = nullptr;
					for (const auto& obj : vec)
					{
						if (obj->state & PiObject::InconsistencyState::TypeBits == PiObject::InconsistencyState::SectionFlag)
						{
							section = dynamic_cast<const PiObject::Section*>(obj.get());
							if (section)
							{
								switch (section->header.sectionType)
								{
									case PiObject::Helper::SectionHeader::Compression:
									case PiObject::Helper::SectionHeader::Disposable:
									case PiObject::Helper::SectionHeader::GuidDefined:
									{
										auto result = findFileName(section->objects);
										if (!result.empty()) {
											return result;
										}
									} break;
									case PiObject::Helper::SectionHeader::UserInterface:
									{
										auto* ptr_ = reinterpret_cast<const char16_t*>(reinterpret_cast<Pi::Section::Version::const_pointer_t>(section->header.header.begin)->VersionString);
										std::u16string u16str(ptr_, (Pi::Section::Utils::getSize(section->header.header) - Pi::Section::Header::structure_size - sizeof(UINT16)) / sizeof(CHAR16));
										return utf16_to_utf8(u16str);
									} break;
									default: break;
								}
							}
						}
					}
					return std::string();
				}

			}
		}
	}
}

#undef NOT_EFI_FV_FILETYPE

#undef PROJ_FILE_TYPE_TO_STR2
#undef PROJ_FILE_TYPE_TO_STR

#undef GET_END_PTR