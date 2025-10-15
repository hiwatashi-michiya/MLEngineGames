#include "Quaternion.h"
#include <functional>
#include <cmath>
#include <algorithm>

using namespace MLEngine::Math;

Quaternion::Quaternion()
{
	//単位クォータニオンで初期化
	*this = IdentityQuaternion();
}

Quaternion::Quaternion(float x, float y, float z, float w) : 
	x(x), y(y), z(z), w(w)
{

}

Quaternion Quaternion::Multiply(const Quaternion& hs) {

	Quaternion qr;

	qr.w = this->w * hs.w - this->x * hs.x - this->y * hs.y - this->z * hs.z;
	qr.x = this->y * hs.z - this->z * hs.y + hs.w * this->x + this->w * hs.x;
	qr.y = this->z * hs.x - this->x * hs.z + hs.w * this->y + this->w * hs.y;
	qr.z = this->x * hs.y - this->y * hs.x + hs.w * this->z + this->w * hs.z;

	*this = qr;

	return qr;

}

Quaternion Quaternion::IdentityQuaternion() {

	*this = { 0.0f,0.0f,0.0f,1.0f };

	return *this;

}

Quaternion Quaternion::ConjuGate() {

	Quaternion qr;

	qr.w = this->w;
	qr.x = -this->x;
	qr.y = -this->y;
	qr.z = -this->z;

	*this = qr;

	return qr;

}

float Quaternion::Norm() {

	float result;

	result = std::sqrtf(std::powf(this->w, 2) + std::powf(this->x, 2)+
		std::powf(this->y, 2) + std::powf(this->z, 2));

	return result;

}

Quaternion Quaternion::Normalize() {

	float norm = this->Norm();

	Quaternion qr;

	qr.w = this->w;
	qr.x = this->x;
	qr.y = this->y;
	qr.z = this->z;

	if (norm != 0.0f) {

		qr.w = this->w / norm;
		qr.x = this->x / norm;
		qr.y = this->y / norm;
		qr.z = this->z / norm;
	}

	*this = qr;

	return qr;

}

Quaternion Quaternion::Inverse() {

	Quaternion qr = this->ConjuGate();

	float squaredNorm = std::powf(this->Norm(), 2);

	if (squaredNorm != 0.0f) {

		qr.w = qr.w / squaredNorm;
		qr.x = qr.x / squaredNorm;
		qr.y = qr.y / squaredNorm;
		qr.z = qr.z / squaredNorm;

	}

	*this = qr;

	return qr;

}

Quaternion Quaternion::MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {

	Quaternion qr;

	qr.w = std::cosf(angle / 2.0f);
	qr.x = axis.x * std::sinf(angle / 2.0f);
	qr.y = axis.y * std::sinf(angle / 2.0f);
	qr.z = axis.z * std::sinf(angle / 2.0f);

	*this = qr;

	return qr;

}

//球面線形補間
Quaternion Quaternion::Slerp(const Quaternion& qr, float t) {

	t = std::clamp(t, 0.0f, 1.0f);

	Quaternion q0 = *this;
	Quaternion q1 = qr;

	float dot = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;

	//遠ければもう片方の回転を利用
	if (dot < 0.0f) {

		q0 = -q0;
		dot = -dot;

	}

	//為す角を求める
	float theta = std::acosf(dot);

	float scale0 = std::sinf((1.0f - t) * theta) / std::sinf(theta);
	float scale1 = std::sinf(t * theta) / std::sinf(theta);

	*this = scale0 * q0 + scale1 * q1;

	return scale0 * q0 + scale1 * q1;

}

Quaternion Quaternion::ConvertFromEuler(const Vector3& euler) {

	return Quaternion(
		std::sin(euler.x * 0.5f) * std::cos(euler.y * 0.5f) * std::cos(euler.z * 0.5f)
		+ (std::cos(euler.x * 0.5f) * std::sin(euler.y * 0.5f) * std::sin(euler.z * 0.5f)),
		std::cos(euler.x * 0.5f) * std::sin(euler.y * 0.5f) * std::cos(euler.z * 0.5f)
		- (std::sin(euler.x * 0.5f) * std::cos(euler.y * 0.5f) * std::sin(euler.z * 0.5f)),
		std::cos(euler.x * 0.5f) * std::cos(euler.y * 0.5f) * std::sin(euler.z * 0.5f)
		+ (std::sin(euler.x * 0.5f) * std::sin(euler.y * 0.5f) * std::cos(euler.z * 0.5f)),
		std::cos(euler.x * 0.5f) * std::cos(euler.y * 0.5f) * std::cos(euler.z * 0.5f)
		- (std::sin(euler.x * 0.5f) * std::sin(euler.y * 0.5f) * std::sin(euler.z * 0.5f))
	);

}

