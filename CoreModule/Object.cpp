#include "Object.h"
using namespace KritiaEngine;

void KritiaEngine::Object::Destroy(Object* obj) {
	obj->OnObjectDestroy();
}

void KritiaEngine::Object::OnObjectDestroy() {}