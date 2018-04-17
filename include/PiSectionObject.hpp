#pragma once
#ifndef PI_SECTION_OBJECT_HPP__
#define	PI_SECTION_OBJECT_HPP__ "0.0.0@PiSectionObject.hpp"

/// STD
#include <memory>

/// PROJECT
#include "General.hpp"
#include "PiBaseObject.hpp"

namespace Project
{
	namespace PiObject
	{
		struct Section;

		typedef std::vector< Section > SectionObjectsVec_t;

		namespace helper
		{

			struct SectionHeader
			{
				enum HeaderType { Simple, Extended } headerType;
				
				enum SectionType {
					Compatibility16,
					Compression,
					Disposable,
					DxeDepex,
					FirmwareVolumeImage,
					FreeformSubtypeGuid,
					GuidDefined,
					Pe32,
					PeiDepex,
					Pic,
					Te,
					Raw,
					SmmDepex,
					UserInterface,
					Version
				} sectionType;

				Pi::Section::Header header;

				SectionHeader(HeaderType htype, Types::const_pointer_t ptr, SectionType stype = Raw) : headerType(htype), sectionType(stype), header(ptr) {}

				DefaultCopyableAndMovable(SectionHeader)

				~SectionHeader() = default;

				bool isExtended() const { return headerType == Extended; }

				void toJson(nlohmann::json& j) const;
			};
		}

		struct Section :
			public BaseObject
		{
			typedef BaseObject Base;

			Section( helper::SectionHeader::HeaderType htype, 
				Types::const_pointer_t ptr,
				const MemoryView& baseBuffer, const MemoryView& myBuffer) :
				Base(baseBuffer, myBuffer), header(htype, ptr)
			{
				setUid(myBuffer);
			}

			DefaultCopyableAndMovable(Section)

			~Section() = default;

			void toJson(nlohmann::json& j) const;

			helper::SectionHeader header;
			SectionObjectsVec_t sections;
		};

	}
}

#endif