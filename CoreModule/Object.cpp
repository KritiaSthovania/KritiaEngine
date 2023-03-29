#include "Object.h"
using namespace KritiaEngine;

void KritiaEngine::Object::Destroy(Object* obj) {
	obj->OnObjectDestroy();
}

void KritiaEngine::Object::GenerateGuid() {
	boost::uuids::uuid uid = boost::uuids::random_generator()();
	guid = boost::uuids::to_string(uid);
}

void KritiaEngine::Object::OnObjectDestroy() {
}