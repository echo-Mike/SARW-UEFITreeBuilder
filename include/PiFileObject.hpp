#pragma once
#ifndef PI_FILE_OBJECT_HPP__
#define	PI_FILE_OBJECT_HPP__ "0.0.0@PiFileObject.hpp"

/// STD

/// PROJECT
#include "General.hpp"
#include "PiSectionObject.hpp"

namespace Project
{
	namespace PiObject
	{

		namespace helper
		{
			struct FileHeader
			{
				enum { Simple, Extended } headerType;
				union
				{
					Pi::File::Header header;
					Pi::File::Extended::Header extended;
				};

				FileHeader(const Pi::File::Header& h) : headerType(Simple), header(h) {}
				FileHeader(const Pi::File::Extended::Header& h) : headerType(Extended), extended(h) {}
				
				DefaultCopyableAndMovable(FileHeader)

				~FileHeader() = default;

				bool isExtended() const { return headerType == Extended; }

				Pi::File::Header::const_pointer_t asSimpleHeader() const
				{
					return isExtended() ? reinterpret_cast<Pi::File::Header::const_pointer_t>(extended.begin) : header.get();
				}

			};

			void to_json(nlohmann::json& j, const FileHeader& obj);
		}

		struct File :
			public BaseObject
		{
			typedef BaseObject Base;

			File(const Pi::File::Header& headerView, const MemoryView& baseBuffer, const MemoryView& myBuffer) :
				Base(baseBuffer, myBuffer), header(headerView)
			{
				setUid(headerView);
			}

			File(const Pi::File::Extended::Header& headerView, const MemoryView& baseBuffer, const MemoryView& myBuffer) :
				Base(baseBuffer, myBuffer), header(headerView)
			{
				setUid(headerView);
			}

			DefaultCopyableAndMovable(File)

			~File() = default;

			void toJson(nlohmann::json& j) const;

			helper::FileHeader header;
			SectionObjectsVec_t sections;
		};

	}
}

#endif