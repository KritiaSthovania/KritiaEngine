#pragma once

#include "MathStructs.h"
#include <sstream>

namespace KritiaEngine::Editor::GUI {
	class ConsoleWindow;
}

namespace KritiaEngine {
	class Debug {
		friend class KritiaEngine::Editor::GUI::ConsoleWindow;
	public:
		template <typename T> static void Log(const T& message) {
			logStream << message << std::endl;
		}
	private:
		static std::ostringstream logStream;
	};

	struct Color {
		friend std::ostream& operator<<(std::ostringstream& cout, Color& color);

	public:
		/// <summary>
		/// The white color with alpha = 1
		/// </summary>
		Color();
		/// <summary>
		/// r,g,b,a should in 0 to 1;
		/// </summary>
		Color(float r, float g, float b, float a);
		Color(const Vector4& vec4);
		float r;
		float g;
		float b;
		float a;
		/// <summary>
		/// Get the RGB components
		/// </summary>
		Vector3 GetRGB();
		friend Color operator*(float a, const Color& vec);
		Color operator+ (const Color& vec);
		Color operator- (const Color& vec);
		Color operator* (float a);
		Color operator/ (float a);
		void operator+= (const Color& vec);
		void operator-= (const Color& vec);
		void operator*= (float a);
		void operator/= (float a);
		operator std::vector<float>() const;
	};

	/// <summary>
	/// An Axis-Aligned Bounding Box (AABB)
	/// </summary>
	struct Bound {
		friend std::ostream& operator<<(std::ostringstream& cout, Bound& bound);
	public:
		Vector3 center;
		// two times of the extent
		Vector3 size;
		// half of the size
		Vector3 GetExtent() const;
		Vector3 GetMin() const;
		Vector3 GetMax() const;
	};
}
