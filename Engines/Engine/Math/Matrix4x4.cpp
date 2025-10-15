#include "Matrix4x4.h"
#include "Vector3.h"
#include "Quaternion.h"
#include <cmath>
#include <cassert>

using namespace MLEngine::Math;

Matrix4x4 Matrix4x4::Identity() {

	Matrix4x4 identity;

	for (int y = 0; y < 4; y++) {

		for (int x = 0; x < 4; x++) {

			//yとxの値が等しい場合に1を入れる
			if (y == x) {

				identity.m[y][x] = 1.0f;

			}
			else {

				identity.m[y][x] = 0.0f;

			}

		}
	}

	return identity;

}

Vector3 Matrix4x4::GetXAxis() { return Vector3(m[0][0], m[0][1], m[0][2]); }

Vector3 Matrix4x4::GetYAxis() { return Vector3(m[1][0], m[1][1], m[1][2]); }

Vector3 Matrix4x4::GetZAxis() { return Vector3(m[2][0], m[2][1], m[2][2]); }

Vector3 Matrix4x4::GetScale() { return Vector3(Length(GetXAxis()), Length(GetYAxis()), Length(GetZAxis())); }

Matrix4x4 Matrix4x4::GetRotateMatrix() {

	Matrix4x4 rotateMatrix{};

	Vector3 xAxis = Normalize(GetXAxis()), yAxis = Normalize(GetYAxis()), zAxis = Normalize(GetZAxis());

	rotateMatrix.m[0][0] = xAxis.x;
	rotateMatrix.m[0][1] = xAxis.y;
	rotateMatrix.m[0][2] = xAxis.z;
	rotateMatrix.m[1][0] = yAxis.x;
	rotateMatrix.m[1][1] = yAxis.y;
	rotateMatrix.m[1][2] = yAxis.z;
	rotateMatrix.m[2][0] = zAxis.x;
	rotateMatrix.m[2][1] = zAxis.y;
	rotateMatrix.m[2][2] = zAxis.z;
	rotateMatrix.m[3][3] = 1.0f;

	return rotateMatrix;

}

Vector3 Matrix4x4::GetRotate() {

	/*Matrix4x4 rotateMatrix = GetRotateMatrix();*/

	Vector3 xAxis = Normalize(GetXAxis()), yAxis = Normalize(GetYAxis()), zAxis = Normalize(GetZAxis());

	Vector3 xIdentity = { 1.0f,0.0f,0.0f }, yIdentity = { 0.0f,1.0f,0.0f }, zIdentity = { 0.0f,0.0f,1.0f };

	Vector3 rotate{};

	if (yAxis.x != 0.0f) {
		rotate.x = std::atan2(yAxis.x, yAxis.y);
	}
	else {
		rotate.x = std::atan2(yAxis.z, yAxis.y);
	}

	if (zAxis.y != 0.0f) {
		rotate.y = std::atan2(zAxis.y, zAxis.z);
	}
	else {
		rotate.y = std::atan2(zAxis.x, zAxis.z);
	}

	if (xAxis.z != 0.0f) {
		rotate.z = std::atan2(xAxis.z, xAxis.x);
	}
	else {
		rotate.z = std::atan2(xAxis.y, xAxis.x);
	}

	return rotate;

}

Vector3 Matrix4x4::GetTranslate() { return Vector3(m[3][0], m[3][1], m[3][2]); }

//行列の加法
Matrix4x4 MLEngine::Math::Add(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 m = {};

	for (int y = 0; y < 4; y++) {

		for (int x = 0; x < 4; x++) {

			m.m[y][x] = m1.m[y][x] + m2.m[y][x];

		}

	}

	return m;

}

//行列の減法
Matrix4x4 MLEngine::Math::Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 m = {};

	for (int y = 0; y < 4; y++) {

		for (int x = 0; x < 4; x++) {

			m.m[y][x] = m1.m[y][x] - m2.m[y][x];
		}
	}

	return m;

}

