#pragma once
#ifndef PI_SECTION_UTILS_HPP__
#define PI_SECTION_UTILS_HPP__ "0.0.0@PiSectionUtils.hpp"

/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"
#include "PiSectionObject.hpp"

namespace Project
{

	namespace Pi
	{

		namespace Section
		{

			namespace Utils
			{

				inline Types::length_t getSize(const Pi::Section::Header& header)
				{
					return  (static_cast<Types::length_t>(header->Size[2]) << 16) +
							(static_cast<Types::length_t>(header->Size[1]) << 8) +
							 static_cast<Types::length_t>(header->Size[0]);
				}

				inline Types::length_t getSize(const Pi::Section::Extended::Header& header)
				{
					return header->ExtendedSize;
				}

				inline Types::length_t getSize(const EFI_COMMON_SECTION_HEADER* header)
				{
					return  (static_cast<Types::length_t>(header->Size[2]) << 16) +
							(static_cast<Types::length_t>(header->Size[1]) << 8) +
							 static_cast<Types::length_t>(header->Size[0]);
				}

				inline Types::length_t getSize(const EFI_COMMON_SECTION_HEADER2* header)
				{
					return header->ExtendedSize;
				}

				inline Types::length_t getSize(const PiObject::Section& sec)
				{
					return sec.header.isExtended() ? getSize(sec.header.get()) : getSize(file.header.header);
				}


				inline Types::length_t getSize1(Types::const_pointer_t header)
				{
					return getSize(reinterpret_cast<const EFI_COMMON_SECTION_HEADER*>(header));
				}

				inline Types::length_t getSize2(Types::const_pointer_t header)
				{
					return reinterpret_cast<const EFI_COMMON_SECTION_HEADER2*>(header)->ExtendedSize;
				}

				inline Types::length_t getSizeAuto(const EFI_COMMON_SECTION_HEADER* header)
				{
					return getSize(header) == 0xFFFFFF ? getSize2(UnifyPtrCast(header)) : getSize(header);
				}
			}

		}

	}

}
#endif