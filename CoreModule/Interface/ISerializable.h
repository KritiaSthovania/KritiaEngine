#pragma once
#include <json/json.hpp>
#include <string>
using json = nlohmann::ordered_json;
namespace KritiaEngine{

	//////////////////////////// Serializables ////////////////////////////////
	class IJsonSerializable {
	public:
		virtual std::string SerializeToJson() = 0;
	};

	class IFileSerializable {
	public:
		virtual void SerializeToFile() = 0;
	};
	/////////////////////////// Deserializables //////////////////////////////
	class IJsonDeserializable {
	public:
		virtual void DeserializeFromJson(const json& json) = 0;
	};

	class PathDeserializable {
	public:
		virtual void DeserializeFromPath(const std::string& path) = 0;
	};

}

