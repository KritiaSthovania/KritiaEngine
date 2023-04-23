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
		Color(const Vector3& rgb, float a);
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
		Bound();
		Vector3 center;
		// two times of the extent
		Vector3 size;
		// half of the size
		Vector3 GetExtent() const;
		Vector3 GetMin() const;
		Vector3 GetMax() const;
	};

	class Collider;
	class GameObject;
	class RigidBody;

	struct ContactPoint {
		ContactPoint(const Vector3& normal, const Vector3& position, std::shared_ptr<Collider> self, std::shared_ptr<Collider> other) {
			this->normal = normal;
			this->position = position;
			thisCollider = self;
			otherCollider = other;
		}
		Vector3 normal;
		Vector3 position;
		std::shared_ptr<Collider> thisCollider;
		std::shared_ptr<Collider> otherCollider;
	};

	struct Collision {
		Collision() {
			selfCollider = nullptr;
			otherCollider = nullptr;
			rigidBody = nullptr;
			gameObject = nullptr;
			contactPoints = std::vector<ContactPoint>();
		}
		std::shared_ptr<Collider> selfCollider;
		std::shared_ptr<Collider> otherCollider;
		std::shared_ptr<RigidBody> rigidBody;
		GameObject* gameObject;
		std::vector<ContactPoint> contactPoints;
	};
}
