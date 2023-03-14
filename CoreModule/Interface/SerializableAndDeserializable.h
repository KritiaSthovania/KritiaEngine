#pragma once
#include <json/json.hpp>
#include <string>
using json = nlohmann::ordered_json;
namespace KritiaEngine{

	//////////////////////////// Serializables ////////////////////////////////
	class JsonSerializable {
	public:
		virtual std::string Serialize() = 0;
	};

	class FileSerializable {
	public:
		virtual void SerializeToFile() = 0;
	};
	/////////////////////////// Deserializables //////////////////////////////
	class JsonDeserializable {
	public:
		virtual void DeserializeFromJson(const json& json) = 0;
	};

	class PathDeserializable {
	public:
		virtual void DeserializeFromPath(const std::string& path) = 0;
	};

}

