#include "Vector3.h"
#include "Vector2.h"
#include <math.h>
#include "Matrix4x4.h"
#include <cassert>
#include <iostream>
#include <algorithm>
#include <numbers>

using namespace MLEngine::Math;

// 加算
Vector3 MLEngine::Math::Add(const Vector3& v1, const Vector3& v2) {

	Vector3 v{};

	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	v.z = v1.z + v2.z;

	return v;
}

// 減算
Vector3 MLEngine::Math::Subtract(const Vector3& v1, const Vector3& v2) {

	Vector3 v{};

	v.x = v1.x - v2.x;
	v.y = v1.y - v2.y;
	v.z = v1.z - v2.z;

	return v;
}

// スカラー倍
Vector3 MLEngine::Math::Multiply(float scalar, const Vector3& v1) {

	Vector3 v{};

	v.x = scalar * v1.x;
	v.y = scalar * v1.y;
	v.z = scalar * v1.z;

	return v;
}

// 内積
float MLEngine::Math::Dot(const Vector3& v1, const Vector3& v2) {

	float x;

	x = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return x;
}

// 長さ(ノルム)
float MLEngine::Math::Length(const Vector3& v) {

	float length;

	length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	return length;
}

// 正規化
Vector3 MLEngine::Math::Normalize(const Vector3& v) {

	float length = MLEngine::Math::Length(v);

	Vector3 newV = { 0.0f,0.0f,0.0f };

	newV.x = v.x;
	newV.y = v.y;
	newV.z = v.z;

	if (length != 0.0f) {

		newV.x = v.x / length;
		newV.y = v.y / length;
		newV.z = v.z / length;
	}

	return newV;
}

Vector3 MLEngine::Math::CoordTransform(const Vector3& vector, const Matrix4x4& matrix) {

	Vector3 result = {};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] +
		vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] +
		vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] +
		vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] +
		vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	if (w == 0.0f) {
		return Vector3::Zero();
	}

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;

}

Vector3 MLEngine::Math::ScreenToWorld(const Vector2& position, float zValue, const Matrix4x4& matView, const Matrix4x4& matProjection)
{

	zValue = std::clamp(zValue, 0.0f, 1.0f);

	Vector3 result = { position.x, position.y, zValue };

	Matrix4x4 viewPort = MakeViewportMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f);

	result = MLEngine::Math::CoordTransform(result, Inverse(viewPort));
	result = MLEngine::Math::CoordTransform(result, Inverse(matProjection));
	result = MLEngine::Math::CoordTransform(result, Inverse(matView));

	return result;

}

//クロス積
Vector3 MLEngine::Math::Cross(const Vector3& v1, const Vector3& v2) {

	Vector3 v(v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x);

	return v;

}

//ベクトル変換(スケールと回転)
Vector3 MLEngine::Math::TransformNormal(const Vector3& v, const Matrix4x4& m) {

	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};

	return result;

}
//線形補間
Vector3 MLEngine::Math::Lerp(const Vector3& v1, const Vector3& v2, float t) {

	t = std::clamp(t, 0.0f, 1.0f);

	Vector3 p = Vector3(
		v1.x + t * (v2.x - v1.x),
		v1.y + t * (v2.y - v1.y),
		v1.z + t * (v2.z - v1.z)
	);

	return p;

}

//球面線形補間
Vector3 MLEngine::Math::Slerp(const Vector3& v1, const Vector3& v2, float t) {

	t = std::clamp(t, 0.0f, 1.0f);

	float theta = acosf(std::clamp(MLEngine::Math::Dot(v1, v2), 0.0f, 1.0f) / (MLEngine::Math::Length(v1) * MLEngine::Math::Length(v2)));

	float s = (1.0f - t) * MLEngine::Math::Length(v1) + t * MLEngine::Math::Length(v2);

	Vector3 p = Vector3(
		s * (std::sin(1 - t) / std::sin(theta) * v1.x / MLEngine::Math::Length(v1) +
			std::sin(t) / std::sin(theta) * v2.x / MLEngine::Math::Length(v2)),
		s * (std::sin(1 - t) / std::sin(theta) * v1.y / MLEngine::Math::Length(v1) +
			std::sin(t) / std::sin(theta) * v2.y / MLEngine::Math::Length(v2)),
		s * (std::sin(1 - t) / std::sin(theta) * v1.z / MLEngine::Math::Length(v1) +
			std::sin(t) / std::sin(theta) * v2.z / MLEngine::Math::Length(v2)));

	return p;

}