//行列の積
Matrix4x4 MLEngine::Math::Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 m = {};

	for (int y = 0; y < 4; y++) {

		for (int x = 0; x < 4; x++) {

			m.m[y][x] = m1.m[y][0] * m2.m[0][x] + m1.m[y][1] * m2.m[1][x] +
				m1.m[y][2] * m2.m[2][x] + m1.m[y][3] * m2.m[3][x];

		}

	}

	return m;

}

//逆行列
Matrix4x4 MLEngine::Math::Inverse(const Matrix4x4& m) {

	//分母
	float denominator = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
		m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -
		m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
		m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -
		m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +
		m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +
		m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
		m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -
		m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
		m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -
		m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +
		m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	Matrix4x4 mResult = {};

	//分母0では割れない
	if (denominator == 0.0f) {
		return mResult;
	}
	

	mResult.m[0][0] = (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[1][3] * m.m[2][1] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]) /
		denominator;
	mResult.m[0][1] = (m.m[0][1] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][1] +
		m.m[0][3] * m.m[2][1] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][1] -
		m.m[0][2] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][2]) /
		-denominator;
	mResult.m[0][2] = (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]) /
		denominator;
	mResult.m[0][3] = (m.m[0][1] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][2]) /
		-denominator;

	mResult.m[1][0] = (m.m[1][0] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][0] +
		m.m[1][3] * m.m[2][0] * m.m[3][2] - m.m[1][3] * m.m[2][2] * m.m[3][0] -
		m.m[1][2] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][2]) /
		-denominator;
	mResult.m[1][1] = (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] +
		m.m[0][3] * m.m[2][0] * m.m[3][2] - m.m[0][3] * m.m[2][2] * m.m[3][0] -
		m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]) /
		denominator;
	mResult.m[1][2] = (m.m[0][0] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][0] +
		m.m[0][3] * m.m[1][0] * m.m[3][2] - m.m[0][3] * m.m[1][2] * m.m[3][0] -
		m.m[0][2] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][2]) /
		-denominator;
	mResult.m[1][3] = (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] +
		m.m[0][3] * m.m[1][0] * m.m[2][2] - m.m[0][3] * m.m[1][2] * m.m[2][0] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]) /
		denominator;

	mResult.m[2][0] = (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[1][3] * m.m[2][0] * m.m[3][1] - m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]) /
		denominator;
	mResult.m[2][1] = (m.m[0][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][0] +
		m.m[0][3] * m.m[2][0] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][0] -
		m.m[0][1] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][1]) /
		-denominator;
	mResult.m[2][2] = (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] +
		m.m[0][3] * m.m[1][0] * m.m[3][1] - m.m[0][3] * m.m[1][1] * m.m[3][0] -
		m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]) /
		denominator;
	mResult.m[2][3] = (m.m[0][0] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][0] +
		m.m[0][3] * m.m[1][0] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][0] -
		m.m[0][1] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][1]) /
		-denominator;

	mResult.m[3][0] = (m.m[1][0] * m.m[2][1] * m.m[3][2] + m.m[1][1] * m.m[2][2] * m.m[3][0] +
		m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][0] -
		m.m[1][1] * m.m[2][0] * m.m[3][2] - m.m[1][0] * m.m[2][2] * m.m[3][1]) /
		-denominator;
	mResult.m[3][1] = (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] +
		m.m[0][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[2][1] * m.m[3][0] -
		m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]) /
		denominator;
	mResult.m[3][2] = (m.m[0][0] * m.m[1][1] * m.m[3][2] + m.m[0][1] * m.m[1][2] * m.m[3][0] +
		m.m[0][2] * m.m[1][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][0] -
		m.m[0][1] * m.m[1][0] * m.m[3][2] - m.m[0][0] * m.m[1][2] * m.m[3][1]) /
		-denominator;
	mResult.m[3][3] = (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] -
		m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]) /
		denominator;

	return mResult;

}

