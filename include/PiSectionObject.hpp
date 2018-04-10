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
				enum { Simple, Extended } headerType;
				
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

				union
				{
					std::unique_ptr<Pi::Section::SectionTag> simple;
					std::unique_ptr<Pi::Section::SectionExtTag> extended;
				};

				SectionHeader(SectionType type, Pi::Section::SectionTag* ptr) : headerType(Simple), sectionType(type), simple(ptr) {}
				SectionHeader(SectionType type, Pi::Section::SectionExtTag* ptr) : headerType(Extended), sectionType(type), extended(ptr) {}

				DefaultMovableNotCopyable(SectionHeader)

				~SectionHeader() 
				{
					if (headerType == Simple)
						delete simple.release();
					else
						delete extended.release();
				};

				MemoryView* get() const
				{
					if (headerType == Simple)
						return dynamic_cast<MemoryView*>(simple.get());
					else
						return dynamic_cast<MemoryView*>(extended.get());
				}
			};
		}

		struct Section :
			public BaseObject
		{
			typedef BaseObject Base;

			Section(helper::SectionHeader::SectionType type, Pi::Section::SectionTag* ptr,
				const MemoryView& baseBuffer, const MemoryView& myBuffer) :
				Base(baseBuffer, myBuffer), header(type, ptr)
			{
				setUid(myBuffer);
			}

			Section(helper::SectionHeader::SectionType type, Pi::Section::SectionExtTag* ptr,
				const MemoryView& baseBuffer, const MemoryView& myBuffer) :
				Base(baseBuffer, myBuffer), header(type, ptr)
			{
				setUid(myBuffer);
			}

			Section(const Section& other);

			Section& operator=(const Section& other);

			DefaultMovable(Section)

			~Section() = default;

			void toJson(nlohmann::json& j) const;

			helper::SectionHeader header;
			SectionObjectsVec_t sections;
		};

	}
}

#endif