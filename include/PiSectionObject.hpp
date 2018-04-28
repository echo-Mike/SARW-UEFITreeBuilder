#pragma once
#ifndef PI_SECTION_OBJECT_HPP__
#define	PI_SECTION_OBJECT_HPP__ "0.0.0@PiSectionObject.hpp"

/// STD
#include <map>

/// PROJECT
#include "PiComplexObject.hpp"
#include "DecompressionModule.hpp"

namespace Project
{

	namespace PiObject
	{

		namespace Helper
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

			struct SectionDecompressedData
			{
				SectionDecompressedData(
						Decompression::Decompresser::Decompresser_t decomp,
						Types::length_t buffSize, 
						Types::unique_byte_buff_t&& buff) :
					memory(reinterpret_cast<Types::pointer_t>(buff.get())), 
					buffer(std::move(buff)),
					decompresser(decomp)
				{
					memory.setLength(buffSize);
				}

				MemoryView memory;
				Types::unique_byte_buff_t buffer;
				Decompression::Decompresser::Decompresser_t decompresser;
			};

			typedef std::unique_ptr< SectionDecompressedData > unique_section_decomp_buff_t;

			typedef std::map<Types::hash_t, unique_section_decomp_buff_t> section_decomp_data_storage_t;

		}

		struct Section :
			public ComplexObject
		{
			typedef ComplexObject Base;

			typedef Helper::SectionDecompressedData decomp_data_t;

			typedef Helper::unique_section_decomp_buff_t unique_section_decomp_buff_t;

			typedef Helper::section_decomp_data_storage_t  decomp_data_storage_t;

			typedef Helper::section_decomp_data_storage_t* decomp_data_storage_ptr_t;

			typedef Pi::Section::Header::value_type RepresentedStruct_t;

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
				decomp_data_storage_ptr_t result = nullptr;
				if (header.sectionType == Helper::SectionHeader::Compression ||
					header.sectionType == Helper::SectionHeader::GuidDefined) {
					result = &DecompressedSectionData;
				} else DEBUG_WARNING_MESSAGE
					DEBUG_PRINT("\tMessage: Can't access decompressed data storage.");
					DEBUG_PRINT("\tSection UID: ", getUid());
					DEBUG_PRINT("\tSection type: ", header.sectionType);
				DEBUG_END_MESSAGE;
				return result;
			}

			inline Pi::Section::Header::const_pointer_t operator->() const { return header.header.get(); }

			Helper::SectionHeader header;
		};

	}

}

#endif