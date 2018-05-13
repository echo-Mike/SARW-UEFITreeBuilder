/// STD
#include <algorithm>

/// PROJECT
#include "PiObjectRepresentation.hpp"

namespace Project {

namespace PiObject {

Types::byte_t default_empty = PROJ_DEFAULT_EMPTY;

/**
 *    OBJECT
 */

void Object::setUid(const MemoryView& buffer)
{
	uid = Checksums::hash(buffer, buffer.getLength());
}

void Object::setUid(MemoryView&& buffer)
{
	uid = Checksums::hash(buffer, buffer.getLength());
}

/**
 *    FREE_SPACE
 */

bool FreeSpace::IsCorrupted(Types::byte_t empty, const MemoryView& buffer)
{
	return (buffer.end != std::find_if_not(
		buffer.begin,
		buffer.end,
		[empty](const Types::byte_t v) -> bool
		{ 
			return v == empty; 
		}
	));
}

Section::decomp_data_storage_ptr_t Section::getDecomressedDataStorage()
{
	static decomp_data_storage_t DecompressedSectionData;
	return &DecompressedSectionData;
	/*
	decomp_data_storage_ptr_t result = nullptr;
	if (header.sectionType == Helper::SectionHeader::Compression ||
		header.sectionType == Helper::SectionHeader::GuidDefined) {
		result = &DecompressedSectionData;
	} else DEBUG_WARNING_MESSAGE
		DEBUG_PRINT("\tMessage: Can't access decompressed data storage.");
		DEBUG_PRINT("\tSection UID: ", getUid());
		DEBUG_PRINT("\tSection type: ", header.sectionType);
	DEBUG_END_MESSAGE;
	return result;
	*/
}

}

}