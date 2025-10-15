#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

namespace MLEngine::Math {

	/// <summary>
	/// 四元数。クォータニオン
	/// </summary>
	class Quaternion
	{
	public:
		Quaternion();
		Quaternion(float x, float y, float z, float w);

		float x;
		float y;
		float z;
		float w;

		/// <summary>
		/// 自身と引数のクォータニオンの積
		/// </summary>
		/// <param name="hs">自身に掛けるクォータニオン</param>
		/// <returns></returns>
		Quaternion Multiply(const Quaternion& hs);

		/// <summary>
		/// 単位クォータニオンを返す
		/// </summary>
		/// <returns></returns>
		Quaternion IdentityQuaternion();

		/// <summary>
		/// 共役クォータニオンを返す
		/// </summary>
		/// <returns></returns>
		Quaternion ConjuGate();

		/// <summary>
		/// クォータニオンのノルムを返す
		/// </summary>
		/// <returns></returns>
		float Norm();

		/// <summary>
		/// 正規化したクォータニオンを返す
		/// </summary>
		/// <returns></returns>
		Quaternion Normalize();

		/// <summary>
		/// 逆クォータニオンを返す
		/// </summary>
		/// <returns></returns>
		Quaternion Inverse();

		/// <summary>
		/// 任意軸回転
		/// </summary>
		/// <param name="axis">任意軸</param>
		/// <param name="angle">回転量</param>
		/// <returns></returns>
		Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

		//球面線形補間
		Quaternion Slerp(const Quaternion& qr, float t);

		/// <summary>
		/// オイラー角からの変換
		/// </summary>
		/// <param name="euler">オイラー角回転</param>
		/// <returns></returns>
		Quaternion ConvertFromEuler(const Vector3& euler);

		//回転行列からの変換
		Quaternion ConvertFromRotateMatrix(const Matrix4x4& matrix);

		Quaternion operator*=(const Quaternion& qr) { return this->Multiply(qr); }

	};

	/// <summary>
	/// 単位クォータニオンを返す
	/// </summary>
	/// <returns></returns>
	Quaternion IdentityQuaternion();

	/// <summary>
	/// クォータニオンの積
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns></returns>
	Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);

	/// <summary>
	/// 共役クォータニオンを返す
	/// </summary>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	Quaternion ConjuGate(const Quaternion& quaternion);

	/// <summary>
	/// クォータニオンのノルムを返す
	/// </summary>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	float Norm(const Quaternion& quaternion);

	/// <summary>
	/// 正規化したクォータニオンを返す
	/// </summary>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	Quaternion Normalize(const Quaternion& quaternion);

	/// <summary>
	/// 逆クォータニオンを返す
	/// </summary>
	/// <param name="quaternion"></param>
	/// <returns></returns>
	Quaternion Inverse(const Quaternion& quaternion);

	/// <summary>
	/// 任意軸回転
	/// </summary>
	/// <param name="axis">任意軸</param>
	/// <param name="angle">回転量</param>
	/// <returns></returns>
	Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

	//オイラー角からの変換
	Quaternion ConvertFromEuler(const Vector3& euler);
	//回転行列からの変換
	Quaternion ConvertFromRotateMatrix(const Matrix4x4& matrix);

	//Slerpを使った回転処理
	Quaternion RotateForAxis(const Quaternion& q, const Vector3& axis, const Vector3& vec, float t);

	//球面線形補間
	Quaternion Slerp(const Quaternion& qr0, const Quaternion& qr1, float t);

	//ある方向からある方向へのベクトル
	Quaternion DirectionToDirectionQuaternion(const Vector3& from, const Vector3& to);

	Vector3 operator*(const Quaternion& q, const Vector3& v);

	Quaternion operator*(const Quaternion& qr1, const Quaternion& qr2);
	Quaternion operator*(float val, const Quaternion& qr);
	Quaternion operator*(const Quaternion& qr, float val);
	Quaternion operator+(const Quaternion& qr1, const Quaternion& qr2);
	Quaternion operator-(const Quaternion& qr1);

}

