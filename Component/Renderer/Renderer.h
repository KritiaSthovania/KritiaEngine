#pragma once

#include <vector>
#include <pch.h>
#include <stb_image.h>
#include "../Component.h"
#include "../Camera.h"

namespace KritiaEngine{
	class Renderer : public Component {
	public:
		virtual void Render(std::shared_ptr<Camera> camera) = 0;
	};

}


