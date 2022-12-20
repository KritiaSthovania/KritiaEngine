#pragma once
#include "MathStructs.h"

namespace KritiaEngine {
	struct Color {
		friend std::ostream& operator<<(std::ostream& cout, Color color);

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
		Vector3 RGB();
		friend Color operator*(float a, const Color& vec);
		Color operator+ (const Color& vec);
		Color operator- (const Color& vec);
		Color operator* (float a);
		Color operator/ (float a);
		void operator+= (const Color& vec);
		void operator-= (const Color& vec);
		void operator*= (float a);
		void operator/= (float a);
	};
}
