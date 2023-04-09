#include "Behaviour.h"

void KritiaEngine::Behaviour::OnInspector() {
	ImguiAlias::BoolField("Enabled", &enabled);
}

std::string KritiaEngine::Behaviour::SerializeToJson() {
	json json;
	json["Enabled"] = enabled;
	return json.dump();
}

void KritiaEngine::Behaviour::DeserializeFromJson(const json& json) {
	enabled = json["Enabled"];
}


