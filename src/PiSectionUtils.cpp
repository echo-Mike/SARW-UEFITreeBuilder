/// STD
#include <algorithm>
#include <cstdio>

/// PROJECT
#include "PiSectionUtils.hpp"

#define GET_END_PTR(Array, Type) ( Array + ( sizeof(Array) / sizeof(Type) ) )

#define PROJ_SEC_TYPE_TO_STR(MACRO_) { MACRO_, #MACRO_ }

#define NOT_EFI_SECTION_TYPE (EFI_SECTION_COMPRESSION - 1)

namespace Project
{
	namespace Pi
	{
		namespace Section
		{
			namespace Utils
			{
				namespace Helper
				{
					static const Types::byte_t SectionTypes[] =
					{
						EFI_SECTION_COMPRESSION,
						EFI_SECTION_GUID_DEFINED,
						EFI_SECTION_DISPOSABLE,
						EFI_SECTION_PE32,
						EFI_SECTION_PIC,
						EFI_SECTION_TE,
						EFI_SECTION_DXE_DEPEX,
						EFI_SECTION_VERSION,
						EFI_SECTION_USER_INTERFACE,
						EFI_SECTION_COMPATIBILITY16,
						EFI_SECTION_FIRMWARE_VOLUME_IMAGE,
						EFI_SECTION_FREEFORM_SUBTYPE_GUID,
						EFI_SECTION_RAW,
						EFI_SECTION_PEI_DEPEX,
						EFI_SECTION_SMM_DEPEX,
						SCT_SECTION_POSTCODE,
						INSYDE_SECTION_POSTCODE
					};

					static const Types::byte_t EncSectionTypes[] =
					{
						EFI_SECTION_COMPRESSION,
						EFI_SECTION_GUID_DEFINED,
						EFI_SECTION_DISPOSABLE
					};

					struct SectionMacroPlusName
					{
						Types::byte_t type;
						const char* name;
					};

					static const SectionMacroPlusName SecNames[] =
					{
						PROJ_SEC_TYPE_TO_STR(EFI_SECTION_COMPRESSION),
						PROJ_SEC_TYPE_TO_STR(EFI_SECTION_GUID_DEFINED),
						PROJ_SEC_TYPE_TO_STR(EFI_SECTION_DISPOSABLE),
						PROJ_SEC_TYPE_TO_STR(EFI_SECTION_PE32),
						PROJ_SEC_TYPE_TO_STR(EFI_SECTION_PIC),
						PROJ_SEC_TYPE_TO_STR(EFI_SECTION_TE),
						PROJ_SEC_TYPE_TO_STR(EFI_SECTION_DXE_DEPEX),
						PROJ_SEC_TYPE_TO_STR(EFI_SECTION_VERSION),
						PROJ_SEC_TYPE_TO_STR(EFI_SECTION_USER_INTERFACE),
						PROJ_SEC_TYPE_TO_STR(EFI_SECTION_COMPATIBILITY16),
						PROJ_SEC_TYPE_TO_STR(EFI_SECTION_FIRMWARE_VOLUME_IMAGE),
						PROJ_SEC_TYPE_TO_STR(EFI_SECTION_FREEFORM_SUBTYPE_GUID),
						PROJ_SEC_TYPE_TO_STR(EFI_SECTION_RAW),
						PROJ_SEC_TYPE_TO_STR(EFI_SECTION_PEI_DEPEX),
						PROJ_SEC_TYPE_TO_STR(EFI_SECTION_SMM_DEPEX),
						PROJ_SEC_TYPE_TO_STR(SCT_SECTION_POSTCODE),
						PROJ_SEC_TYPE_TO_STR(INSYDE_SECTION_POSTCODE),
						PROJ_SEC_TYPE_TO_STR(NOT_EFI_SECTION_TYPE)
					};

