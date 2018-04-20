#pragma once
#ifndef PI_FILE_OBJECT_HPP__
#define	PI_FILE_OBJECT_HPP__ "0.0.0@PiFileObject.hpp"

/// STD

/// PROJECT
#include "PiBaseObject.hpp"

namespace Project
{

	namespace PiObject
	{

		namespace helper
		{

			struct FileHeader
			{

				enum HeaderType { Simple, Extended } headerType;

				Pi::File::Header header;

				FileHeader(HeaderType htype, const Pi::File::Header& h) : headerType(htype), header(h) {}
				
				DefaultCopyableAndMovable(FileHeader)

				~FileHeader() = default;

				bool isExtended() const { return headerType == Extended; }

			};

			void to_json(nlohmann::json& j, const FileHeader& obj);

		}

		struct File :
			public ComplexObject
		{
			typedef ComplexObject Base;

			File(helper::FileHeader::HeaderType htype, const Pi::File::Header& headerView, 
				 const MemoryView& baseBuffer, const MemoryView& myBuffer) :
				Base(baseBuffer, myBuffer, InconsistencyState::FileFlag), 
				header(htype, headerView)
			{
				setUid(headerView);
			}

			File(helper::FileHeader::HeaderType htype, const Pi::File::Header& headerView, 
				 const MemoryView& baseBuffer, MemoryView&& myBuffer) :
				Base(baseBuffer, std::move(myBuffer), InconsistencyState::FileFlag), 
				header(htype, headerView)
			{
				setUid(headerView);
			}

			DefaultCopyableAndMovable(File)

			~File() = default;

			// Virtual i-face implementation

			void toJson(nlohmann::json& j) const;

			PROJ_CopyablePiObject(File)
			
			// Class i-face

			inline Pi::File::Header* operator->() { return &header.header; }
			inline const Pi::File::Header* operator->() const { return &header.header; }

			helper::FileHeader header;
		};

	}

}

#endif