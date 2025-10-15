#pragma once
#include "Camera.h"
#include <memory>
#include "Vector3.h"
#include "Input.h"

namespace MLEngine::Object {

	/// <summary>
	/// マウスで操作可能なデバッグ用のカメラ
	/// </summary>
	class DebugCamera
	{
	public:
		DebugCamera();
		~DebugCamera();

		//カメラ取得
		MLEngine::Object::Camera* GetCamera() { return camera_.get(); }
		//初期化
		void Initialize();
		//更新
		void Update();
		//デバッグ
		void Debug();
		//位置セット
		void SetPosition(const MLEngine::Math::Vector3& position) { camera_->position_ = position; }
		//回転セット
		void SetRotate(const MLEngine::Math::Vector3& rotate) { camera_->rotation_ = rotate; }

	private:

		MLEngine::Input::Manager* input_ = nullptr;

		std::shared_ptr<MLEngine::Object::Camera> camera_;

		//初期座標
		MLEngine::Math::Vector3 startPosition_ = { 0.0f,65.0f, -60.0f };

		//初期回転
		float startRotate_ = 0.9f;

		//回転感度
		float rotateSensi_ = 0.003f;
		//移動感度
		float moveSensi_ = 0.10f;

	};

}



