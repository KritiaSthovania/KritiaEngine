#include "MathStructs.h"
#include "Settings.h"
#include "Utilities.h"
#include "Mathf.h"

using namespace KritiaEngine;

constexpr float epsilon = 1e-6;

KritiaEngine::Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

KritiaEngine::Vector2::Vector2(const glm::vec2& vec) {
	this->x = vec.x;
	this->y = vec.y;
}

KritiaEngine::Vector2::Vector2() {
	this->x = this->y = 0;
}

float KritiaEngine::Vector2::Cross(const Vector2& vec1, const Vector2& vec2) {
	return vec1.x * vec2.y - vec1.y * vec2.x;
}

Vector2 KritiaEngine::Vector2::Zero() {
	return Vector2();
}

Vector2 KritiaEngine::Vector2::operator-(const Vector2& vec) const {
	return Vector2(this->x - vec.x, this->y - vec.y);
}

Vector2 KritiaEngine::Vector2::operator+(const Vector2& vec) const {
	return Vector2(this->x + vec.x, this->y + vec.y);
}

Vector2 KritiaEngine::Vector2::operator*(const Vector2& vec) const {
	return Vector2(x * vec.x, y * vec.y);
}

void KritiaEngine::Vector2::operator-=(const Vector2& vec) {
	x -= vec.x;
	y -= vec.y;
}

Vector2 KritiaEngine::Vector2::operator*(float a) const {
	return Vector2(a * x, a * y);
}

Vector2 KritiaEngine::Vector2::operator/(float a) const {
	return Vector2(x / a, y / a);
}

KritiaEngine::Vector2::operator glm::vec2() const {
	return glm::vec2(x, y);
}

std::ostringstream& KritiaEngine::operator<<(std::ostringstream& cout, const Vector2& vec) {
	cout << "(" << vec.x << ", " << vec.y << ")";
	return cout;
}


//----------Vector3------------------------

KritiaEngine::Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

