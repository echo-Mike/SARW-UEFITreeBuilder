#pragma once
#ifndef PI_VOLUME_UTILS_HPP__
#define PI_VOLUME_UTILS_HPP__ "0.0.0@PiVolumeUtils.hpp"

/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"
#include "PiVolumeObject.hpp"

namespace Project
{

	namespace Pi
	{

		namespace Volume
		{

			namespace Utils
			{

				Types::length_t getSize(Pi::Volume::Header::const_pointer_t header);

				inline Types::length_t getSize(const Pi::Volume::Header& header) { return getSize(header.get()); }

				inline Types::length_t getSize(const PiObject::Volume& volume) { return getSize(volume.normalHdr.get()); }
				
				Types::length_t getHeaderSize(Pi::Volume::Header::const_pointer_t header);

				inline Types::length_t getHeaderSize(const Pi::Volume::Header& header) { return getHeaderSize(header.get()); }

				inline Types::length_t getHeaderSize(const PiObject::Volume& volume) { return getHeaderSize(volume.normalHdr.get()); }

				bool addParsedVolume(const Pi::Volume::Header& header);

				bool isParsedVolume(const Pi::Volume::Header& header);

			}

		}

	}

}

#endif