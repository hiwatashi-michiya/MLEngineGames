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
#include "Model.h"
#include "InstancingResource.h"
#include "Texture/Texture.h"

namespace MLEngine::Resource {
	
	/// <summary>
	/// モデルの描画を行うクラス
	/// </summary>
	class InstancingModel
	{
	public:

		//GPUに使う用のパーティクルデータ
		struct InstancingForGPU {
			MLEngine::Math::Matrix4x4 WVP;
			MLEngine::Math::Matrix4x4 World;
			MLEngine::Math::Matrix4x4 WorldInverseTranspose;
			MLEngine::Math::Vector4 color;
		};
		//モデル生成
		static InstancingModel* Create(const std::string& filename);

		static const uint32_t kMaxInstance_ = 4096;

	public:
		//初期化
		void Initialize(const std::string& filename);
		//実際の描画
		void Render(ID3D12GraphicsCommandList* commandList);
		//テクスチャセット
		void SetTexture(MLEngine::Resource::Texture texture) { texture_ = texture; }
		//テクスチャセット
		void SetTexture(const std::string& name);

		//ライト切り替え
		void SetLight(bool flag) { material_->constMap_->enableLighting = flag; }

		//色変更
		void SetColor(const MLEngine::Math::Vector4& color) { material_->constMap_->color = color; }

		//ImGui表示
		void ImGuiUpdate(const std::string& name);

		//インスタンスカウント増加
		void AddInstanceCount() { instanceCount_++; }
		//描画データ追加
		void AddModelData(MLEngine::Resource::RigidModel* model);
		//カメラセット
		void SetCamera(MLEngine::Object::Camera* camera);

		//メッシュ
		MLEngine::Graphics::Mesh* mesh_;

		//マテリアル
		std::unique_ptr<MLEngine::Graphics::Material> material_;

	private:

		//カメラ座標バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> cameraBuff_;
		//画面上のワールド座標バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> matBuff_;

		//現在のインスタンスカウント
		int32_t instanceCount_ = 0;

		//TransformMatrix
		InstancingForGPU* matTransformMap_ = nullptr;
		//カメラ座標マップ
		CameraForGPU* cameraMap_ = nullptr;

		//テクスチャ
		MLEngine::Resource::Texture texture_;

		//インスタンシングリソース
		MLEngine::Resource::InstancingResource instancingResource_;

	};

}


