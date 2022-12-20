#include "Light.h"
#include "../Component/Transform.h"
#include "../CoreModule/GameObject.h"


KritiaEngine::Light::Light(GameObject* gameObject)
{
	this->gameObject = gameObject;
	this->color = Color();
	this->type = Directional;
}
