#pragma once
#include "Model/Animation/Animation.h"
#include "RenderingData.h"
#include "Matrix4x4.h"
#include <algorithm>
#include <array>
#include <d3d12.h>
#include <span>
#include <vector>
#include <wrl.h>

namespace MLEngine::Resource::Skinning {

	//最大4Jointの影響を受ける
	const uint32_t kNumMaxInfluence = 4;

	/// <summary>
	/// 頂点情報
	/// </summary>
	struct VertexInfluence {
		std::array<float, kNumMaxInfluence> weights;
		std::array<int32_t, kNumMaxInfluence> jointIndices;
	};
	/// <summary>
	/// GPUに送るスケルトンの行列データ
	/// </summary>
	struct WellForGPU {
		MLEngine::Math::Matrix4x4 skeletonSpaceMatrix; //位置用
		MLEngine::Math::Matrix4x4 skeletonSpaceInverseTransposeMatrix; //法線用
	};

	/// <summary>
	/// スキンクラスターのデータをまとめたクラス
	/// </summary>
	class SkinCluster
	{
	public:
		SkinCluster();
		~SkinCluster();
		//生成
		void Create(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const Skeleton& skeleton,
			const MLEngine::Resource::ModelData& modelData);
		//更新
		void Update(const Skeleton& skeleton);

		std::vector<MLEngine::Math::Matrix4x4> inverseBindPoseMatrices_;

		Microsoft::WRL::ComPtr<ID3D12Resource> influenceResource_;
		D3D12_VERTEX_BUFFER_VIEW influenceBufferView_;
		std::span<VertexInfluence> mappedInfluence_;
		Microsoft::WRL::ComPtr<ID3D12Resource> paletteResource_;
		std::span<WellForGPU> mappedPalette_;
		std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> paletteSrvHandle_;

	private:

	};

}


