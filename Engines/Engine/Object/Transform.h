#pragma once
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "Vector3.h"

namespace MLEngine::Object {

	/// <summary>
	/// スケール、回転、平行移動とワールド行列をまとめたクラス
	/// </summary>
	class Transform
	{
	public:

		Transform();
		~Transform();
		//行列更新
		void UpdateMatrix();
		//親セット
		void SetParent(Transform* transform) { parent_ = transform; }
		//ワールド行列
		MLEngine::Math::Matrix4x4 worldMatrix_;
		//平行移動
		MLEngine::Math::Vector3 translate_;
		//回転(オイラー)
		MLEngine::Math::Vector3 rotate_;
		//回転(クォータニオン)
		MLEngine::Math::Quaternion rotateQuaternion_;
		//スケール
		MLEngine::Math::Vector3 scale_;

	private:
		//親
		Transform* parent_ = nullptr;

	};

}

