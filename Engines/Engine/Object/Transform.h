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
		//デバッグ用
		void Debug();
		//ワールド行列
		MLEngine::Math::Matrix4x4 worldMatrix;
		//平行移動
		MLEngine::Math::Vector3 translate;
		//回転(オイラー)
		MLEngine::Math::Vector3 rotate;
		//回転(クォータニオン)
		MLEngine::Math::Quaternion rotateQuaternion;
		//スケール
		MLEngine::Math::Vector3 scale;

	private:
		//親
		Transform* parent_ = nullptr;

	};

}