KritiaEngine::Vector3::Vector3(const glm::vec3 &vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

KritiaEngine::Vector3::Vector3()
{
	this->x = this->y = this->z = 0;
}

Vector3 KritiaEngine::Vector3::Zero()
{
	return Vector3();
}

Vector3 KritiaEngine::Vector3::Normalize(const Vector3 &vec)
{
	if (vec != Vector3::Zero()) {
		float temp = Mathf::Sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
		return Vector3(vec.x / temp, vec.y / temp, vec.z / temp);
	} else {
		return Vector3::Zero();
	}

}

Vector3 KritiaEngine::Vector3::Cross(const Vector3 &vec1, const Vector3 &vec2)
{
	Vector3 vec = Vector3(vec1.y * vec2.z - vec2.y * vec1.z,
		vec1.z * vec2.x - vec2.z * vec1.x,
		vec1.x * vec2.y - vec2.x * vec1.y);
	return vec;
}

Vector3 KritiaEngine::Vector3::Project(const Vector3& point, const Vector3& axis) {
	Vector3 dir = Vector3::Normalize(axis);
	return Vector3::Dot(point, dir) * dir;
}

float KritiaEngine::Vector3::Dot(const Vector3& vec1, const Vector3& vec2) {
	return glm::dot((glm::vec3)vec1, (glm::vec3)vec2);
}

float KritiaEngine::Vector3::Magnitude(const Vector3& vec) {
	return glm::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

glm::vec3 KritiaEngine::Vector3::ToGlmVec3(const Vector3 &vec3)
{
	return glm::vec3(vec3.x, vec3.y, vec3.z);
}

Vector3 KritiaEngine::Vector3::operator+(const Vector3 &vec)
{
	return Vector3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
}

Vector3 KritiaEngine::Vector3::operator-(const Vector3 &vec)
{
	return Vector3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
}

Vector3 KritiaEngine::Vector3::operator*(float a)
{
	return Vector3(this->x * a, this->y * a, this->z * a);
}

Vector3 KritiaEngine::Vector3::operator*(const Vector3& vec) {
	return Vector3(this->x * vec.x, this->y * vec.y, this->z * vec.z);
}

Vector3 KritiaEngine::Vector3::operator/(float a)
{
	return Vector3(this->x / a, this->y / a, this->z / a);
}

void KritiaEngine::Vector3::operator+=(const Vector3 &vec)
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;
}

void KritiaEngine::Vector3::operator-=(const Vector3 &vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	this->z -= vec.z;
}

void KritiaEngine::Vector3::operator*=(float a)
{
	this->x *= a;
	this->y *= a;
	this->z *= a;
}

void KritiaEngine::Vector3::operator/=(float a)
{
	this->x /= a;
	this->y /= a;
	this->z /= a;
}

bool KritiaEngine::Vector3::operator==(const Vector3& vec) {
	return abs(x - vec.x) < epsilon && abs(y - vec.y) < epsilon && abs(z - vec.z) < epsilon;
}

bool KritiaEngine::Vector3::operator!=(const Vector3& vec) {
	return !(*this==vec);
}

KritiaEngine::Vector3::operator glm::vec3() const
{
	return glm::vec3(x, y, z);
}

KritiaEngine::Vector3::operator std::vector<float>() const {
	return std::vector<float>({ x,y,z });
}

Vector2 KritiaEngine::operator*(float a, const Vector2& vec) {
	return Vector2(a * vec.x, a * vec.y);
}

Vector2 KritiaEngine::operator/(float a, const Vector2& vec) {
	return Vector2(a / vec.x, a / vec.y);
}

std::ostringstream& KritiaEngine::operator<<(std::ostringstream& cout, const Vector3& vec) {
	cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return cout;
}

Vector3 KritiaEngine::operator-(const Vector3& self) {
	return Vector3(-self.x, -self.y, -self.z);
}

Vector3 KritiaEngine::operator*(float a, const Vector3& vec)
{
	return Vector3(a * vec.x, a * vec.y, a * vec.z);
}

Vector3 KritiaEngine::operator+(const Vector3& vec1, const Vector3& vec2) {
	return Vector3(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z);
}

Vector3 KritiaEngine::operator-(const Vector3& vec1, const Vector3& vec2) {
	return Vector3(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z);
}


//----------------Vector4--------------------------

KritiaEngine::Vector4::Vector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

KritiaEngine::Vector4::Vector4()
{
	this->x = this->y = this->z = this->w = 0;
}

KritiaEngine::Vector4::Vector4(glm::vec4 vec)
{
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
	this->w = vec.w;
}

KritiaEngine::Vector4::Vector4(const Vector3& vec, float w) {
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
	this->w = w;
}

Vector4 KritiaEngine::Vector4::Zero()
{
	return Vector4();
}

glm::vec4 KritiaEngine::Vector4::ToGlmVec4(const Vector4 &vec4)
{
	return glm::vec4(vec4.x, vec4.y, vec4.z, vec4.w);
}


Vector4 KritiaEngine::Vector4::operator+(const Vector4& vec) const
{
	return Vector4(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w);
}

Vector4 KritiaEngine::Vector4::operator-(const Vector4& vec) const
{
	return Vector4(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w);
}

Vector4 KritiaEngine::Vector4::operator*(float a) const
{
	return Vector4(this->x * a, this->y * a, this->z * a, this->w * a);
}

Vector4 KritiaEngine::Vector4::operator/(float a) const
{
	return Vector4(this->x / a, this->y / a, this->z / a, this->w / a);
}

void KritiaEngine::Vector4::operator+=(const Vector4& vec)
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;
	this->w += vec.w;
}

void KritiaEngine::Vector4::operator-=(const Vector4& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	this->z -= vec.z;
	this->w -= vec.w;
}

void KritiaEngine::Vector4::operator*=(float a)
{
	this->x *= a;
	this->y *= a;
	this->z *= a;
	this->w *= a;
}

void KritiaEngine::Vector4::operator/=(float a)
{
	this->x /= a;
	this->y /= a;
	this->z /= a;
	this->w /= a;
}

bool KritiaEngine::Vector4::operator==(const Vector4& vec) {
	return abs(x - vec.x) < epsilon && abs(y - vec.y) < epsilon && abs(z - vec.z) < epsilon && abs(w - vec.w) < epsilon;
}

bool KritiaEngine::Vector4::operator!=(const Vector4& vec) {
	return !(*this == vec);
}

KritiaEngine::Vector4::operator glm::vec4() const
{
	return glm::vec4(x, y, z, w);
}

KritiaEngine::Vector4::operator Vector3() const {
	return Vector3(x, y, z);
}

KritiaEngine::Vector4::operator std::vector<float>() const {
	return std::vector<float>({ x, y, z, w });
}

