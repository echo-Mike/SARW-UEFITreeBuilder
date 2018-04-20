#pragma once
#ifndef PI_VOLUME_OBJECT_HPP__
#define	PI_VOLUME_OBJECT_HPP__ "0.0.0@PiVolumeObject.hpp"

/// STD

/// PROJECT
#include "PiBaseObject.hpp"

namespace Project
{

	namespace PiObject
	{

		struct Volume :
			public ComplexObject
		{
			typedef ComplexObject Base;

			Volume( const Pi::Volume::Header& hdr, 
					const MemoryView& baseBuffer, const MemoryView& myBuffer) :
				Base(baseBuffer, myBuffer, InconsistencyState::VolumeFlag), 
				header(hdr)
			{
				fullHeader.begin = header.begin;
				setUid(hdr);
			}

			Volume( const Pi::Volume::Header& hdr, 
					const MemoryView& baseBuffer, MemoryView&& myBuffer) :
				Base(baseBuffer, std::move(myBuffer), InconsistencyState::VolumeFlag), 
				header(hdr)
			{
				fullHeader.begin = header.begin;
				setUid(hdr);
			}

			DefaultCopyableAndMovable(Volume)

			~Volume() = default;

			// Virtual i-face implementation

			void toJson(nlohmann::json& j) const;

			PROJ_CopyablePiObject(Volume)
			
			// Class i-face

			Pi::Volume::Header* operator->() { return &normalHdr; }
			const Pi::Volume::Header* operator->() const { return &normalHdr; }

			Pi::Volume::Header normalHdr;
			Pi::Volume::Extension::Header extendedHdr;
			MemoryView normalHdr_memory;
			MemoryView extendedHdr_memory;
		};

	}

}

#endif