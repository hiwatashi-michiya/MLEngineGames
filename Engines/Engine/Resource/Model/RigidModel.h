#pragma once
#include "Animation/Animation.h"
#include "Object/Camera.h"
#include "../RenderingData.h"
#include "Graphics/Mesh.h"
#include <d3d12.h>
#include <dxcapi.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <wrl.h>
#include "Vector4.h"

namespace MLEngine::Resource {
	class InstancingModel;
}

namespace MLEngine::Resource {

	/// <summary>
	/// モデルの描画を行うクラス
	/// </summary>
	class RigidModel
	{
	public:

		RigidModel() {};
		~RigidModel();

		//初期化
		void Initialize(const std::string& filename, const std::string& texturename = "");
		//描画コマンド積む
		void Draw(MLEngine::Object::Camera* camera);
		//メッシュ切り替え
		void SetMesh(const std::string& filename, const std::string& texturename = "");

		//ワールド行列セット
		void SetWorldMatrix(const MLEngine::Math::Matrix4x4& matrix) { worldMatrix_ = matrix; }
		//アニメーション読み込み
		void LoadAnimation(const std::string& filename);

		/// <summary>
		/// アニメーション開始
		/// </summary>
		/// <param name="isLoop">ループするかどうか</param>
		void StartAnimation(bool isLoop = false) {
			isStartAnimation_ = true;
			isLoop_ = isLoop;
		}
		//アニメーション一時停止
		void StopAnimation() { isStartAnimation_ = false; }
		//アニメーションのリセット
		void ResetAnimation();
		//アニメーション速度変更
		void SetAnimationSpeed(float speed) { animationSpeed_ = speed; }
		//アニメーション更新処理
		void UpdateAnimation();
		//アニメーションの終わりかどうか
		bool IsEndAnimation() const { return isEndAnimation_; }
		//アニメーションのループフラグを設定
		void SetIsLoop(bool flag) { isLoop_ = flag; }
		//インスタンシングモデル取得
		InstancingModel* GetInstancingModel() { return instancingModel_; }


		//ワールド行列
		MLEngine::Math::Matrix4x4 worldMatrix_;
		//アニメーションのマトリックス
		MLEngine::Math::Matrix4x4 localMatrix_;
		//ワールドビュープロジェクション行列
		MLEngine::Math::Matrix4x4 worldViewProjectionMatrix_;
		//色
		MLEngine::Math::Vector4 color_;
		//アクティブフラグ
		bool isActive_ = true;

	private:

		InstancingModel* instancingModel_ = nullptr;

		//アニメーション
		std::unique_ptr<Animation> animation_;

		//スケルトン
		std::unique_ptr<Skeleton> skeleton_;

		//アニメーションタイム
		float animationTime_ = 0.0f;

		//アニメーション速度
		float animationSpeed_ = 1.0f;

		//アニメーション管理フラグ
		bool isStartAnimation_ = false;

		//ループ管理フラグ
		bool isLoop_ = false;

		//アニメーションの終点に到達したかどうか
		bool isEndAnimation_ = false;

	};

}



