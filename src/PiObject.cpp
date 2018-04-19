/// STD
#include <algorithm>
/// PROJECT
#include "PiObjectRepresentation.hpp"
#include "PiSectionUtils.hpp"

namespace Project
{
	namespace PiObject
	{

		bool FreeSpace::checkCorruption(Types::memory_t empty)
		{
			corruptFlag = (end != std::find_if_not(
				begin, 
				end, 
				[empty](const Types::memory_t v) -> bool
				{ 
					return v == empty; 
				}
			));
			return corruptFlag;
		}

		//void to_json(nlohmann::json& j, const FreeSpace& obj)
		//{

		//}

		void BaseObject::toJson(nlohmann::json& j) const
		{
			j["uid"] = uid;
			j["position"] = OffsetView(basePtr, memory);
			if (!freeSpace.empty())
			{
				std::vector< OffsetView > freeOffsets;
				freeOffsets.reserve(freeSpace.size());
				for (const auto& v : freeSpace)
					freeOffsets.emplace_back(memory.begin, v);
				j["freeSpace"] = freeOffsets;
			}
		}

		void BaseObject::setUid(const MemoryView& buffer)
		{
			uid = Checksums::hash(buffer, buffer.getLength());
		}

		namespace helper
		{

			void to_json(nlohmann::json& j, const SectionHeader& obj)
			{
				switch (obj.header->Type) {
				// Encapsulation sections
				case EFI_SECTION_COMPRESSION :
				{

				} break;

				case EFI_SECTION_GUID_DEFINED :
				{

				} break;

				case EFI_SECTION_DISPOSABLE :
				{

				} break;

				// Leaf sections
				case EFI_SECTION_SMM_DEPEX :
				case EFI_SECTION_PEI_DEPEX :
				case EFI_SECTION_DXE_DEPEX :
				{

				} break;

				case EFI_SECTION_PE32 :
				case EFI_SECTION_PIC :
				{

				} break;

				case SCT_SECTION_POSTCODE :
				case INSYDE_SECTION_POSTCODE :
				{

				} break;

				case EFI_SECTION_TE :
				{

				} break;

				case EFI_SECTION_VERSION :
				{

				} break;

				case EFI_SECTION_USER_INTERFACE :
				{

				} break;

				case EFI_SECTION_COMPATIBILITY16 :
				{

				} break;

				case EFI_SECTION_FIRMWARE_VOLUME_IMAGE :
				{

				} break;

				case EFI_SECTION_FREEFORM_SUBTYPE_GUID :
				{

				} break;

				case EFI_SECTION_RAW :
				{

				} break;

				default :
				{

				} break;

				}
			}
		}

	}
}