//転置行列
Matrix4x4 MLEngine::Math::Transpose(const Matrix4x4& m) {

	Matrix4x4 mResult;

	for (int y = 0; y < 4; y++) {

		for (int x = 0; x < 4; x++) {

			mResult.m[y][x] = m.m[x][y];
		}
	}

	return mResult;

}

//単位行列の作成
Matrix4x4 MLEngine::Math::MakeIdentity4x4() {

	Matrix4x4 m = {};

	for (int y = 0; y < 4; y++) {

		for (int x = 0; x < 4; x++) {

			//yとxの値が等しい場合に1を入れる
			if (y == x) {

				m.m[y][x] = 1;

			}
			else {

				m.m[y][x] = 0;

			}

		}
	}

	return m;

}

//平行移動行列
Matrix4x4 MLEngine::Math::MakeTranslateMatrix(const Vector3& translate) {

	Matrix4x4 m = {};

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {

			if (y == x) {
				m.m[y][x] = 1.0f;
			}
			else {
				m.m[y][x] = 0.0f;
			}

		}
	}

	m.m[3][0] = translate.x;
	m.m[3][1] = translate.y;
	m.m[3][2] = translate.z;

	return m;

}

//拡大縮小行列
Matrix4x4 MLEngine::Math::MakeScaleMatrix(const Vector3& scale) {

	Matrix4x4 m = {};

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {

			if (y == x) {
				m.m[y][x] = 1.0f;
			}
			else {
				m.m[y][x] = 0.0f;
			}

		}
	}

	m.m[0][0] = scale.x;
	m.m[1][1] = scale.y;
	m.m[2][2] = scale.z;

	return m;

}

//X軸回転行列
Matrix4x4 MLEngine::Math::MakeRotateXMatrix(float radian) {

	Matrix4x4 m{};

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (y == x) {
				m.m[y][x] = 1.0f;
			}
			else {
				m.m[y][x] = 0.0f;
			}
		}
	}

	m.m[1][1] = std::cos(radian);
	m.m[1][2] = std::sin(radian);
	m.m[2][1] = -std::sin(radian);
	m.m[2][2] = std::cos(radian);

	return m;

}

//Y軸回転行列
Matrix4x4 MLEngine::Math::MakeRotateYMatrix(float radian) {

	Matrix4x4 m{};

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (y == x) {
				m.m[y][x] = 1.0f;
			}
			else {
				m.m[y][x] = 0.0f;
			}
		}
	}

	m.m[0][0] = std::cos(radian);
	m.m[0][2] = -std::sin(radian);
	m.m[2][0] = std::sin(radian);
	m.m[2][2] = std::cos(radian);

	return m;

}

//Z軸回転行列
Matrix4x4 MLEngine::Math::MakeRotateZMatrix(float radian) {

	Matrix4x4 m{};

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			if (y == x) {
				m.m[y][x] = 1.0f;
			}
			else {
				m.m[y][x] = 0.0f;
			}
		}
	}

	m.m[0][0] = std::cos(radian);
	m.m[0][1] = std::sin(radian);
	m.m[1][0] = -std::sin(radian);
	m.m[1][1] = std::cos(radian);

	return m;

}

Matrix4x4 MLEngine::Math::MakeRotateMatrix(const Vector3& rotation) {

	Matrix4x4 m{};

	m = MLEngine::Math::Multiply(MLEngine::Math::MakeRotateXMatrix(rotation.x),
		MLEngine::Math::Multiply(MLEngine::Math::MakeRotateYMatrix(rotation.y), MLEngine::Math::MakeRotateZMatrix(rotation.z)));

	return m;

}

//3次元アフィン変換行列
Matrix4x4 MLEngine::Math::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	Matrix4x4 scaleMatrix = MLEngine::Math::MakeScaleMatrix(scale);
	Matrix4x4 rotateMatrix = MLEngine::Math::MakeRotateMatrix(rotate);
	Matrix4x4 translateMatrix = MLEngine::Math::MakeTranslateMatrix(translate);

	Matrix4x4 m{};

	m = MLEngine::Math::Multiply(MLEngine::Math::Multiply(scaleMatrix, rotateMatrix), translateMatrix);

	return m;

}

