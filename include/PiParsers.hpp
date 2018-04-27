#pragma once
#ifndef PI_PARSERS_HPP__
#define PI_PARSERS_HPP__ "0.0.0@PiParsers.hpp"

/// STD
#include <vector>

/// PROJECT
#include "General.hpp"
#include "PiViews.hpp"
#include "PiObjectRepresentation.hpp"

namespace Project
{

	namespace Parsers
	{

		typedef Types::length_t(LengthFunction)(const MemoryView&);

		template < typename SubObjT, LengthFunction calcLen, typename ParserFuncT, typename VecT >
		void processHeaders(
			ParserFuncT parser,
			PiObject::ComplexObject& baseObject, 
			VecT headerVec, 
			const MemoryView& buffer,
			Types::memory_t empty,
			const char* errMsg,
			PiObject::InconsistencyState::InconsistencyState_t errorState)
		{
			MemoryView subObjMemory(buffer.begin, buffer.begin);

			// Call parser for every header entry
			for (const auto& hdr : headerVec)
			{	// Check that previous object fill all space before current
				if (subObjMemory.end < hdr.begin) {
					baseObject.emplace_back<PiObject::FreeSpace>(empty, baseObject.memory, MemoryView(subObjMemory.end, hdr.begin));
				}

				subObjMemory.begin = hdr.begin;
				subObjMemory.end = hdr.begin + calcLen(hdr);

				if (buffer.isOutside(subObjMemory.end - 1)) DEBUG_ERROR_MESSAGE
					OffsetView ov(baseObject.memory.begin, subObjMemory);
					DEBUG_PRINT(errMsg);
					DEBUG_PRINT("\tSub-object offset: ", ov.offset);
					DEBUG_PRINT("\tSub-object length: ", ov.length);
					DEBUG_PRINT("\tBuffer length: ", baseObject.memory.getLength());
					DEBUG_PRINT("\tExceeds by: ", ov.offset + ov.length - baseObject.memory.getLength());
				DEBUG_END_MESSAGE_AND_EVAL({
					auto ptr = std::make_unique<PiObject::Data>(buffer, subObjMemory);
					ptr->state |= errorState;
					baseObject.objects.emplace_back(std::move(ptr));
					continue;
				})
				baseObject.emplace_back<SubObjT>( parser(hdr, subObjMemory, baseObject.memory, empty) );
			}

			// Check for space after last object
			if (subObjMemory.end < buffer.end) {
				baseObject.emplace_back<PiObject::FreeSpace>(empty, baseObject.memory, MemoryView(subObjMemory.end, buffer.end));
			}
		}

		PiObject::object_vec_t FreeSpaceParser(const MemoryView& buffer);

		PiObject::Section SectionParser(const Pi::Section::Header& sectionView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty);

		PiObject::File FileParser(const Pi::File::Header& fileView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t empty);

		PiObject::Volume VolumeParser(const Pi::Volume::Header& volumeView, const MemoryView& buffer, const MemoryView& baseBuffer, Types::memory_t = 0x00);

	}

}

#endif