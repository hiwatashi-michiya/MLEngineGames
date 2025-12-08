#pragma once
#include "Core/DirectXSetter.h"
#include "RenderTexture.h"
#include <vector>
#include "PostEffects.h"
#include <memory>
#include "Object/Camera.h"

namespace MLEngine::Core::Render::PostEffect {

	/// <summary>
	/// フルスクリーンポストエフェクト描画クラス
	/// </summary>
	class PostEffectDrawer
	{
	public:
		//インスタンス取得
		static PostEffectDrawer* GetInstance();
		//初期化
		void Initialize();
		/// <summary>
		/// 描画コマンド積む
		/// </summary>
		/// <param name="drawNum">書き込むレンダーテクスチャのインデックス</param>
		void Draw(int32_t drawNum);
		/// <summary>
		/// 指定したレンダーテクスチャにバリアを貼る
		/// </summary>
		/// <param name="index">レンダーテクスチャの番号</param>
		void SetBarrier(int32_t index, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
		//デバッグ
		void Debug();
		//エフェクトタイプセット
		void SetType(PostEffectType type) { type_ = type; }
		//カメラセット
		void SetCamera(MLEngine::Object::Camera* camera) { for (int32_t i = 0; i < postEffects_.size(); i++) { postEffects_[i]->SetCamera(camera); } }
		//ポストエフェクトリスト取得
		std::vector<std::shared_ptr<PostEffects>> GetPostEffects() const { return postEffects_; }

	private:

		DirectXSetter* dxSetter_ = nullptr;

		ID3D12Device* device_ = nullptr;
		//ポストエフェクトリスト
		std::vector<std::shared_ptr<PostEffects>> postEffects_;
		//レンダーテクスチャリスト
		std::vector<RenderTexture> renderTextures_;
		//ポストエフェクトのタイプ
		PostEffectType type_ = kDepthBasedOutline;

		//最初に一回だけ描画
		bool isUsedAllEffects_ = false;

	private:
		//シングルトン化
		PostEffectDrawer() = default;
		~PostEffectDrawer() = default;
		PostEffectDrawer(const PostEffectDrawer&) = delete;
		const PostEffectDrawer& operator=(const PostEffectDrawer&) = delete;

	};

}




