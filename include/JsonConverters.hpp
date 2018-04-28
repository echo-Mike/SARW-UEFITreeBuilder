#pragma once
#ifndef JSON_CONVERTERS_HPP__
#define JSON_CONVERTERS_HPP__ "0.0.0@JsonConverters.hpp"

/// JSON
#include <nlohmann/json.hpp>

/// PROJECT
#include "MemoryView.hpp"
#include "GuidDefines.h"
#include "PiViews.hpp"
#include "PiObjectRepresentation.hpp"

/**
 *    Free converters
 */

void to_json(nlohmann::json& j, const EFI_GUID& guid);

/**
 *    Namespace-ed converters
 */

namespace Project
{

	inline void to_json(nlohmann::json& j, const OffsetView& obj)
	{
		j["offset"] = obj.offset;
		j["length"] = obj.length;
		j["valid"] = obj.valid;
	}

	namespace Guid
	{
		void to_json(nlohmann::json& j, const Project::Guid::GuidWithName& guid);
	}

	void to_json(nlohmann::json& j, const Pi::Volume::Header& hdr);

	void to_json(nlohmann::json& j, const Pi::File::Header& hdr);

	void to_json(nlohmann::json& j, const Pi::Section::Header& hdr);

	namespace PiObject
	{
		inline void to_json(nlohmann::json& j, const Object& obj) { (&obj)->toJson(j); }

		inline void to_json(nlohmann::json& j, const unique_object_ptr_t& obj_ptr) { obj_ptr->toJson(j); }

		namespace Helper
		{
			void to_json(nlohmann::json& j, const SectionHeader& obj);

			void to_json(nlohmann::json& j, const unique_section_decomp_buff_t& obj);

			void to_json(nlohmann::json& j, const FileHeader& obj);
		}
	}

}

#endif