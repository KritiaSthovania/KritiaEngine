#include "Object.h"
using namespace KritiaEngine;

void KritiaEngine::Object::Destroy(Object* obj) {
	
}

void KritiaEngine::Object::OnObjectDestroy() {}

std::string KritiaEngine::Object::Serialize() {
	return "Virtual serialize function of Object is called.";
}