std::ostringstream& KritiaEngine::operator<<(std::ostringstream& cout, const Vector4& vec) {
	cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
	return cout;
}

Vector4 KritiaEngine::operator*(float a, const Vector4& vec)
{
	return Vector4(a * vec.x, a * vec.y, a * vec.z, a * vec.w);
}

//---------Matrix4x4--------------

glm::mat4 KritiaEngine::Matrix4x4::ToGlmMat4(const Matrix4x4 &mat4)
{
	glm::vec4 column0 = glm::vec4(mat4.entries[0][0], mat4.entries[0][1], mat4.entries[0][2], mat4.entries[0][3]);
	glm::vec4 column1 = glm::vec4(mat4.entries[1][0], mat4.entries[1][1], mat4.entries[1][2], mat4.entries[1][3]);
	glm::vec4 column2 = glm::vec4(mat4.entries[2][0], mat4.entries[2][1], mat4.entries[2][2], mat4.entries[2][3]);
	glm::vec4 column3 = glm::vec4(mat4.entries[3][0], mat4.entries[3][1], mat4.entries[3][2], mat4.entries[3][3]);
	return glm::mat4(column0, column1, column2, column3);
}

Matrix4x4 KritiaEngine::Matrix4x4::operator*(const Matrix4x4 &mat) const
{
	if (Settings::UseGLM) {
		return Matrix4x4(ToGlmMat4(*this) * ToGlmMat4(mat));
	}
}

Vector4 KritiaEngine::Matrix4x4::operator*(const Vector4& vec4) const {
	return (glm::mat4)(*this) * (glm::vec4)(vec4);
}

void KritiaEngine::Matrix4x4::operator*=(const Matrix4x4& mat) {
	*this = *this * mat;
}

KritiaEngine::Matrix4x4::operator glm::mat4() const
{
	glm::vec4 column0 = glm::vec4(entries[0][0], entries[0][1], entries[0][2], entries[0][3]);
	glm::vec4 column1 = glm::vec4(entries[1][0], entries[1][1], entries[1][2], entries[1][3]);
	glm::vec4 column2 = glm::vec4(entries[2][0], entries[2][1], entries[2][2], entries[2][3]);
	glm::vec4 column3 = glm::vec4(entries[3][0], entries[3][1], entries[3][2], entries[3][3]);
	return glm::mat4(column0, column1, column2, column3);
}

KritiaEngine::Matrix4x4::operator Matrix3x3() const {
	float entry[3][3] = { entries[0][0], entries[0][1], entries[0][2], entries[1][0], entries[1][1], entries[1][2], entries[2][0], entries[2][1], entries[2][2] };
	return Matrix3x3(entry);
}

