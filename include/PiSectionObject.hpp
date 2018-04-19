#pragma once
#ifndef PI_SECTION_OBJECT_HPP__
#define	PI_SECTION_OBJECT_HPP__ "0.0.0@PiSectionObject.hpp"

/// STD
#include <memory>

/// PROJECT
#include "General.hpp"
#include "PiBaseObject.hpp"
#include "DecompressionModule.hpp"

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
					Version,
					PostcodeSct,
					PostcodeInsyde
				} sectionType;

				Pi::Section::Header header;

				SectionHeader(const Pi::Section::Header& hdr, HeaderType htype = Simple, SectionType stype = Raw) : headerType(htype), sectionType(stype), header(hdr) {}

				DefaultCopyableAndMovable(SectionHeader)

				~SectionHeader() = default;

				bool isExtended() const { return headerType == Extended; }

			};

			void to_json(nlohmann::json& j, const SectionHeader& obj);

			struct SectionDecompressedData
			{
				SectionDecompressedData(
						Types::length_t buffSize, 
						Types::unique_byte_buff_t&& buff) :
					memory(reinterpret_cast<Types::pointer_t>(buff.get())), 
					buffer(std::move(buff)) 
				{
					memory.setLength(buffSize);
				}

				MemoryView memory;
				Types::unique_byte_buff_t buffer;
			};

			typedef std::unique_ptr< SectionDecompressedData > unique_decomp_buff_t;

			void to_json(nlohmann::json& j, const unique_decomp_buff_t& obj);

		}

		struct Section :
			public BaseObject
		{
			typedef BaseObject Base;

			Section(const Pi::Section::Header& hdr,
					const MemoryView& baseBuffer, 
					const MemoryView& myBuffer) :
				Base(baseBuffer, myBuffer), 
				decompData(nullptr),
				header(hdr)
			{
				setUid(myBuffer);
			}

			DefaultCopyableAndMovable(Section)

			~Section() = default;

			void toJson(nlohmann::json& j) const;
			
			helper::unique_decomp_buff_t decompData;
			helper::SectionHeader header;
			SectionObjectsVec_t sections;
		};

	}
}

#endif