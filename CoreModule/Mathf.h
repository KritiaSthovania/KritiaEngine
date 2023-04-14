#pragma once
#include "MathStructs.h"
namespace KritiaEngine {
	// common math functions
	class Mathf
	{
	public:
		const static float PI;
		static float Cos(float degrees);
		static float Sin(float degrees);
		static float Tan(float degrees);
		static float Radians(float degrees);
		static float Sqrt(float number);
		static float Sign(float value);
		/// <summary>
		/// Translate a matrix by a given vector
		/// </summary>
		/// <param name="mat4">The matrix to translate</param>
		/// <param name="vec3">Translation vector</param>
		/// <returns></returns>
		static Matrix4x4 Translate(const Matrix4x4 &mat4, const Vector3 &vec3);
		/// <summary>
		/// Rotate a matrix by a given euler angle and axis
		/// </summary>
		/// <param name="mat4"> The matrix to rotate</param>
		/// <param name="angle"> Euler angle</param>
		/// <param name="axis"> Rotation axis </param>
		/// <returns></returns>
		static Matrix4x4 Rotate(const Matrix4x4 &mat4, float angle, const Vector3 &axis);

		/// <summary>
		/// Scale a matrix by a scale vector
		/// </summary>
		/// <param name="mat4"> The matrix to scale </param>
		/// <param name="scale"> Scale vector</param>
		/// <returns></returns>
		static Matrix4x4 Scale(const Matrix4x4 &mat4, const Vector3 &scale);

	};
}

