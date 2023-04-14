#include "Mathf.h"
#include "Settings.h"
using namespace KritiaEngine;

const float KritiaEngine::Mathf::PI = 3.14159265358979323846264338327950288;

float KritiaEngine::Mathf::Cos(float degrees)
{
	return glm::cos(degrees / 180 * PI);
}

float KritiaEngine::Mathf::Sin(float degrees)
{
	return glm::sin(degrees / 180 * PI);
}

float KritiaEngine::Mathf::Tan(float degrees)
{
	return glm::tan(degrees / 180 * PI);
}

float KritiaEngine::Mathf::Radians(float degrees)
{
	return glm::radians(degrees);
}

float KritiaEngine::Mathf::Sqrt(float number) {
	return glm::sqrt(number);
}

float KritiaEngine::Mathf::Sign(float value) {
	if (value > 0) {
		return 1;
	} else if (value < 0) {
		return -1;
	} else {
		return 0;
	}
}

Matrix4x4 KritiaEngine::Mathf::Translate(const Matrix4x4& mat4, const Vector3& vec3)
{
	if (Settings::UseGLM) {
		glm::mat4 mat = glm::translate(Matrix4x4::ToGlmMat4(mat4), Vector3::ToGlmVec3(vec3));
		return Matrix4x4(mat);
	}
}

Matrix4x4 KritiaEngine::Mathf::Rotate(const Matrix4x4 &mat4, float angle, const Vector3 &axis)
{
	if (Settings::UseGLM) {
		glm::mat4 mat = glm::rotate(Matrix4x4::ToGlmMat4(mat4), angle, Vector3::ToGlmVec3(axis));
		return Matrix4x4(mat);
	}
}

Matrix4x4 KritiaEngine::Mathf::Scale(const Matrix4x4 &mat4, const Vector3 &scale)
{
	if (Settings::UseGLM) {
		glm::mat4 mat = glm::scale(Matrix4x4::ToGlmMat4(mat4), Vector3::ToGlmVec3(scale));
		return Matrix4x4(mat);
	}

}

