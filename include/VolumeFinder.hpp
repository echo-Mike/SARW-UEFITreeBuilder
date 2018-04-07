#pragma once
#ifndef VOLUME_FINDER_HPP__
#define	VOLUME_FINDER_HPP__ "0.0.0@VolumeFinder.hpp"

/// STD
#include <vector>

/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"

namespace Project
{
	namespace Finders
	{

		typedef std::vector< Pi::Volume::Header > VolumesVec_t;

		struct VolumeFinder
		{
			static const signature_cast_32<EFI_FVH_SIGNATURE> VolumeSignature;

			VolumesVec_t operator()(const MemoryView& buffer);

		};

	}
}

#endif