Quaternion Quaternion::ConvertFromRotateMatrix(const Matrix4x4& matrix) {

	Quaternion qr = IdentityQuaternion();

	float elem[4]{}; //0:x 1:y 2:z 3:w

	elem[0] = matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2] + 1.0f;
	elem[1] = -matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2] + 1.0f;
	elem[2] = -matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2] + 1.0f;
	elem[3] = matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2] + 1.0f;

	//最大成分のインデックス
	uint32_t biggestIndex = 0;
	for (uint32_t i = 0; i < 4; i++) {
		if (elem[i] > elem[biggestIndex]) {
			biggestIndex = i;
		}
	}

	float tmpQr[4]{}; //仮クォータニオン
	float v = std::sqrtf(elem[biggestIndex]) * 0.5f;

	tmpQr[biggestIndex] = v;

	float multi = 0.25f / v;

	switch (biggestIndex)
	{
	default:
	case 0:
		tmpQr[1] = (matrix.m[0][1] + matrix.m[1][0]) * multi;
		tmpQr[2] = (matrix.m[2][0] + matrix.m[0][2]) * multi;
		tmpQr[3] = (matrix.m[1][2] - matrix.m[2][1]) * multi;
		break;
	case 1:
		tmpQr[0] = (matrix.m[0][1] + matrix.m[1][0]) * multi;
		tmpQr[2] = (matrix.m[1][2] + matrix.m[2][1]) * multi;
		tmpQr[3] = (matrix.m[2][0] - matrix.m[0][2]) * multi;
		break;
	case 2:
		tmpQr[0] = (matrix.m[2][0] + matrix.m[0][2]) * multi;
		tmpQr[1] = (matrix.m[1][2] + matrix.m[2][1]) * multi;
		tmpQr[3] = (matrix.m[0][1] - matrix.m[1][0]) * multi;
		break;
	case 3:
		tmpQr[0] = (matrix.m[1][2] - matrix.m[2][1]) * multi;
		tmpQr[1] = (matrix.m[2][0] - matrix.m[0][2]) * multi;
		tmpQr[2] = (matrix.m[0][1] - matrix.m[1][0]) * multi;
		break;
	}

	this->x = tmpQr[0];
	this->y = tmpQr[1];
	this->z = tmpQr[2];
	this->w = tmpQr[3];

	return *this;

}

Quaternion MLEngine::Math::RotateForAxis(const Quaternion& q, const Vector3& axis, const Vector3& vec, float t)
{

	//長さのないベクトルを使用しない
	if (Length(vec) == 0.0f) {
		return q;
	}

	//一旦正規化
	Vector3 normVec = Normalize(vec);

	Vector3 tmp = MLEngine::Math::ConjuGate(q) * normVec;

	Quaternion diff = MLEngine::Math::MakeRotateAxisAngleQuaternion(Normalize(Cross(axis, Normalize(tmp))), std::acos(Dot(axis, Normalize(tmp))));

	return MLEngine::Math::Slerp(MLEngine::Math::IdentityQuaternion(), diff, t) * q;

}

Quaternion MLEngine::Math::IdentityQuaternion() { return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); }

Quaternion MLEngine::Math::Multiply(const Quaternion& lhs, const Quaternion& rhs) {

	Quaternion qr;

	qr.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
	qr.x = lhs.y * rhs.z - lhs.z * rhs.y + rhs.w * lhs.x + lhs.w * rhs.x;
	qr.y = lhs.z * rhs.x - lhs.x * rhs.z + rhs.w * lhs.y + lhs.w * rhs.y;
	qr.z = lhs.x * rhs.y - lhs.y * rhs.x + rhs.w * lhs.z + lhs.w * rhs.z;

	return qr;

}

Quaternion MLEngine::Math::ConjuGate(const Quaternion& quaternion) {

	Quaternion qr;

	qr.w = quaternion.w;
	qr.x = -quaternion.x;
	qr.y = -quaternion.y;
	qr.z = -quaternion.z;

	return qr;

}

