#pragma once
#ifndef PI_VOLUME_OBJECT_HPP__
#define	PI_VOLUME_OBJECT_HPP__ "0.0.0@PiVolumeObject.hpp"

/// PROJECT
#include "PiBaseObject.hpp"

#define PROJ_DEFAULT_EMPTY (0xFF)

namespace Project
{

	namespace PiObject
	{

		struct Volume :
			public ComplexObject
		{
			typedef ComplexObject Base;

			typedef Pi::Volume::Header::value_type RepresentedStruct_t;

			Volume( const Pi::Volume::Header& hdr, 
					const MemoryView& baseBuffer, const MemoryView& myBuffer) :
				Base(baseBuffer, myBuffer, InconsistencyState::VolumeFlag), 
				normalHdr(hdr)
			{
				fullNormalHdr.begin = normalHdr.begin;
				setUid(hdr);
			}

			Volume( const Pi::Volume::Header& hdr, 
					const MemoryView& baseBuffer, MemoryView&& myBuffer) :
				Base(baseBuffer, std::move(myBuffer), InconsistencyState::VolumeFlag), 
				normalHdr(hdr)
			{
				fullNormalHdr.begin = normalHdr.begin;
				setUid(hdr);
			}

			DefaultCopyableAndMovable(Volume)

			~Volume() = default;

			// Virtual i-face implementation

			void toJson(nlohmann::json& j) const;

			PROJ_CopyablePiObject(Volume)
			
			// Class i-face

			inline const RepresentedStruct_t* operator->() const { return normalHdr.get(); }

			Pi::Volume::Header normalHdr;
			Pi::Volume::Extension::Header extendedHdr;
			MemoryView fullNormalHdr;
			MemoryView fullExtendedHdr;
		};

	}

}

#endif