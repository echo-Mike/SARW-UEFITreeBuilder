#pragma once
#ifndef PI_VOLUME_OBJECT_HPP__
#define	PI_VOLUME_OBJECT_HPP__ "0.0.0@PiVolumeObject.hpp"

/// STD

/// PROJECT
#include "General.hpp"
#include "PiFileObject.hpp"

namespace Project
{
	namespace PiObject
	{

		typedef std::vector< File > FileObjectsVec_t;

		struct Volume :
			public BaseObject
		{
			typedef BaseObject Base;

			Volume( const Pi::Volume::Header& volHeader, 
					const MemoryView& baseBuffer, const MemoryView& myBuffer) :
					Base(baseBuffer, myBuffer), header(volHeader)
			{
				fullHeader.begin = header.begin;
			}

			void toJson(nlohmann::json& j) const;
			
			Pi::Volume::Header header;
			MemoryView fullHeader;
			struct {
				Pi::Volume::Extension::Header header;
				MemoryView memory;
			} extHeader;
			FileObjectsVec_t files;
		};

	}
}

#endif