float MLEngine::Math::Norm(const Quaternion& quaternion) {

	float result;

	result = std::sqrtf(std::powf(quaternion.w, 2) + std::powf(quaternion.x, 2) +
		std::powf(quaternion.y, 2) + std::powf(quaternion.z, 2));

	return result;

}

Quaternion MLEngine::Math::Normalize(const Quaternion& quaternion) {

	float norm = MLEngine::Math::Norm(quaternion);

	Quaternion qr;

	qr.w = quaternion.w;
	qr.x = quaternion.x;
	qr.y = quaternion.y;
	qr.z = quaternion.z;

	if (norm != 0.0f) {

		qr.w = quaternion.w / norm;
		qr.x = quaternion.x / norm;
		qr.y = quaternion.y / norm;
		qr.z = quaternion.z / norm;
	}

	return qr;

}

Quaternion MLEngine::Math::Inverse(const Quaternion& quaternion) {

	Quaternion qr = MLEngine::Math::ConjuGate(quaternion);

	float squaredNorm = std::powf(MLEngine::Math::Norm(quaternion), 2);

	if (squaredNorm != 0.0f) {

		qr.w = qr.w / squaredNorm;
		qr.x = qr.x / squaredNorm;
		qr.y = qr.y / squaredNorm;
		qr.z = qr.z / squaredNorm;

	}

	return qr;

}

Quaternion MLEngine::Math::MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {

	Quaternion qr;

	qr.w = std::cosf(angle / 2.0f);
	qr.x = axis.x * std::sinf(angle / 2.0f);
	qr.y = axis.y * std::sinf(angle / 2.0f);
	qr.z = axis.z * std::sinf(angle / 2.0f);

	return qr;

}

Quaternion MLEngine::Math::ConvertFromEuler(const Vector3& euler) {


	return Quaternion(
		std::sin(euler.x * 0.5f) * std::cos(euler.y * 0.5f) * std::cos(euler.z * 0.5f)
		+ (std::cos(euler.x * 0.5f) * std::sin(euler.y * 0.5f) * std::sin(euler.z * 0.5f)),
		std::cos(euler.x * 0.5f) * std::sin(euler.y * 0.5f) * std::cos(euler.z * 0.5f)
		- (std::sin(euler.x * 0.5f) * std::cos(euler.y * 0.5f) * std::sin(euler.z * 0.5f)),
		std::cos(euler.x * 0.5f) * std::cos(euler.y * 0.5f) * std::sin(euler.z * 0.5f)
		+ (std::sin(euler.x * 0.5f) * std::sin(euler.y * 0.5f) * std::cos(euler.z * 0.5f)),
		std::cos(euler.x * 0.5f) * std::cos(euler.y * 0.5f) * std::cos(euler.z * 0.5f)
		- (std::sin(euler.x * 0.5f) * std::sin(euler.y * 0.5f) * std::sin(euler.z * 0.5f))
	);

}

Quaternion MLEngine::Math::ConvertFromRotateMatrix(const Matrix4x4& matrix) {

	/// 参考資料
	/// http://marupeke296.com/DXG_No58_RotQuaternionTrans.html

	Quaternion qr = MLEngine::Math::IdentityQuaternion();

	float elem[4]{}; //0:x 1:y 2:z 3:w

	elem[0] = matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2] + 1.0f;
	elem[1] = -matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2] + 1.0f;
	elem[2] = -matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2] + 1.0f;
	elem[3] = matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2] + 1.0f;

	//最大成分のインデックス
	uint32_t biggestIndex = 0;
	for (uint32_t i = 0; i < 4; i++) {
		if (elem[i] > elem[biggestIndex]) {
			biggestIndex = i;
		}
	}

	float tmpQr[4]{}; //仮クォータニオン
	float v = std::sqrtf(elem[biggestIndex]) * 0.5f;

	tmpQr[biggestIndex] = v;

	float multi = 0.25f / v;

	switch (biggestIndex)
	{
	default:
	case 0:
		tmpQr[1] = (matrix.m[0][1] + matrix.m[1][0]) * multi;
		tmpQr[2] = (matrix.m[2][0] + matrix.m[0][2]) * multi;
		tmpQr[3] = (matrix.m[1][2] - matrix.m[2][1]) * multi;
		break;
	case 1:
		tmpQr[0] = (matrix.m[0][1] + matrix.m[1][0]) * multi;
		tmpQr[2] = (matrix.m[1][2] + matrix.m[2][1]) * multi;
		tmpQr[3] = (matrix.m[2][0] - matrix.m[0][2]) * multi;
		break;
	case 2:
		tmpQr[0] = (matrix.m[2][0] + matrix.m[0][2]) * multi;
		tmpQr[1] = (matrix.m[1][2] + matrix.m[2][1]) * multi;
		tmpQr[3] = (matrix.m[0][1] - matrix.m[1][0]) * multi;
		break;
	case 3:
		tmpQr[0] = (matrix.m[1][2] - matrix.m[2][1]) * multi;
		tmpQr[1] = (matrix.m[2][0] - matrix.m[0][2]) * multi;
		tmpQr[2] = (matrix.m[0][1] - matrix.m[1][0]) * multi;
		break;
	}

	qr.x = tmpQr[0];
	qr.y = tmpQr[1];
	qr.z = tmpQr[2];
	qr.w = tmpQr[3];

	return qr;

}

