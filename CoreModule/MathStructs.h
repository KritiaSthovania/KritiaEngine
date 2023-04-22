#pragma once
#include <pch.h>
#include <sstream>

namespace KritiaEngine {
	struct Vector2 {
		friend std::ostringstream& operator<<(std::ostringstream& cout, const Vector2& vec);
	public:
		Vector2(float x, float y);
		Vector2(const glm::vec2& vec);
		Vector2();
		static float Cross(const Vector2& vec1, const Vector2& vec2);
		float x, y;
		static Vector2 Zero();
		friend Vector2 operator* (float a, const Vector2& vec);
		Vector2 operator-(const Vector2& vec) const;
		Vector2 operator+(const Vector2& vec) const;
		Vector2 operator/(float a) const;
		operator glm::vec2() const;
	};

	struct Vector3 {
		friend std::ostringstream& operator<<(std::ostringstream& cout, const Vector3& vec);
	public:
		Vector3(float x, float y, float z);
		Vector3(const glm::vec3 &vec);
		Vector3();
		static Vector3 Zero();
		static Vector3 Normalize(const Vector3 &vec);
		static Vector3 Cross(const Vector3 &vec1, const Vector3 &vec2);
		static Vector3 Project(const Vector3& point, const Vector3& axis);
		static float Dot(const Vector3& vec1, const Vector3& vec2);
		static float Magnitude(const Vector3& vec);
		static glm::vec3 ToGlmVec3(const Vector3 &vec);
		float x;
		float y;
		float z;
		//operators
		friend Vector3 operator-(const Vector3& self);
		friend Vector3 operator*(float a, const Vector3& vec);
		friend Vector3 operator+(const Vector3& vec1, const Vector3& vec2);
		friend Vector3 operator-(const Vector3& vec1, const Vector3& vec2);
		Vector3 operator+ (const Vector3 &vec);
		Vector3 operator- (const Vector3 &vec);
		Vector3 operator* (float a);
		// component-wise multiply
		Vector3 operator* (const Vector3& vec);
		Vector3 operator/ (float a);
		void operator+= (const Vector3 &vec);
		void operator-= (const Vector3 &vec);
		void operator*= (float a);
		void operator/= (float a);
		bool operator == (const Vector3& vec);
		bool operator!= (const Vector3& vec);
		operator glm::vec3() const;
		operator std::vector<float>() const;
	private:

	};
	struct Vector4 {
		friend std::ostringstream& operator<<(std::ostringstream& cout, const Vector4& vec);
	public:
		Vector4(float x, float y, float z, float w);
		Vector4();
		Vector4(glm::vec4 vec);
		Vector4(const Vector3& vec, float w);
		static Vector4 Zero();
		static glm::vec4 ToGlmVec4(const Vector4& vec4);
		float x;
		float y;
		float z;
		float w;
		// operators
		friend Vector4 operator*(float a, const Vector4& vec);
		Vector4 operator+ (const Vector4& vec) const;
		Vector4 operator- (const Vector4& vec) const;
		Vector4 operator* (float a) const;
		Vector4 operator/ (float a) const;
		void operator+= (const Vector4& vec);
		void operator-= (const Vector4& vec);
		void operator*= (float a);
		void operator/= (float a);
		bool operator== (const Vector4& vec);
		bool operator!= (const Vector4& vec);
		operator glm::vec4() const;
		operator Vector3() const;
		operator std::vector<float>() const;
	private:

	};

	struct Matrix3x3 {
	public:
		Matrix3x3();
		Matrix3x3(const Vector3& column0, const Vector3& column1, const Vector3& column2);
		Matrix3x3(const float entries[3][3]);
		Matrix3x3(const glm::mat3& mat);
		Matrix3x3 Inverse();
		Matrix3x3 Transpose();
		static Matrix3x3 Identity();
		static glm::mat3 ToGlmMat3(const Matrix3x3 &mat);
		// operator
		operator glm::mat3() const;
		Vector3 operator* (const Vector3& vec3) const;
		Matrix3x3 operator* (const Matrix3x3& mat) const;
	private:
		float entries[3][3];
	};

