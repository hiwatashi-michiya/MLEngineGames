#pragma once
#include "Model/Animation/Animation.h"
#include "Object/Camera.h"
#include "RenderingData.h"
#include "Graphics/Mesh.h"
#include "SkinCluster.h"
#include <array>
#include <d3d12.h>
#include <dxcapi.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_map>
#include <vector>
#include <wrl.h>

namespace MLEngine::Resource {

	/// <summary>
	/// スキニングモデルの描画を行うクラス
	/// </summary>
	class SkinningModel
	{
	public:

		//モデル全体の初期化
		static void StaticInitialize(ID3D12Device* device);
		//ブレンドモード
		enum BlendMode {
			kNormal,//通常
			kAdd,//加算
			kSubtract,//減算
			kMultiply,//乗算
			kScreen,//スクリーン

			kCountBlend
		};
		//ブレンドモードの名前
		static const char* BlendTexts[BlendMode::kCountBlend];

		//モデル生成
		static SkinningModel* Create(const std::string& filename, int32_t number);

		//描画前処理
		static void PreDraw(ID3D12GraphicsCommandList* commandList);

		//描画後処理
		static void PostDraw();
		//終了処理
		static void Finalize();

		//モデル全体に影響を与えるImGui
		static void StaticImGuiUpdate();

		//全体のブレンドモード変更
		static void SetBlendMode(BlendMode blendMode) { currentBlendMode_ = blendMode; }

	public:
		//初期化
		void Initialize(const std::string& filename, int32_t number);
		//描画コマンド積む
		void Draw(MLEngine::Object::Camera* camera);
		//スケルトン描画
		void DrawSkeleton(MLEngine::Object::Camera* camera);
		//描画コマンド積む
		void Draw(const MLEngine::Math::Matrix4x4& localMatrix, MLEngine::Object::Camera* camera);
		//実際の描画
		void Render();
		//テクスチャセット
		void SetTexture(MLEngine::Resource::Texture texture) { texture_ = texture; }

		//メッシュ切り替え
		void SetMesh(const std::string& objFileName);

		//ライト切り替え
		void SetLight(bool flag) { material_->constMap_->enableLighting = flag; }

		//色変更
		void SetColor(const MLEngine::Math::Vector4& color) { material_->constMap_->color = color; }

		//ImGui表示
		void ImGuiUpdate(const std::string& name);
		//ワールド行列セット
		void SetWorldMatrix(const MLEngine::Math::Matrix4x4& matrix) { worldMatrix_ = matrix; }

		//アニメーションのロード、切り替え時に数字を使う
		void LoadAnimation(const std::string& filename, int32_t number);

		//アニメーションを指定した数字のものに切り替え
		void SetAnimation(int32_t number, bool isReset = true);
		//アニメーション取得
		Animation* GetAnimation() { return animations_[currentFileName_].get(); }
		//スケルトン取得
		Skeleton* GetSkeleton() { return skeleton_.get(); }
		//スケルトンの行列取得
		MLEngine::Math::Matrix4x4 GetSkeletonSpaceMatrix(std::string name) const { return skeleton_->joints[skeleton_->jointMap[name]].skeletonSpaceMatrix; }

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
		//ワールド行列
		MLEngine::Math::Matrix4x4 worldMatrix_;

		//メッシュ
		MLEngine::Graphics::Mesh* mesh_;

		//マテリアル
		std::unique_ptr<MLEngine::Graphics::Material> material_;

		//現在使用しているメッシュのパス
		std::string meshFilePath_;

	private:

		//静的メンバ変数

		//デバイス
		static ID3D12Device* device_;
		//コマンドリスト
		static ID3D12GraphicsCommandList* commandList_;
		//ルートシグネチャ
		static ID3D12RootSignature* rootSignature_;
		//PSO
		static ID3D12PipelineState* pipelineStates_[BlendMode::kCountBlend];

		static IDxcBlob* vs3dBlob_;
		static IDxcBlob* ps3dBlob_;

		//モデル識別用変数(ImGuiで使用)
		static int modelNumber_;

		static BlendMode currentBlendMode_;

	private:

		//カメラ座標バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> cameraBuff_;
		//画面上のワールド座標バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> matBuff_;

		//数字に対応したファイル名を返すマップ
		std::unordered_map<int32_t, std::string> stringMap_;

		//アニメーション
		std::unordered_map<std::string, std::unique_ptr<Animation>> animations_;

		//スケルトン
		std::unique_ptr<Skeleton> skeleton_;

		//スキンクラスター
		std::unique_ptr<Skinning::SkinCluster> skinCluster_;

		//現在再生しているアニメーションのファイル名
		std::string currentFileName_;

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

		//アニメーションのマトリックス
		MLEngine::Math::Matrix4x4 localMatrix_;

		//TransformMatrix
		MLEngine::Math::TransformationMatrix* matTransformMap_ = nullptr;
		//カメラ座標マップ
		CameraForGPU* cameraMap_ = nullptr;

		//テクスチャ
		MLEngine::Resource::Texture texture_;

		//ディゾルブ用のマスク画像
		MLEngine::Resource::Texture maskTexture_;

	};

}


