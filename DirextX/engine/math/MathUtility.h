#pragma once
#include <random>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix2x2.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
namespace MathUtility
{
	//
	const float kPi = 3.14159265358979323846f;

	// Vector2

	Vector2 operator+(const Vector2& v1, const Vector2& v2);

	Vector2 operator-(const Vector2& v1, const Vector2& v2);

	Vector2 operator*(const Vector2& v1, const float& f);

	Vector2 operator*(const float& f, const Vector2& v1);

	Vector2 operator/(const Vector2& v1, const float& f);

	Vector2& operator+=(Vector2& v1, const Vector2& v2);

	Vector2& operator-=(Vector2& v1, const Vector2& v2);

	Vector2& operator*=(Vector2& v1, const float& f);

	Vector2& operator/=(Vector2& v1, const float& f);

	// Vector3
	Vector3 operator+(const Vector3& v1, const Vector3& v2);

	Vector3 operator-(const Vector3& v1, const Vector3& v2);

	Vector3 operator*(const Vector3& v1, const float& f);

	Vector3 operator*(const float& f, const Vector3& v1);

	Vector3 operator/(const Vector3& v1, const float& f);

	Vector3& operator+=(Vector3& v1, const Vector3& v2);

	Vector3& operator-=(Vector3& v1, const Vector3& v2);

	Vector3& operator*=(Vector3& v1, const float& f);

	float Length(Vector3 a);

	float Lerp(float a, float b, float t);

	Vector3 Lerp(const Vector3& a, const Vector3& b, float t);

	Vector3 Normalize(Vector3 a);

	Vector3 TransformNormal(const Vector3& a, const Matrix4x4& m1);

	// Vector4
	Vector4 operator-(const Vector4& a);

	Vector4 operator+(const Vector4& v1, const Vector4& v2);

	Vector4 operator-(const Vector4& v1, const Vector4& v2);

	Vector4 operator*(const Vector4& v1, const float& f);

	Vector4 operator*(const float& f, const Vector4& v1);

	Vector4 operator*(const Vector4& v1, const Vector4& v2);

	Vector4 operator/(const Vector4& v1, const float& f);

	Vector4& operator+=(Vector4& v1, const Vector4& v2);

	Vector4& operator-=(Vector4& v1, const Vector4& v2);

	Vector4& operator*=(Vector4& v1, const float& f);
	float Dot(const Vector4& q1, const Vector4& q2);

	Vector4 Normalize(Vector4 a);

	Vector4 Lerp(const Vector4& a, const Vector4& b, float t);

	Vector4 SLerp(const Vector4& a, const Vector4& b, float t);


	// Matrix4x4
	Matrix4x4 MakeIdentity4x4();

	Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);

	Matrix4x4 MakeRotateXMatrix(float radian);

	Matrix4x4 MakeRotateYMatrix(float radian);

	Matrix4x4 MakeRotateZMatrix(float radian);

	Matrix4x4 MakeRotateMatrix(const Vector4& quaternion);

	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector4& rotate, const Vector3& translate);

	Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	float Matrix2x2Determinant(const Matrix2x2& m1);

	float Matrix3x3Determinant(const Matrix3x3& m1);

	float Matrix4x4Determinant(const Matrix4x4& m1);

	Matrix4x4 Transpose(const Matrix4x4& m1);

	Matrix4x4 Inverse(const Matrix4x4& m1);

	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

	// random

	float RandomFloat(float min, float max, std::mt19937& random);


	// Quaternion(Vector4)

	float Length(Vector2 a);
};