	/// <summary>
    /// 4x4¡–æÿ’Û
    /// </summary>
	struct Matrix4x4 {
	public:
		Matrix4x4();
		Matrix4x4(const Vector4& column0, const Vector4& column1, const Vector4& column2, const Vector4& column3);
		Matrix4x4(const float entries[4][4]);
		Matrix4x4(const glm::mat4& mat);
		Matrix4x4(const Matrix3x3& mat);
		/// <summary>
		/// Convention for Identity Matrix
		/// </summary>
		static Matrix4x4 Identity();
		/// <summary>
		/// Get the perspective matrix
		/// </summary>
		/// <param name="fovy"> field of view on y direction in degree</param>
		/// <param name="aspect">aspect ratio</param>
		/// <param name="">near plane distance</param>
		/// <param name="">far plane distance</param>
		/// <returns></returns>
		static Matrix4x4 Perspective(float fovy, float aspect, float near, float far);

		static Matrix4x4 Ortho(float left, float right, float bottom, float top, float near, float far);

		/// <summary>
		/// Get the view matrix
		/// </summary>
		/// <param name="eye">Position of the eye/camera</param>
		/// <param name="center">looking position</param>
		/// <param name="up">up vector</param>
		/// <returns></returns>
		static Matrix4x4 LookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

		static glm::mat4 ToGlmMat4(const Matrix4x4& mat4);

		/// <summary>
		/// Get a column by index
		/// </summary>
		/// <param name="index">0 to 3</param>
		Vector4 GetColumn(int index) const;
		/// <summary>
		/// Get a column by index
		/// </summary>
		/// <param name="index">0 to 3</param>
		Vector4 GetRow(int index) const;
		/// <summary>
		/// Set a column by index
		/// </summary>
		/// <param name="index">0 to 3</param>
		void SetColumn(int index, const Vector4& column);
		/// <summary>
		/// Set a column by index
		/// </summary>
		/// <param name="index">0 to 3</param>
		void SetRow(int index, const Vector4& row);

		/// <summary>
		/// Get an entry by row and column
		/// </summary>
		float GetEntry(int row, int column) const;
		/// <summary>
		/// Set an entry by row and column
		/// </summary>
		void SetEntry(int row, int column, int newEntry);

		float* GetPtr();

		// operators
		Matrix4x4 operator* (const Matrix4x4& mat) const;
		Vector4 operator* (const Vector4& vec4) const;
		void operator *= (const Matrix4x4& mat);
		operator glm::mat4() const;
		/// <summary>
		/// Get the first 3x3 entries of the matrix.
		/// </summary>
		operator Matrix3x3() const;
	private:
		float entries[4][4];
	};


	struct Quaternion {
		friend std::ostringstream& operator<<(std::ostringstream& cout, const Quaternion& quat);
	public:
		/// <summary>
		/// The identity. Better use Quaternion::Identity()
		/// </summary>
		Quaternion();
		Quaternion(float x, float y, float z, float w);
		Quaternion(const glm::quat& quat);
		/// <summary>
		/// Same as FromRotationMatrix
		/// </summary>
		Quaternion(const Matrix4x4& mat);
		/// <summary>
		/// Same as FromTwoVectors
		/// </summary>
		Quaternion(const Vector3& vec1, const Vector3& vec2);
		float w, x, y, z;
		static Vector3 ToEuler(const Quaternion& quat);
		static Quaternion Normalize(const Quaternion& quat);
		static Quaternion Conjugate(const Quaternion& quat);
		static Quaternion Inverse(const Quaternion& quat);
		static Quaternion Identity();
		/// <summary>
		/// Convert a quaternion to a 4x4 rotation matrix
		/// </summary>
		static Matrix4x4 ToRotationMatrix(const Quaternion& quat);
		/// <summary>
		/// Returns a rotation that rotates z degrees around the z axis, x degrees around the x axis, and y degrees around the y axis; applied in that order.
		/// </summary>
		static Quaternion FromEuler(float x, float y, float z);
		/// <summary>
		/// The quaternion of the rotation from vec1 to vec2
		/// </summary>
		static Quaternion FromTwoVectors(const Vector3& vec1, const Vector3& vec2);
		static Quaternion FromRoationMatrix(const Matrix4x4& mat);

		friend Quaternion operator* (float a, const Quaternion& quat);
		Quaternion operator+ (const Quaternion& quat);
		Quaternion operator* (float a);
		Quaternion operator* (const Quaternion& quat);
		void operator+= (const Quaternion& quat);
		void operator*= (const Quaternion& quat);
		bool operator== (const Quaternion& quat);
		bool operator!= (const Quaternion& quat);
		operator glm::quat() const;
		operator std::vector<float>() const;
	};
}

