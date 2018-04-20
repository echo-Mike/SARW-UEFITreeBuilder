#pragma once
#ifndef PI_SECTION_OBJECT_HPP__
#define	PI_SECTION_OBJECT_HPP__ "0.0.0@PiSectionObject.hpp"

/// STD
#include <map>
/// PROJECT
#include "PiBaseObject.hpp"
#include "DecompressionModule.hpp"

namespace Project
{
	namespace PiObject
	{

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

			typedef std::unique_ptr< SectionDecompressedData > unique_section_decomp_buff_t;

			void to_json(nlohmann::json& j, const unique_section_decomp_buff_t& obj);

			typedef std::map<Types::hash_t, unique_section_decomp_buff_t> section_decomp_data_storage_t;

		}

		struct Section :
			public ComplexObject
		{
			typedef ComplexObject Base;

			typedef helper::section_decomp_data_storage_t  decomp_data_storage_t;

			typedef helper::section_decomp_data_storage_t* decomp_data_storage_ptr_t;

		private:

			static decomp_data_storage_t DecompressedSectionData;

			inline void initialize()
			{
				if (memory.getLength() >= PROJ_4KB) {
					setSimpleUid(*this);
				} else {
					setUid(memory);
				}
			}

		public:

			Section(const Pi::Section::Header& hdr,
					const MemoryView& baseBuffer, 
					const MemoryView& myBuffer) :
				Base(baseBuffer, myBuffer, InconsistencyState::SectionFlag), 
				header(hdr)
			{
				initialize();
			}

			Section(const Pi::Section::Header& hdr,
					const MemoryView& baseBuffer, 
					MemoryView&& myBuffer) :
				Base(baseBuffer, std::move(myBuffer), InconsistencyState::SectionFlag),
				header(hdr)
			{
				initialize();
			}

			DefaultCopyableAndMovable(Section)

			~Section() = default;

			// Virtual i-face implementation

			void toJson(nlohmann::json& j) const;
			
			PROJ_CopyablePiObject(Section)

			// Class i-face

			decomp_data_storage_ptr_t getDecomressedDataStorage() const 
			{
				return  header.sectionType == helper::SectionHeader::Compression ||
                        header.sectionType == helper::SectionHeader::GuidDefined ?
						&DecompressedSectionData :
						nullptr;
			}

			inline Pi::Section::Header* operator->() { return &header.header; }
			inline const Pi::Section::Header* operator->() const { return &header.header; }

			helper::SectionHeader header;
		};

	}
}

#endif