					std::size_t secNamePosition(Types::byte_t type)
					{
						switch (type)
						{
							case EFI_SECTION_COMPRESSION :
								return 0;
							case EFI_SECTION_GUID_DEFINED :
								return 1;
							case EFI_SECTION_DISPOSABLE :
								return 2;
							case EFI_SECTION_PE32 :
								return 3;
							case EFI_SECTION_PIC :
								return 4;
							case EFI_SECTION_TE :
								return 5;
							case EFI_SECTION_DXE_DEPEX :
								return 6;
							case EFI_SECTION_VERSION :
								return 7;
							case EFI_SECTION_USER_INTERFACE :
								return 8;
							case EFI_SECTION_COMPATIBILITY16 :
								return 9;
							case EFI_SECTION_FIRMWARE_VOLUME_IMAGE :
								return 10;
							case EFI_SECTION_FREEFORM_SUBTYPE_GUID :
								return 11;
							case EFI_SECTION_RAW :
								return 12;
							case EFI_SECTION_PEI_DEPEX :
								return 13;
							case EFI_SECTION_SMM_DEPEX :
								return 14;
							case SCT_SECTION_POSTCODE :
								return 15;
							case INSYDE_SECTION_POSTCODE :
								return 16;
							case NOT_EFI_SECTION_TYPE:
								return sizeof(SecNames) / sizeof(SectionMacroPlusName) - 1;
							default:
								return sizeof(SecNames) / sizeof(SectionMacroPlusName);
								break;
						}
					}
				}

				Types::length_t getHeaderSize(Pi::Section::Header::const_pointer_t header)
				{
					using namespace Pi::Section;
					bool extended = getSize(header) == PROJ_SECTION_MAX_SIZE;
					switch (header->Type)
					{
						case EFI_SECTION_COMPRESSION: return extended ? Extended::Compression::structure_size : Compression::structure_size;
						case EFI_SECTION_GUID_DEFINED: return extended ? Extended::GuidDefined::structure_size : GuidDefined::structure_size;
						case EFI_SECTION_DISPOSABLE: return extended ? Extended::Disposable::structure_size : Disposable::structure_size;
						case EFI_SECTION_PE32: return extended ? Extended::Pe32::structure_size : Pe32::structure_size;
						case EFI_SECTION_PIC: return extended ? Extended::Pic::structure_size : Pic::structure_size;
						case EFI_SECTION_TE: return extended ? Extended::Te::structure_size : Te::structure_size;
						case EFI_SECTION_DXE_DEPEX: return extended ? Extended::DxeDepex::structure_size : DxeDepex::structure_size;
						case EFI_SECTION_VERSION: return extended ? Extended::Version::structure_size : Version::structure_size;
						case EFI_SECTION_USER_INTERFACE: return extended ? Extended::UserInterface::structure_size : UserInterface::structure_size;
						case EFI_SECTION_COMPATIBILITY16: return extended ? Extended::Compatibility16::structure_size : Compatibility16::structure_size;
						case EFI_SECTION_FIRMWARE_VOLUME_IMAGE: return extended ? Extended::FirmwareVolumeImage::structure_size : FirmwareVolumeImage::structure_size;
						case EFI_SECTION_FREEFORM_SUBTYPE_GUID: return extended ? Extended::FreeformSubtypeGuid::structure_size : FreeformSubtypeGuid::structure_size;
						case EFI_SECTION_RAW: return extended ? Extended::Raw::structure_size : Raw::structure_size;
						case EFI_SECTION_PEI_DEPEX: return extended ? Extended::PeiDepex::structure_size : PeiDepex::structure_size;
						case EFI_SECTION_SMM_DEPEX: return extended ? Extended::SmmDepex::structure_size : SmmDepex::structure_size;
						case SCT_SECTION_POSTCODE: return extended ? Extended::Postcode::structure_size : Postcode::structure_size;
						case INSYDE_SECTION_POSTCODE : return extended ? Extended::Postcode::structure_size : Postcode::structure_size;
						default: return extended ? Extended::Header::structure_size : Header::structure_size;
					}
				}

