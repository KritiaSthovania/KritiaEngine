#pragma once
#include <pch.h>
#include <json/json.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>


namespace KritiaEngine{
	namespace Editor::GUI {
		class ImguiManager;
	}

	class Object
	{
		friend class KritiaEngine::Editor::GUI::ImguiManager;
	public:
		static void Destroy(Object* obj);
		std::string name;
	protected:		
		Object() = default;
		std::string guid = "";
		void GenerateGuid();
	private:
		// called internally when an object is destroyed.
		virtual void OnObjectDestroy();
	};

}
