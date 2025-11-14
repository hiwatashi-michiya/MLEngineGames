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
		//メッシュ切り替え
		void SetMesh(const std::string& filename, const std::string& texturename = "");

		//ワールド行列セット
		void SetWorldMatrix(const MLEngine::Math::Matrix4x4& matrix) { worldMatrix = matrix; }
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
		//自分自身をインスタンシングモデルに登録
		void Regist();
		//ワールド行列
		MLEngine::Math::Matrix4x4 worldMatrix;
		//アニメーションのマトリックス
		MLEngine::Math::Matrix4x4 localMatrix;
		//ワールドビュープロジェクション行列
		MLEngine::Math::Matrix4x4 worldViewProjectionMatrix;
		//色
		MLEngine::Math::Vector4 color;
		//アクティブフラグ
		bool isActive = true;

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



