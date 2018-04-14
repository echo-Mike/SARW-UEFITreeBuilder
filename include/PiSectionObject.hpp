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

				std::unique_ptr< ::Project::helper::AbstractInheritance > view;

				SectionHeader(HeaderType htype, SectionType stype) : headerType(htype), sectionType(stype), view(nullptr) {}

				SectionHeader(SectionType type, ::Project::helper::InheritanceFilter<Pi::Section::SectionTag>* ptr) : headerType(Simple), sectionType(type), view(ptr) {}
				
				SectionHeader(SectionType type, ::Project::helper::InheritanceFilter<Pi::Section::SectionExtTag>* ptr) : headerType(Extended), sectionType(type), view(ptr) {}

				SectionHeader(const SectionHeader& other) :
					headerType(other.headerType),
					sectionType(other.sectionType),
					view(nullptr)
				{
					view.reset(other.get()->copy());
				}

				SectionHeader& operator=(const SectionHeader& other)
				{
					if (this == &other) {
						return *this;
					}
					headerType = other.headerType;
					sectionType = other.sectionType;
					view.reset(other.get()->copy());
					return *this;
				}

				DefaultMovable(SectionHeader)

				~SectionHeader() = default;

				::Project::helper::AbstractInheritance* get() const { return view.get(); }
			};
		}

		struct Section :
			public BaseObject
		{
			typedef BaseObject Base;

			Section( helper::SectionHeader::SectionType type, 
				::Project::helper::InheritanceFilter<Pi::Section::SectionTag>* ptr,
				const MemoryView& baseBuffer, const MemoryView& myBuffer) :
				Base(baseBuffer, myBuffer), header(type, ptr)
			{
				setUid(myBuffer);
			}

			Section( helper::SectionHeader::SectionType type, 
				::Project::helper::InheritanceFilter<Pi::Section::SectionExtTag>* ptr,
				const MemoryView& baseBuffer, const MemoryView& myBuffer) :
				Base(baseBuffer, myBuffer), header(type, ptr)
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