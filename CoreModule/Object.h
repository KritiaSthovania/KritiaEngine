#pragma once
#include <pch.h>
#include <json/json.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace KritiaEngine{
	class Object
	{
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
