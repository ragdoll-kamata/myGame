#include "MathUtility.h"
#include <cmath>
namespace MathUtility
{
	Vector2 operator+(const Vector2& v1, const Vector2& v2) {
		return Vector2(v1.x + v2.x, v1.y + v2.y);
	}
	Vector2 operator-(const Vector2& v1, const Vector2& v2) {
		return Vector2(v1.x - v2.x, v1.y - v2.y);
	}
	Vector2 operator*(const Vector2& v1, const float& f) {
		return Vector2(v1.x * f, v1.y * f);
	}
	Vector2 operator*(const float& f, const Vector2& v1) {
		return v1 * f;
	}
	Vector2 operator/(const Vector2& v1, const float& f) {
		return Vector2(v1.x / f, v1.y / f);
	}
	Vector2& operator+=(Vector2& v1, const Vector2& v2) {
		return v1 = v1 + v2;
	}
	Vector2& operator-=(Vector2& v1, const Vector2& v2) {
		return v1 = v1 - v2;
	}
	Vector2& operator*=(Vector2& v1, const float& f) {
		return v1 = v1 * f;
	}
	Vector2& operator/=(Vector2& v1, const float& f) {
		return v1 = v1 / f;
	}
	Vector3 operator+(const Vector3& v1, const Vector3& v2) {
		return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}
	Vector3 operator-(const Vector3& v1, const Vector3& v2) {
		return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}
	Vector3 operator*(const Vector3& v1, const float& f) {
		return Vector3(v1.x * f, v1.y * f, v1.y * f);
	}
	Vector3 operator*(const float& f, const Vector3& v1) {
		return v1 * f;
	}
	Vector3 operator/(const Vector3& v1, const float& f) {
		return Vector3(v1.x / f, v1.y / f, v1.y / f);
	}
	Vector3& operator+=(Vector3& v1, const Vector3& v2) {
		v1 = v1 + v2;

		return v1;
	}
	Vector3& operator-=(Vector3& v1, const Vector3& v2) {
		v1 = v1 - v2;

		return v1;
	}
	Vector3& operator*=(Vector3& v1, const float& f) {
		v1 = v1 * f;
		return v1;
	}
	float Length(Vector3 a) {
		return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
	}
	float Lerp(float a, float b, float t) {
		return (1.0f - t) * a + t * b;
	}
	Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
		return Vector2(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t));
	}
	Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
		return Vector3(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t), Lerp(a.z, b.z, t));
	}
	Vector3 Normalize(Vector3 a) {
		float len = Length(a);
		if (len == 0.0f) {
			return a;
		}
		return {a.x / len, a.y / len, a.z / len};
	}
	Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {

		Vector3 c{
			v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
			v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
			v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
		};

		return Vector3();
	}

	float Dot(const Vector4& q1, const Vector4& q2) {
		return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
	}

	Vector4 Normalize(Vector4 a) {
		float len = std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
		if (len < 1e-6f) {
			// ゼロ除算防止：単位クォータニオンを返す
			return Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		}
		return a / len;
	}

	Vector4 Lerp(const Vector4& a, const Vector4& b, float t) {
		return Vector4(Lerp(a.x, b.x, t), Lerp(a.y, b.y, t), Lerp(a.z, b.z, t), Lerp(a.w, b.w, t));
	}
	Vector4 SLerp(const Vector4& a, const Vector4& b, float t) {
		Vector4 q1 = Normalize(a);
		Vector4 q2 = Normalize(b);
		float dot = Dot(q1, q2);

		// ショートパスのために q2 を反転
		if (dot < 0.0f) {
			dot = -dot;
			q2 = -q2;
		}

		const float epsilon = 1e-5f;
		if (1.0f - dot < epsilon) {
			// 線形補間で十分
			return Normalize((1.0f - t) * q1 + t * q2);
		}

		float theta = std::acos(dot);
		float s1 = std::sinf((1.0f - t) * theta) / std::sinf(theta);
		float s2 = std::sinf(t * theta) / std::sinf(theta);

		return s1 * q1 + s2 * q2;
	}

	Matrix4x4 MakeIdentity4x4() {
		Matrix4x4 m1 = {};
		for (int i = 0; i < 4; i++) {
			m1.m[i][i] = 1.0f;
		}
		return m1;
	}

	Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
		Matrix4x4 m3 = {};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					m3.m[i][j] += m1.m[i][k] * m2.m[k][j];
				}
			}
		}
		return m3;
	}
	Matrix4x4 MakeRotateXMatrix(float radian) {
		return Matrix4x4{
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, std::cos(radian), std::sin(radian), 0.0f,
			0.0f, -std::sin(radian), std::cos(radian), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
	Matrix4x4 MakeRotateYMatrix(float radian) {
		return Matrix4x4{
			std::cos(radian), 0.0f, -std::sin(radian), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			std::sin(radian), 0.0f, std::cos(radian), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
	Matrix4x4 MakeRotateZMatrix(float radian) {
		return Matrix4x4{
			std::cos(radian), std::sin(radian), 0.0f, 0.0f,
			-std::sin(radian), std::cos(radian), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}
	Matrix4x4 MakeRotateMatrix(const Vector4& quaternion) {
		Matrix4x4 a{};
		a.m[0][0] = std::powf(quaternion.w, 2.0f) + std::powf(quaternion.x, 2.0f) - std::powf(quaternion.y, 2.0f) - std::powf(quaternion.z, 2.0f);
		a.m[0][1] = 2.0f * (quaternion.x * quaternion.y + quaternion.w * quaternion.z);
		a.m[0][2] = 2.0f * (quaternion.x * quaternion.z - quaternion.w * quaternion.y);
		a.m[0][3] = 0.0f;

		a.m[1][0] = 2.0f * (quaternion.x * quaternion.y - quaternion.w * quaternion.z);
		a.m[1][1] = std::powf(quaternion.w, 2.0f) - std::powf(quaternion.x, 2.0f) + std::powf(quaternion.y, 2.0f) - std::powf(quaternion.z, 2.0f);
		a.m[1][2] = 2.0f * (quaternion.y * quaternion.z + quaternion.w * quaternion.x);
		a.m[1][3] = 0.0f;

		a.m[2][0] = 2.0f * (quaternion.x * quaternion.z + quaternion.w * quaternion.y);
		a.m[2][1] = 2.0f * (quaternion.y * quaternion.z - quaternion.w * quaternion.x);
		a.m[2][2] = std::powf(quaternion.w, 2.0f) - std::powf(quaternion.x, 2.0f) - std::powf(quaternion.y, 2.0f) + std::powf(quaternion.z, 2.0f);
		a.m[2][3] = 0.0f;

		a.m[3][0] = 0.0f;
		a.m[3][1] = 0.0f;
		a.m[3][2] = 0.0f;
		a.m[3][3] = 1.0f;

		return a;
	}
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
		Matrix4x4 matrixRotate = MakeRotateXMatrix(rotate.x) * MakeRotateYMatrix(rotate.y) * MakeRotateZMatrix(rotate.z);
		Matrix4x4 a = {
			scale.x * matrixRotate.m[0][0], scale.x * matrixRotate.m[0][1], scale.x * matrixRotate.m[0][2], 0.0f,
			scale.y * matrixRotate.m[1][0], scale.y * matrixRotate.m[1][1], scale.y * matrixRotate.m[1][2], 0.0f,
			scale.z * matrixRotate.m[2][0], scale.z * matrixRotate.m[2][1], scale.z * matrixRotate.m[2][2], 0.0f,
			translate.x, translate.y, translate.z, 1.0f
		};
		return a;
	}
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector4& rotate, const Vector3& translate) {
		Matrix4x4 matrixRotate = MakeRotateMatrix(rotate);
		Matrix4x4 a = {
			scale.x * matrixRotate.m[0][0], scale.x * matrixRotate.m[0][1], scale.x * matrixRotate.m[0][2], 0.0f,
			scale.y * matrixRotate.m[1][0], scale.y * matrixRotate.m[1][1], scale.y * matrixRotate.m[1][2], 0.0f,
			scale.z * matrixRotate.m[2][0], scale.z * matrixRotate.m[2][1], scale.z * matrixRotate.m[2][2], 0.0f,
			translate.x, translate.y, translate.z, 1.0f
		};
		return a;
	}
	Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
		Matrix4x4 scaleMatrix = {
			{
				scale.x, 0.0f, 0.0f, 0.0f,
				0.0f, scale.y, 0.0f, 0.0f,
				0.0f, 0.0f, scale.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			}
		};

		return scaleMatrix;
	}
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
		Matrix4x4 translateMatrix = {
			{
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				translate.x, translate.y, translate.z, 1.0f
			}
		};

		return translateMatrix;
	}
	float Matrix2x2Determinant(const Matrix2x2& m1) {
		return m1.m[0][0] * m1.m[1][1] - m1.m[1][0] * m1.m[0][1];
	}
	float Matrix3x3Determinant(const Matrix3x3& m1) {
		float a = 0.0f;
		Matrix2x2 m2 = {};
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					m2.m[j][k] = m1.m[j >= i ? j + 1 : j][k + 1];
				}
			}
			if (i % 2 == 0) {
				a += m1.m[i][0] * Matrix2x2Determinant(m2);
			} else {
				a += -m1.m[i][0] * Matrix2x2Determinant(m2);
			}
		}
		return a;
	}
	float Matrix4x4Determinant(const Matrix4x4& m1) {
		float a = 0.0f;
		Matrix3x3 m2 = {};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					m2.m[j][k] = m1.m[j >= i ? j + 1 : j][k + 1];
				}
			}
			if (i % 2 == 0) {
				a += Matrix3x3Determinant(m2) * m1.m[i][0];
			} else {
				a += Matrix3x3Determinant(m2) * -m1.m[i][0];
			}
		}
		return a;
	}
	Matrix4x4 Transpose(const Matrix4x4& m1) {
		Matrix4x4 m2 = {};
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m2.m[i][j] = m1.m[j][i];
			}
		}
		return m2;
	}
	Matrix4x4 Inverse(const Matrix4x4& m1) {
		float a = Matrix4x4Determinant(m1);
		Matrix3x3 m2 = {};
		Matrix4x4 m3 = {};

		if (a == 0) {
			return m1;
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 3; k++) {
					for (int l = 0; l < 3; l++) {
						m2.m[k][l] = m1.m[k >= j ? k + 1 : k][l >= i ? l + 1 : l];
					}
				}

				if ((i + j) % 2 == 0) {
					m3.m[i][j] = Matrix3x3Determinant(m2) / a;
				} else {
					m3.m[i][j] = -Matrix3x3Determinant(m2) / a;
				}
			}
		}
		return m3;
	}
	Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
		Matrix4x4 m = {};

		m.m[0][0] = (1.0f / tanf(fovY / 2.0f)) / aspectRatio;
		m.m[1][1] = (1.0f / tanf(fovY / 2.0f));
		m.m[2][2] = farClip / (farClip - nearClip);
		m.m[2][3] = 1.0f;
		m.m[3][2] = -nearClip * farClip / (farClip - nearClip);

		return m;
	}
	Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
		Matrix4x4 m = {};

		m.m[0][0] = 2.0f / (right - left);
		m.m[1][1] = 2.0f / (top - bottom);
		m.m[2][2] = 1.0f / (farClip - nearClip);
		m.m[3][3] = 1.0f;
		m.m[3][0] = (left + right) / (left - right);
		m.m[3][1] = (top + bottom) / (bottom - top);
		m.m[3][2] = nearClip / (nearClip - farClip);

		return m;
	}
	float RandomFloat(float min, float max, std::mt19937& random) {
		std::uniform_real_distribution<float> dis(min, max);
		return dis(random);
	}

	Vector4 operator-(const Vector4& a) {
		return Vector4(-a.x, -a.y, -a.z, -a.w);
	}

	Vector4 operator+(const Vector4& v1, const Vector4& v2) {
		return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
	}
	Vector4 operator-(const Vector4& v1, const Vector4& v2) {
		return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
	}

	Vector4 operator*(const Vector4& v1, const float& f) {
		return Vector4(v1.x * f, v1.y * f, v1.z * f, v1.w * f);
	}

	Vector4 operator*(const float& f, const Vector4& v1) {
		return v1 * f;
	}

	Vector4 operator*(const Vector4& v1, const Vector4& v2) {
		return Vector4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
	}

	Vector4 operator/(const Vector4& v1, const float& f) {
		return  Vector4(v1.x / f, v1.y / f, v1.z / f, v1.w / f);
	}

	Vector4& operator+=(Vector4& v1, const Vector4& v2) {
		v1 = v1 + v2;

		return v1;
	}

	Vector4& operator-=(Vector4& v1, const Vector4& v2) {
		v1 = v1 - v2;

		return v1;
	}

	Vector4& operator*=(Vector4& v1, const float& f) {
		v1 = v1 * f;

		return v1;
	}

	float Length(Vector2 a) {
		return sqrtf(a.x * a.x + a.y * a.y);
	}
}