//球面線形補間
Quaternion MLEngine::Math::Slerp(const Quaternion& qr0, const Quaternion& qr1, float t) {

	t = std::clamp(t, 0.0f, 1.0f);

	Quaternion q0 = MLEngine::Math::Normalize(qr0);
	Quaternion q1 = MLEngine::Math::Normalize(qr1);

	float dot = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;

	//遠ければもう片方の回転を利用
	if (dot < 0.0f) {

		q0 = -q0;
		dot = -dot;

	}

	//零に近い値
	float epsilon = 0.0005f;

	//特異点に近い場合、sinを使わずに処理
	if (dot >= 1.0f - epsilon) {
		return (1.0f - t) * q0 + t * q1;
	}


	//為す角を求める
	float theta = std::acosf(dot);

	float scale0 = std::sinf((1.0f - t) * theta) / std::sinf(theta);
	float scale1 = std::sinf(t * theta) / std::sinf(theta);

	return scale0 * q0 + scale1 * q1;

}

Quaternion MLEngine::Math::DirectionToDirectionQuaternion(const Vector3& from, const Vector3& to) {

	// 回転軸をクロス積から求める
	Vector3 axis = Cross(from, to);
	// 内積
	float dot = Dot(from, to);
	// 完全に平行な場合、単位クォータニオンを返す
	if (dot > 0.9999f) {
		return { 0.0f, 0.0f, 0.0f, 1.0f };
	}

	//逆向きのベクトルだった場合、垂直なベクトルを一つ選ぶ
	if (dot <= -1.0f) {

		if (from.x != 0.0f || from.y != 0.0f) {

			axis = { from.y, -from.x,0.0f };
			axis = Normalize(axis);
		}
		else if (from.x != 0.0f || from.z != 0.0f) {

			axis = { 0.0f, -from.z, from.x };
			axis = Normalize(axis);

		}

	}
	else {
		axis = Normalize(Cross(from, to));
	}

	// θを求める
	float theta = std::acos(Dot(from, to) / (Length(from) * Length(to)));

	return MLEngine::Math::MakeRotateAxisAngleQuaternion(axis, theta);
}

Vector3 MLEngine::Math::operator*(const Quaternion& q, const Vector3& v) {

	Quaternion tmpQ = { v.x, v.y, v.z, 0.0f };

	Quaternion newQuaternion = q * tmpQ * MLEngine::Math::Inverse(q);

	return { newQuaternion.x ,newQuaternion.y, newQuaternion.z };

}

Quaternion MLEngine::Math::operator*(const Quaternion& qr1, const Quaternion& qr2) { return MLEngine::Math::Multiply(qr1, qr2); }

Quaternion MLEngine::Math::operator*(float val, const Quaternion& qr) {
	return Quaternion(qr.x * val, qr.y * val, qr.z * val, qr.w * val);
}

Quaternion MLEngine::Math::operator*(const Quaternion& qr, float val) {
	return Quaternion(qr.x * val, qr.y * val, qr.z * val, qr.w * val);
}

Quaternion MLEngine::Math::operator+(const Quaternion& qr1, const Quaternion& qr2) {
	return Quaternion(qr1.x + qr2.x, qr1.y + qr2.y, qr1.z + qr2.z, qr1.w + qr2.w);
}
Quaternion MLEngine::Math::operator-(const Quaternion& qr) { return Quaternion(-qr.x, -qr.y, -qr.z, -qr.w); }