KritiaEngine::Matrix4x4::Matrix4x4() : entries{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
{
}

KritiaEngine::Matrix4x4::Matrix4x4(const Vector4 &column0, const Vector4 &column1, const Vector4 &column2, const Vector4 &column3) : entries{column0.x, column0.y, column0.z, column0.w, 
column1.x, column1.y, column1.z, column1.w, column2.x, column2.y, column2.z, column2.w ,column3.x, column3.y, column3.z, column3.w }
{
}

KritiaEngine::Matrix4x4::Matrix4x4(const float entry[4][4]) : entries{ entry[0][0], entry[0][1], entry[0][2], entry[0][3],
															     entry[1][0], entry[1][1], entry[1][2], entry[1][3],
															     entry[2][0], entry[2][1], entry[2][2], entry[2][3],
															     entry[3][0], entry[3][1], entry[3][2], entry[3][3] }
{
}

KritiaEngine::Matrix4x4::Matrix4x4(const glm::mat4 &mat) : entries{ mat[0][0], mat[0][1], mat[0][2], mat[0][3],
	                                                         mat[1][0], mat[1][1], mat[1][2], mat[1][3],
	                                                         mat[2][0], mat[2][1], mat[2][2], mat[2][3],
	                                                         mat[3][0], mat[3][1], mat[3][2], mat[3][3] }
{

}

KritiaEngine::Matrix4x4::Matrix4x4(const Matrix3x3& mat) {
	*this = glm::mat4((glm::mat3)mat);
}

Matrix4x4 KritiaEngine::Matrix4x4::Identity()
{
	float entries[4][4] = {1, 0, 0, 0,
		                  0, 1, 0, 0,
		                  0, 0, 1, 0,
		                  0, 0, 0, 1 };
	return Matrix4x4(entries);
}

Matrix4x4 KritiaEngine::Matrix4x4::LookAt(const Vector3 &eye, const Vector3 &center, const Vector3 &up)
{
	if (Settings::UseGLM) {
		return Matrix4x4(glm::lookAt(Vector3::ToGlmVec3(eye), Vector3::ToGlmVec3(center), Vector3::ToGlmVec3(up)));
	}

}

Vector4 KritiaEngine::Matrix4x4::GetColumn(int index) const
{
	switch (index) {
	case 0:
		return Vector4(entries[0][0], entries[0][1], entries[0][2], entries[0][3]);
	case 1:
		return Vector4(entries[1][0], entries[1][1], entries[1][2], entries[1][3]);
	case 2:
		return Vector4(entries[2][0], entries[2][1], entries[2][2], entries[2][3]);
	case 3:
		return Vector4(entries[3][0], entries[3][1], entries[3][2], entries[3][3]);
	}
}

Vector4 KritiaEngine::Matrix4x4::GetRow(int index) const
{
	switch (index) {
	case 0:
		return Vector4(entries[0][0], entries[1][0], entries[2][0], entries[3][0]);
	case 1:
		return Vector4(entries[0][1], entries[1][1], entries[2][1], entries[3][1]);
	case 2:
		return Vector4(entries[0][2], entries[1][2], entries[2][2], entries[3][2]);
	case 3:
		return Vector4(entries[0][3], entries[1][3], entries[2][3], entries[3][3]);
	}
}

void KritiaEngine::Matrix4x4::SetColumn(int index, const Vector4 &column)
{
	switch (index)
	{
	case 0:
		entries[0][0] = column.x;
		entries[0][1] = column.y;
		entries[0][2] = column.z;
		entries[0][3] = column.w;
	case 1:
		entries[1][0] = column.x;
		entries[1][1] = column.y;
		entries[1][2] = column.z;
		entries[1][3] = column.w;
	case 2:
		entries[2][0] = column.x;
		entries[2][1] = column.y;
		entries[2][2] = column.z;
		entries[2][3] = column.w;
	case 3:
		entries[3][0] = column.x;
		entries[3][1] = column.y;
		entries[3][2] = column.z;
		entries[3][3] = column.w;
	}
}

void KritiaEngine::Matrix4x4::SetRow(int index, const Vector4 &row)
{
	switch (index)
	{
	case 0:
		entries[0][0] = row.x;
		entries[1][0] = row.y;
		entries[2][0] = row.z;
		entries[3][0] = row.w;
	case 1:
		entries[0][1] = row.x;
		entries[1][1] = row.y;
		entries[2][1] = row.z;
		entries[3][1] = row.w;
	case 2:
		entries[0][2] = row.x;
		entries[1][2] = row.y;
		entries[2][2] = row.z;
		entries[3][2] = row.w;
	case 3:
		entries[0][3] = row.x;
		entries[1][3] = row.y;
		entries[2][3] = row.z;
		entries[3][3] = row.w;
	}
}

float KritiaEngine::Matrix4x4::GetEntry(int row, int column) const
{
	return entries[column][row];
}

void KritiaEngine::Matrix4x4::SetEntry(int row, int column, int newEntry)
{
	entries[column][row] = newEntry;
}

float* KritiaEngine::Matrix4x4::GetPtr() {
	return entries[0];
}

Matrix4x4 KritiaEngine::Matrix4x4::Perspective(float fovy, float aspect, float near, float far)
{
	if (Settings::UseGLM) {
		return Matrix4x4(glm::perspective(glm::radians(fovy), aspect, near, far));
	}
}

Matrix4x4 KritiaEngine::Matrix4x4::Ortho(float left, float right, float bottom, float top, float near, float far) {
	if (Settings::UseGLM) {
		return glm::ortho(left, right, bottom, top, near, far);
	}
}

KritiaEngine::Matrix3x3::Matrix3x3() : entries{0}
{

}

KritiaEngine::Matrix3x3::Matrix3x3(const Vector3& column0, const Vector3& column1, const Vector3& column2) : entries{
	column0.x, column0.y, column0.z, column1.x, column1.y, column1.z, column2.x, column2.y, column2.z
}
{

}

KritiaEngine::Matrix3x3::Matrix3x3(const float entry[3][3]) : entries{ entry[0][0], entry[0][1], entry[0][2],  
                                                                       entry[1][0], entry[1][1], entry[1][2],
																       entry[2][0], entry[2][1], entry[2][2] }
{
}

KritiaEngine::Matrix3x3::Matrix3x3(const glm::mat3& mat) : entries{ mat[0][0], mat[0][1], mat[0][2],
		                                                            mat[1][0], mat[1][1], mat[1][2],
		                                                            mat[2][0], mat[2][1], mat[2][2] }
{
}

Matrix3x3 KritiaEngine::Matrix3x3::Inverse()
{
	if (Settings::UseGLM) {
		return glm::inverse((glm::mat3)*this);
	}
}

Matrix3x3 KritiaEngine::Matrix3x3::Transpose()
{
	if (Settings::UseGLM) {
		return glm::transpose((glm::mat3)*this);
	}
}

Matrix3x3 KritiaEngine::Matrix3x3::Identity()
{
	float entries[3][3] = { 1,0,0,
						    0,1,0,
						    0,0,1 };
	return Matrix3x3(entries);
}

glm::mat3 KritiaEngine::Matrix3x3::ToGlmMat3(const Matrix3x3 &mat)
{
	glm::vec3 column0 = glm::vec3(mat.entries[0][0], mat.entries[0][1], mat.entries[0][2]);
	glm::vec3 column1 = glm::vec3(mat.entries[1][0], mat.entries[1][1], mat.entries[1][2]);
	glm::vec3 column2 = glm::vec3(mat.entries[2][0], mat.entries[2][1], mat.entries[2][2]);
	return glm::mat3(column0, column1, column2);
}

KritiaEngine::Matrix3x3::operator glm::mat3() const
{
	glm::vec3 column0 = glm::vec3(entries[0][0], entries[0][1], entries[0][2]);
	glm::vec3 column1 = glm::vec3(entries[1][0], entries[1][1], entries[1][2]);
	glm::vec3 column2 = glm::vec3(entries[2][0], entries[2][1], entries[2][2]);
	return glm::mat3(column0, column1, column2);
}

Vector3 KritiaEngine::Matrix3x3::operator*(const Vector3& vec3) const {
	return (glm::mat3)*this * (glm::vec3)vec3;
}

Matrix3x3 KritiaEngine::Matrix3x3::operator*(const Matrix3x3& mat) const {
	return (glm::mat3)*this * glm::mat3(mat);
}



KritiaEngine::Quaternion::Quaternion() {
	x = y = z = 0;
	w = 1;
}

KritiaEngine::Quaternion::Quaternion(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

KritiaEngine::Quaternion::Quaternion(const glm::quat& quat) {
	x = quat.x;
	y = quat.y;
	z = quat.z;
	w = quat.w;
}

KritiaEngine::Quaternion::Quaternion(const Matrix4x4& mat) {
	Quaternion quat = FromRoationMatrix(mat);
	this->x = quat.x;
	this->y = quat.y;
	this->z = quat.z;
	this->w = quat.w;
}

KritiaEngine::Quaternion::Quaternion(const Vector3& vec1, const Vector3& vec2) {
	Quaternion quat = FromTwoVectors(vec1, vec2);
	this->x = quat.x;
	this->y = quat.y;
	this->z = quat.z;
	this->w = quat.w;
}

Vector3 KritiaEngine::Quaternion::ToEuler(const Quaternion& quat) {
	Vector3 vec = glm::eulerAngles((glm::quat)quat);
	vec.x = glm::degrees(vec.x);
	vec.y = glm::degrees(vec.y);
	vec.z = glm::degrees(vec.z);
	return vec;
}

Quaternion KritiaEngine::Quaternion::Normalize(const Quaternion& quat) {
	return Quaternion(glm::normalize((glm::quat)quat));
}

Quaternion KritiaEngine::Quaternion::Conjugate(const Quaternion& quat) {
	return Quaternion(-quat.x, -quat.y, -quat.z, quat.w);
}

Quaternion KritiaEngine::Quaternion::Inverse(const Quaternion& quat) {
	return Quaternion(glm::inverse((glm::quat)quat));
}

Quaternion KritiaEngine::Quaternion::Identity() {
	return Quaternion(0, 0, 0, 1);
}

Matrix4x4 KritiaEngine::Quaternion::ToRotationMatrix(const Quaternion& quat) {
	float entries[4][4] = {1 - 2 * (quat.y * quat.y + quat.z * quat.z), 2 * (quat.x * quat.y + quat.z * quat.w), 2 * (quat.z * quat.x - quat.y * quat.w), 0,
						   2 * (quat.x * quat.y - quat.z * quat.w), 1 - 2 * (quat.z * quat.z + quat.x * quat.x), 2 * (quat.y * quat.z + quat.x * quat.w), 0,
						   2 * (quat.z * quat.x + quat.y * quat.w), 2 * (quat.y * quat.z - quat.x * quat.w), 1 - 2 * (quat.x * quat.x + quat.y * quat.y), 0,
						   0,                                       0,                                           0,                                       1 };
	return Matrix4x4(entries);
}

Quaternion KritiaEngine::Quaternion::FromEuler(float x, float y, float z) {
	Quaternion quatx = Quaternion(Mathf::Sin(x / 2), 0, 0, Mathf::Cos(x / 2));
	Quaternion quaty = Quaternion(0, Mathf::Sin(y / 2), 0, Mathf::Cos(y / 2));
	Quaternion quatz = Quaternion(0, 0, Mathf::Sin(z / 2), Mathf::Cos(z / 2));
	return quatz * quaty * quatx;
}

Quaternion KritiaEngine::Quaternion::FromTwoVectors(const Vector3& vec1, const Vector3& vec2) {
	if (Vector3::Dot(vec1, vec2) == 1 || Vector3::Dot(vec1, vec2) == -1) {
		return Quaternion::Identity();
	} else {
		Vector3 vec3 = Vector3::Cross(vec1, vec2);
		Quaternion quat = Quaternion();
		quat.x = vec3.x;
		quat.y = vec3.y;
		quat.z = vec3.z;
		quat.w = Mathf::Sqrt((Vector3::Magnitude(vec1) * Vector3::Magnitude(vec1)) * (Vector3::Magnitude(vec2) * Vector3::Magnitude(vec2))) + Vector3::Dot(vec1, vec2);
		return Normalize(quat);
	}
}

Quaternion KritiaEngine::Quaternion::FromRoationMatrix(const Matrix4x4& mat) {
	return glm::quat((glm::mat4)mat);
}

Quaternion KritiaEngine::Quaternion::operator+(const Quaternion& quat) {
	return glm::quat(*this) + glm::quat(quat);
}

Quaternion KritiaEngine::Quaternion::operator*(float a) {
	return a * glm::quat(*this);
}

Quaternion KritiaEngine::Quaternion::operator*(const Quaternion& quat) {
	Quaternion res;
	float tempx = x, tempy = y, tempz = z, tempw = w;
	res.x = tempw * quat.x + tempx * quat.w + tempy * quat.z - tempz * quat.y;
	res.y = tempw * quat.y + tempy * quat.w + tempz * quat.x - tempx * quat.z;
	res.z = tempw * quat.z + tempz * quat.w + tempx * quat.y - tempy * quat.x;
	res.w = tempw * quat.w - tempx * quat.x - tempy * quat.y - tempz * quat.z;
	return res;
}

void KritiaEngine::Quaternion::operator+=(const Quaternion& quat) {
	*this = *this + quat;
}

void KritiaEngine::Quaternion::operator*=(const Quaternion& quat) {
	*this = *this * quat;
}

KritiaEngine::Quaternion::operator glm::quat() const {
	return glm::quat(w, x, y, z);
}

KritiaEngine::Quaternion::operator std::vector<float>() const {
	return std::vector<float>({ x,y,z,w });
}

bool KritiaEngine::Quaternion::operator==(const Quaternion& quat) {
	return abs(x - quat.x) < epsilon && abs(y - quat.y) < epsilon && abs(z - quat.z) < epsilon && abs(w - quat.w) < epsilon;
}

bool KritiaEngine::Quaternion::operator!=(const Quaternion& quat) {
	return !(*this == quat);
}

std::ostringstream& KritiaEngine::operator<<(std::ostringstream& cout, const Quaternion& quat) {
	cout << "(" << quat.x << ", " << quat.y << ", " << quat.z << ", " << quat.w << ")";
	return cout;
}

Quaternion KritiaEngine::operator*(float a, const Quaternion& quat) {
	return a * glm::quat(quat);
}


