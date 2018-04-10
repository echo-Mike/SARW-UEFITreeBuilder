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

			void toJson(nlohmann::json& j) const;

			FileObjectsVec_t files;
		};

	}
}

#endif