/// STD
#include <algorithm>
#include <utility>

/// PROJECT
#include "VolumeFinder.hpp"
#include "vFFSGuids.hpp"

namespace Project
{
	namespace Finders
	{
		
		typedef std::pair<
			Types::length_t, // Header offset
			Pi::Volume::Header::const_pointer_t // Pointer to first byte of structure
		> FVHeaderData;

		typedef std::vector< FVHeaderData > HeadersVec_t;

		static const std::size_t VolumeSignatureOffset = offsetof(Pi::Volume::Header::value_type, Signature);

		VolumesVec_t VolumeFinder::operator()(const MemoryView& buffer)
		{
			VolumesVec_t result;
			HeadersVec_t headers;
			Types::const_pointer_t current = buffer.begin;

			// Find all occurrence of Firmware volume signature EFI_FVH_SIGNATURE
			while ( ( current = std::search( current,  buffer.end, VolumeSignature.bytes, VolumeSignature.bytes + sizeof(EFI_FVH_SIGNATURE)) ) != buffer.end )
			{
				headers.emplace_back(
					current - buffer.begin - VolumeSignatureOffset,
					reinterpret_cast<Pi::Volume::Header::const_pointer_t>(current - VolumeSignatureOffset)
				);
				++current;
			}

			// Check found headers
			headers.erase(
				std::remove_if(
					headers.begin(),
					headers.end(),
					[&buffer](const FVHeaderData& value) -> bool
					{
						// Obtain pointer to possible FV header
						auto headerPtr = value.second;
						// 1) Check length clue
						if (headerPtr->HeaderLength < Pi::Volume::Header::structure_size) 
							return true;
						// 2) Check volume length sanity clue
						if (headerPtr->FvLength > buffer.getLength())
							return true;
						// 3) Check checksum clue
						{
							Pi::Volume::Header::value_type header_copy = *headerPtr;
							// Zero current checksum
							header_copy.Checksum = 0;
							// Calculate checksum
							auto checkSum = Project::Checksums::calc16( 
								reinterpret_cast<Types::const_pointer_t>(&header_copy), 
								Pi::Volume::Header::structure_size
							);
							if (headerPtr->Checksum != checkSum) {
								return true;
							}
						}
						// 4) Check GUID match clue
						if ( FfsGuids::isValidFfsGuid(reinterpret_cast<Types::const_pointer_t>(&headerPtr->FileSystemGuid)).code == FfsGuids::KnownFfsGuids::Unknown )
							return true;
						return false;
					}
				),
				headers.end()
			);

			// Sort headers by offset increment
			std::sort(
				headers.begin(), 
				headers.end(), 
				[](const FVHeaderData& a, const FVHeaderData& b) -> bool 
				{
					return a.first < b.first; 
				}
			);

			// Create view entries
			std::for_each(
				headers.begin(),
				headers.end(),
				[&result](const FVHeaderData& a) { result.emplace_back(a.second); }
			);

#ifdef DEBUG

			std::for_each(
				result.begin(),
				result.end(),
				[](const Pi::Volume::Header& header) 
				{
					DEBUG_INFO_MESSAGE
						DEBUG_PRINT("\tMessage: Firmware volume found");
						DEBUG_PRINT("\tGUID: ", FfsGuids::isValidFfsGuid(reinterpret_cast<Types::const_pointer_t>(&header->FileSystemGuid)).name);
					DEBUG_END_MESSAGE
				}
			);
#endif
			return result;
		}

	}
}