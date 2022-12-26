#pragma once
#include <pch.h>

namespace KritiaEngine{
	class Object
	{
	public:
		static void Destroy(Object* obj);
		const char* name;
		const char* guid;
	private:
		// called internally when an object is destroyed.
		virtual void OnObjectDestroy();
	};

}