Vector3 MLEngine::Math::CatmullRomPoint(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {

	Vector3 p{};

	p.x = ((-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * powf(t, 3) +
		(2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x) * powf(t, 2) +
		(-p0.x + p2.x) * t + (2 * p1.x)) / 2.0f;

	p.y = ((-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * powf(t, 3) +
		(2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y) * powf(t, 2) +
		(-p0.y + p2.y) * t + (2 * p1.y)) / 2.0f;

	p.z = ((-p0.z + 3 * p1.z - 3 * p2.z + p3.z) * powf(t, 3) +
		(2 * p0.z - 5 * p1.z + 4 * p2.z - p3.z) * powf(t, 2) +
		(-p0.z + p2.z) * t + (2 * p1.z)) / 2.0f;

	return p;

}

Vector3 MLEngine::Math::Project(const Vector3& v1, const Vector3& v2) {

	Vector3 project;
	float lengthB = MLEngine::Math::Length(v2);

	project = MLEngine::Math::Multiply(MLEngine::Math::Dot(v1, v2) / (lengthB * lengthB), v2);

	return project;

}

Vector3 MLEngine::Math::ClosestPoint(const Vector3& point, const Segment& segment) {

	Vector3 cp;

	cp = MLEngine::Math::Add(segment.origin, MLEngine::Math::Project(MLEngine::Math::Subtract(point, segment.origin), segment.diff));

	return cp;

}

Vector3 MLEngine::Math::Perpendicular(const Vector3& vector) {

	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y, vector.x,0.0f };
	}

	return { 0.0f,-vector.z, vector.y };

}

Vector3 MLEngine::Math::Reflect(const Vector3& input, const Vector3& normal) {

	Vector3 r{};

	r = MLEngine::Math::Subtract(input, MLEngine::Math::Multiply(2 * MLEngine::Math::Dot(input, normal), normal));

	return r;

}

Vector3 MLEngine::Math::RotateOnZAxis(const Vector3& vec, float angle)
{

	float sinTheta = std::sinf(angle);
	float cosTheta = std::cosf(angle);


	return Vector3{
		vec.x * cosTheta - vec.y * sinTheta,
		vec.x * sinTheta + vec.y * cosTheta,
		vec.z
	};
}

Vector3 MLEngine::Math::RotateOnYAxis(const Vector3& vec, float angle)
{

	float sinTheta = std::sinf(angle);
	float cosTheta = std::cosf(angle);

	return Vector3{
		vec.x * cosTheta + vec.z * sinTheta,
		vec.y,
		-vec.x * sinTheta + vec.z * cosTheta
	};
}

void MLEngine::Math::ClampAABB(AABB& aabb) {

	aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
	aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);
	aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
	aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);
	aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
	aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);

}

//OBB回転
void MLEngine::Math::RotateOBB(OBB& obb, const Vector3& rotate) {

	//回転行列を生成
	Matrix4x4 rotateMatrix = Multiply(MakeRotateXMatrix(rotate.x),
		Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));

	//回転行列から軸を抽出
	obb.orientations[0].x = rotateMatrix.m[0][0];
	obb.orientations[0].y = rotateMatrix.m[0][1];
	obb.orientations[0].z = rotateMatrix.m[0][2];

	obb.orientations[1].x = rotateMatrix.m[1][0];
	obb.orientations[1].y = rotateMatrix.m[1][1];
	obb.orientations[1].z = rotateMatrix.m[1][2];

	obb.orientations[2].x = rotateMatrix.m[2][0];
	obb.orientations[2].y = rotateMatrix.m[2][1];
	obb.orientations[2].z = rotateMatrix.m[2][2];

	obb.orientations[0] = MLEngine::Math::Normalize(obb.orientations[0]);
	obb.orientations[1] = MLEngine::Math::Normalize(obb.orientations[1]);
	obb.orientations[2] = MLEngine::Math::Normalize(obb.orientations[2]);

}


//二項演算子
Vector3 MLEngine::Math::operator+(const Vector3& v1, const Vector3& v2) { return MLEngine::Math::Add(v1, v2); }
Vector3 MLEngine::Math::operator-(const Vector3& v1, const Vector3& v2) { return MLEngine::Math::Subtract(v1, v2); }
Vector3 MLEngine::Math::operator*(float s, const Vector3& v) { return MLEngine::Math::Multiply(s, v); }
Vector3 MLEngine::Math::operator*(const Vector3& v, float s) { return MLEngine::Math::Multiply(s, v); }
Vector3 MLEngine::Math::operator/(const Vector3& v, float s) { return MLEngine::Math::Multiply(1.0f / s, v); }

//単項演算子
Vector3 MLEngine::Math::operator-(const Vector3& v) { return { -v.x, -v.y, -v.z }; }
Vector3 MLEngine::Math::operator+(const Vector3& v) { return v; }