Matrix4x4 MLEngine::Math::MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {

	Matrix4x4 scaleMatrix = MLEngine::Math::MakeScaleMatrix(scale);
	Matrix4x4 rotateMatrix = MLEngine::Math::MakeRotateMatrix(rotate);
	Matrix4x4 translateMatrix = MLEngine::Math::MakeTranslateMatrix(translate);

	Quaternion qr = ConvertFromRotateMatrix(rotateMatrix);

	qr = qr;

	Matrix4x4 m{};

	m = MLEngine::Math::Multiply(MLEngine::Math::Multiply(scaleMatrix, rotateMatrix), translateMatrix);

	return m;

}

//透視投影行列
Matrix4x4 MLEngine::Math::MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {

	Matrix4x4 m{};

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			m.m[y][x] = 0.0f;
		}
	}

	m.m[0][0] = 1.0f / aspectRatio * 1.0f / tanf(fovY / 2.0f);
	m.m[1][1] = 1.0f / tanf(fovY / 2.0f);
	m.m[2][2] = farClip / (farClip - nearClip);
	m.m[2][3] = 1.0f;
	m.m[3][2] = (-nearClip * farClip) / (farClip - nearClip);

	return m;

}

//正射影行列
Matrix4x4 MLEngine::Math::MakeOrthographicMatrix(float left, float top, float right, float bottom,
	float nearClip, float farClip) {

	Matrix4x4 m{};

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			m.m[y][x] = 0.0f;
		}
	}

	m.m[0][0] = 2.0f / (right - left);
	m.m[1][1] = 2.0f / (top - bottom);
	m.m[2][2] = 1.0f / (farClip - nearClip);
	m.m[3][0] = (left + right) / (left - right);
	m.m[3][1] = (top + bottom) / (bottom - top);
	m.m[3][2] = (nearClip) / (nearClip - farClip);
	m.m[3][3] = 1.0f;

	return m;

}

//ビューポート変換行列
Matrix4x4 MLEngine::Math::MakeViewportMatrix(float left, float top, float width, float height,
	float minDepth, float maxDepth) {

	Matrix4x4 m{};

	for (int y = 0; y < 4; y++) {
		for (int x = 0; x < 4; x++) {
			m.m[y][x] = 0.0f;
		}
	}

	m.m[0][0] = width / 2.0f;
	m.m[1][1] = -height / 2.0f;
	m.m[2][2] = maxDepth - minDepth;
	m.m[3][0] = left + (width / 2.0f);
	m.m[3][1] = top + (height / 2.0f);
	m.m[3][2] = minDepth;
	m.m[3][3] = 1.0f;

	return m;

}

Matrix4x4 MLEngine::Math::MakeRotateAxisAngle(const Vector3& axis, float angle) {

	Matrix4x4 result{};

	result.m[0][0] = powf(axis.x, 2) * (1.0f - cosf(angle)) + cosf(angle);
	result.m[0][1] = axis.x * axis.y * (1.0f - cosf(angle)) + axis.z * sinf(angle);
	result.m[0][2] = axis.x * axis.z * (1.0f - cosf(angle)) - axis.y * sinf(angle);
	result.m[0][3] = 0.0f;

	result.m[1][0] = axis.x * axis.y * (1.0f - cosf(angle)) - axis.z * sinf(angle);
	result.m[1][1] = powf(axis.y, 2) * (1.0f - cosf(angle)) + cosf(angle);
	result.m[1][2] = axis.y * axis.z * (1.0f - cosf(angle)) + axis.x * sinf(angle);
	result.m[1][3] = 0.0f;

	result.m[2][0] = axis.x * axis.z * (1.0f - cosf(angle)) + axis.y * sinf(angle);
	result.m[2][1] = axis.y * axis.z * (1.0f - cosf(angle)) - axis.x * sinf(angle);
	result.m[2][2] = powf(axis.z, 2) * (1.0f - cosf(angle)) + cosf(angle);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;

}

