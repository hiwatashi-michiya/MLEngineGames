#pragma once
#include "Object/Camera.h"
#include "Vector3.h"
#include "Vector4.h"
#include <d3d12.h>
#include <dxcapi.h>
#include <stdint.h>
#include <wrl.h>
#include <memory>
#include <array>

namespace MLEngine::Resource {

	/// <summary>
	/// 線分の描画クラス
	/// </summary>
	class Line
	{
	public:
		Line();
		~Line();
		//頂点数
		static const int32_t kVertexNum_ = 2;
		//静的初期化
		static void Initialize(ID3D12Device* device);
		//描画前処理
		static void PreDraw(ID3D12GraphicsCommandList* commandList);
		//描画後処理
		static void PostDraw();
		//描画コマンドを積む
		void Draw(MLEngine::Object::Camera* camera, const MLEngine::Math::Matrix4x4& matrix = MLEngine::Math::MakeIdentity4x4());
		//実際の描画
		void Render();
		//始点
		MLEngine::Math::Vector3 start;
		//終点
		MLEngine::Math::Vector3 end;
		//色
		MLEngine::Math::Vector4 color;
		//表示フラグ
		bool isActive = true;

	private:

		//デバイス
		static ID3D12Device* device_;
		//コマンドリスト
		static ID3D12GraphicsCommandList* commandList_;
		//ルートシグネチャ
		static ID3D12RootSignature* rootSignature_;
		//PSO
		static ID3D12PipelineState* pipelineState_;

	private:
		//線の頂点データ
		struct LineVertexData {
			MLEngine::Math::Vector4 position;
			MLEngine::Math::Vector4 color;
		};

		//頂点リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuff_;
		//頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView_{};

		//ビュープロジェクション行列リソース
		Microsoft::WRL::ComPtr<ID3D12Resource> wvpBuff_;

		MLEngine::Math::Matrix4x4* wvpMatrix_ = nullptr;

		LineVertexData* vertexMap_ = nullptr;

	};

	/// <summary>
	/// 主に当たり判定に使う線の箱描画
	/// </summary>
	class LineBox
	{
	public:
		LineBox();
		~LineBox();
		//OBBセット
		void SetOBB(MLEngine::Math::OBB* obb) { obb_ = obb; }
		//更新
		void Update();
		//色セット
		void SetColor(MLEngine::Math::Vector4 color);
		//描画コマンド積む
		void Draw(MLEngine::Object::Camera* camera, const MLEngine::Math::Matrix4x4& matrix = MLEngine::Math::MakeIdentity4x4());
		//表示フラグセット
		void SetIsActive(bool flag);

	private:

		//描画に使用する線の数
		static const uint32_t kMaxLine_ = 12;

		MLEngine::Math::OBB* obb_ = nullptr;
		//線分
		std::array<std::unique_ptr<Line>, kMaxLine_> lines_;

	};

	/// <summary>
	/// 主に当たり判定に使う線の箱描画
	/// </summary>
	class LineSphere
	{
	public:
		LineSphere();
		~LineSphere();
		//OBBセット
		void SetSphere(MLEngine::Math::Sphere* sphere) { sphere_ = sphere; }
		//更新
		void Update();
		//色セット
		void SetColor(MLEngine::Math::Vector4 color);
		//描画コマンド積む
		void Draw(MLEngine::Object::Camera* camera, const MLEngine::Math::Matrix4x4& matrix = MLEngine::Math::MakeIdentity4x4());
		//表示フラグセット
		void SetIsActive(bool flag);

	private:

		//球体の分割数
		static const uint32_t kSubdivision_ = 6;
		//描画に使用する線の数(分割数 * 分割数 * 2)
		static const uint32_t kMaxLine_ = 72;

		MLEngine::Math::Sphere* sphere_ = nullptr;
		//線分
		std::array<std::unique_ptr<Line>, kMaxLine_> lines_;

	};

}



