#pragma once

#include <vector>
#include <pch.h>
#include <stb_image.h>
#include "../Component.h"
#include "../Camera.h"
#include "../../CoreModule/Manager/RendererManager.h"
namespace KritiaEngine{
	class Renderer : public Component {
		friend class KritiaEngine::Manager::RendererManager;
	protected:
		virtual void Render(const std::shared_ptr<Camera>& camera) = 0;
	};

}