//ある方向をある方向へ向ける回転行列
Matrix4x4 MLEngine::Math::DirectionToDirection(const Vector3& from, const Vector3& to) {

	//回転軸
	Vector3 n{};

	//cosを求める
	float cosTheta = Dot(from, to);

	//逆向きのベクトルだった場合、垂直なベクトルを一つ選ぶ
	if (cosTheta <= -1.0f) {

		if (from.x != 0.0f || from.y != 0.0f) {

			n = { from.y, -from.x,0.0f };
			n = Normalize(n);
		}
		else if (from.x != 0.0f || from.z != 0.0f) {

			n = { 0.0f, -from.z, from.x };
			n = Normalize(n);

		}

	}
	else {
		n = Normalize(Cross(from, to));
	}

	//sinを求める
	float sinTheta = Length(Cross(from, to));

	Matrix4x4 result{};

	//任意軸の回転行列を求めた値から生成
	result.m[0][0] = powf(n.x, 2) * (1.0f - cosTheta) + cosTheta;
	result.m[0][1] = n.x * n.y * (1.0f - cosTheta) + n.z * sinTheta;
	result.m[0][2] = n.x * n.z * (1.0f - cosTheta) - n.y * sinTheta;
	result.m[0][3] = 0.0f;

	result.m[1][0] = n.x * n.y * (1.0f - cosTheta) - n.z * sinTheta;
	result.m[1][1] = powf(n.y, 2) * (1.0f - cosTheta) + cosTheta;
	result.m[1][2] = n.y * n.z * (1.0f - cosTheta) + n.x * sinTheta;
	result.m[1][3] = 0.0f;

	result.m[2][0] = n.x * n.z * (1.0f - cosTheta) + n.y * sinTheta;
	result.m[2][1] = n.y * n.z * (1.0f - cosTheta) - n.x * sinTheta;
	result.m[2][2] = powf(n.z, 2) * (1.0f - cosTheta) + cosTheta;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;

}

Matrix4x4 MLEngine::Math::MakeRotateMatrix(const Quaternion& quaternion) {

	Matrix4x4 result{};

	result.m[0][0] = quaternion.w * quaternion.w + quaternion.x * quaternion.x -
		quaternion.y * quaternion.y - quaternion.z * quaternion.z;
	result.m[0][1] = 2.0f * (quaternion.x * quaternion.y + quaternion.w * quaternion.z);
	result.m[0][2] = 2.0f * (quaternion.x * quaternion.z - quaternion.w * quaternion.y);
	result.m[0][3] = 0.0f;


	result.m[1][0] = 2.0f * (quaternion.x * quaternion.y - quaternion.w * quaternion.z);
	result.m[1][1] = quaternion.w * quaternion.w - quaternion.x * quaternion.x +
		quaternion.y * quaternion.y - quaternion.z * quaternion.z;
	result.m[1][2] = 2.0f * (quaternion.y * quaternion.z + quaternion.w * quaternion.x);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 2.0f * (quaternion.x * quaternion.z + quaternion.w * quaternion.y);
	result.m[2][1] = 2.0f * (quaternion.y * quaternion.z - quaternion.w * quaternion.x);
	result.m[2][2] = quaternion.w * quaternion.w - quaternion.x * quaternion.x -
		quaternion.y * quaternion.y + quaternion.z * quaternion.z;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;

}

// 二項演算子
Matrix4x4 MLEngine::Math::operator+(const Matrix4x4& m1, const Matrix4x4& m2) { return MLEngine::Math::Add(m1, m2); }
Matrix4x4 MLEngine::Math::operator-(const Matrix4x4& m1, const Matrix4x4& m2) { return MLEngine::Math::Subtract(m1, m2); }
Matrix4x4 MLEngine::Math::operator*(const Matrix4x4& m1, const Matrix4x4& m2) { return MLEngine::Math::Multiply(m1, m2); }

