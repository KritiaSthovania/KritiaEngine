#pragma once
#include <pch.h>
#include <json/json.hpp>

namespace KritiaEngine{
	class Object
	{
	public:
		static void Destroy(Object* obj);
		std::string name;
	private:
		// called internally when an object is destroyed.
		virtual void OnObjectDestroy();
		/// <summary>
		/// Serialize to a json string
		/// </summary>
		virtual std::string Serialize();
		const char* guid;
	};

}
