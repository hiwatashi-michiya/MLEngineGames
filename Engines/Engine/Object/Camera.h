#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Quaternion.h"

namespace MLEngine::Object {

	/// <summary>
	/// 描画時に使う視点カメラ
	/// </summary>
	class Camera
	{
	public:
		//初期化
		void Initialize();
		//更新
		void Update();

		//ローカルスケール
		MLEngine::Math::Vector3 scale_ = { 1.0f,1.0f,1.0f };

		//ローカル回転角
		MLEngine::Math::Vector3 rotation_ = { 0.0f,0.0f,0.0f };

		//ローカル座標
		MLEngine::Math::Vector3 position_ = { 0.0f,0.0f,0.0f };

		//回転行列
		MLEngine::Math::Matrix4x4 matRotate_ = MLEngine::Math::MakeIdentity4x4();

		//ワールド行列(カメラ行列)
		MLEngine::Math::Matrix4x4 matWorld_;

		//ビュー行列
		MLEngine::Math::Matrix4x4 matView_;
		//プロジェクション行列
		MLEngine::Math::Matrix4x4 matProjection_;
		//ビュープロジェクション行列
		MLEngine::Math::Matrix4x4 matViewProjection_;
		//ワールド座標取得
		MLEngine::Math::Vector3 GetWorldPosition() {

			MLEngine::Math::Vector3 pos;

			pos.x = matWorld_.m[3][0];
			pos.y = matWorld_.m[3][1];
			pos.z = matWorld_.m[3][2];

			return pos;

		}


	private:

		//クォータニオン回転
		MLEngine::Math::Quaternion rotateQuaternion_ = { 0.0f,0.0f,0.0f,1.0f };

	};

}