				bool isSectionType(Types::byte_t type)
				{
					using namespace Helper;
					auto end = GET_END_PTR(SectionTypes, Types::byte_t);
					return std::find(SectionTypes, end, type) != end;
				}

				bool isEncapsulationType(Types::byte_t type)
				{
					using namespace Helper;
					auto end = GET_END_PTR(EncSectionTypes, Types::byte_t);
					return std::find(EncSectionTypes, end, type) != end;
				}

				const char* sectionTypeToCStr(Types::byte_t type)
				{
					using namespace Helper;
					auto end = GET_END_PTR(SecNames, SectionMacroPlusName);
					auto pos = secNamePosition(type);
					if (SecNames + pos != end) {
						if (SecNames[pos].type != type)
							DEBUG_WARNING_MESSAGE
								DEBUG_PRINT("\tMessage: Found type do not correspond to section type.");
								DEBUG_PRINT("\tSection type: ", type);
								DEBUG_PRINT("\tType name found: ", SecNames[pos].name);
							DEBUG_END_MESSAGE

						return SecNames[pos].name;
					} else {
						return SecNames[secNamePosition(NOT_EFI_SECTION_TYPE)].name;
					}
				}

				bool checkRsa2048Sha256GuidedSection(const Pi::Section::Header& sectionView, const MemoryView& buffer)
				{
					/*
					auto dataSize = Pi::Section::Utils::getSizeAuto(sectionView);
					auto headerSize = Pi::Section::Utils::isExtendedSection(sectionView) ? Pi::Section::Extended::GuidedSha256::structure_size : Pi::Section::GuidedSha256::structure_size;
					auto tmp = buffer.memcpy();
					MemoryView dataView(tmp.get() + (sectionView.begin - buffer.begin)  + headerSize);
					dataView.setLength(dataSize - headerSize);
					*/
					const EFI_CERT_BLOCK_RSA_2048_SHA256* certBlock = nullptr;
					if (Pi::Section::Utils::isExtendedSection(sectionView)) {
						certBlock = &reinterpret_cast<const RSA_2048_SHA_256_SECTION2_HEADER*>(sectionView.begin)->CertBlockRsa2048Sha256;
					} else {
						certBlock = &reinterpret_cast<const RSA_2048_SHA_256_SECTION_HEADER*>(sectionView.begin)->CertBlockRsa2048Sha256;
					}
					auto keyBytes = reinterpret_cast<Types::const_byte_ptr_t>(certBlock->PublicKey);
					DEBUG_INFO_MESSAGE
						DEBUG_PRINT("\tMessage: checkRsa2048Sha256GuidedSection is not implemented yet.");
						DEBUG_PRINT("\tPublic key:");
						char strBuffer[64];
						for (std::size_t i = 0; i < 16; ++i)
						{
							std::memset(strBuffer, 0, sizeof(strBuffer));
							std::snprintf(strBuffer, sizeof(strBuffer),
								"\t\t%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX:%.2hhX",
								keyBytes[16 * i +  0], keyBytes[16 * i +  1], keyBytes[16 * i +  2], keyBytes[16 * i +  3],
								keyBytes[16 * i +  4], keyBytes[16 * i +  5], keyBytes[16 * i +  6], keyBytes[16 * i +  7],
								keyBytes[16 * i +  8], keyBytes[16 * i +  9], keyBytes[16 * i + 10], keyBytes[16 * i + 11],
								keyBytes[16 * i + 12], keyBytes[16 * i + 13], keyBytes[16 * i + 14], keyBytes[16 * i + 15]
							);
							DEBUG_PRINT(strBuffer);
						}
					DEBUG_END_MESSAGE
					return false;
				}
			}
		}
	}
}

#undef NOT_EFI_SECTION_TYPE

#undef PROJ_SEC_TYPE_TO_STR

#undef GET_END_PTR