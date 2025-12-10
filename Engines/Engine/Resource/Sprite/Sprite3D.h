#pragma once
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
#include "Transform.h"

namespace MLEngine::Resource {
	class InstancingModel;
}

namespace MLEngine::Resource {

	/// <summary>
	/// モデルの描画を行うクラス
	/// </summary>
	class Sprite3D
	{
	public:

		Sprite3D() {};
		~Sprite3D();

		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="textureName">テクスチャのファイルパス</param>
		/// <param name="divide">テクスチャの分割数</param>
		void Initialize(const std::string& textureName, uint32_t divide);

		/// <summary>
		/// アニメーション開始
		/// </summary>
		void StartAnimation() {
			isStartAnimation_ = true;
		}
		//アニメーション一時停止
		void StopAnimation() { isStartAnimation_ = false; }
		//アニメーションにかかる時間を変更
		void SetAnimationTime(float time) { animationTime_ = time; }
		//アニメーション速度変更
		void SetAnimationSpeed(float speed) { animationSpeed_ = speed; }
		//アニメーション更新処理
		void UpdateAnimation();
		//インスタンシングモデル取得
		InstancingModel* GetInstancingModel() { return instancingModel_; }
		//テクスチャのインデックスを取得
		uint32_t GetTextureIndex() const { return texture_.GetIndex(); }
		//テクスチャをセット
		void SetTexture(const std::string& filePath) { texture_.Load(filePath); }
		//自分自身をインスタンシングモデルに登録
		void Regist();
		//デバッグ用
		void Debug();
		//アニメーションのマトリックス
		MLEngine::Math::Matrix4x4 localMatrix;
		//ワールドビュープロジェクション行列
		MLEngine::Math::Matrix4x4 worldViewProjectionMatrix;
		//色
		MLEngine::Math::Vector4 color;
		//アクティブフラグ
		bool isActive = true;
		//マテリアルデータ
		MaterialData materialData;

		//トランスフォーム
		MLEngine::Object::Transform transform;

	private:

		//画像サイズのパラメータセット
		void SetSizeParameters();

	private:

		InstancingModel* instancingModel_ = nullptr;

		//テクスチャ
		MLEngine::Resource::Texture texture_;

		//アスペクト比
		float aspect_ = 1.0f;
		//分割数
		uint32_t divide_ = 1;
		//UVの位置
		MLEngine::Math::Vector2 uvTranslate_{};
		//UVスケール
		MLEngine::Math::Vector2 uvScale_{ 1.0f,1.0f };
		//UV移動量
		float uvMoveSize_ = 1.0f;
		//一ループにかかる時間
		float animationTime_ = 1.0f;
		//切り替えに必要な時間
		float countTime_ = 0.0f;
		//アニメーション速度
		float animationSpeed_ = 1.0f;

		//アニメーション管理フラグ
		bool isStartAnimation_ = false;

	};

}



