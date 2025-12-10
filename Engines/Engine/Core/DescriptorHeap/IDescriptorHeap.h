#pragma once
#include <d3d12.h>
#include <string>
#include <unordered_map>
#include <wrl.h>
#include <vector>

namespace MLEngine::Core {

	/// <summary>
	/// デスクリプタヒープをインデックスごと管理するクラス
	/// </summary>
	class IDescriptorHeap
	{
	public:

		IDescriptorHeap() = delete;

		IDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t maxDescriptor);

		//CPUのDescriptorHandle取得
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
		//GPUのDescriptorHandle取得
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);
		//ポインタ取得
		ID3D12DescriptorHeap* Get() { return heap_.Get(); }
		//先頭のCPUハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandleStart() { return heap_->GetCPUDescriptorHandleForHeapStart(); }
		//先頭のGPUハンドルを取得
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandleStart() { return heap_->GetGPUDescriptorHandleForHeapStart(); }
		//未使用のインデックスを使用済みにして返す
		virtual uint32_t GetUnUsedIndex();
		//インデックスを未使用に戻す
		void SetIndexUnUsed(uint32_t index);
		//サイズ取得
		uint32_t GetDescriptorSize() const { return kDescriptorSize_; }
		//最大数取得
		uint32_t GetMaxDescriptor() const { return kMaxDescriptor_; }

	protected:

		/// <summary>
		/// 生成
		/// </summary>
		/// <param name="heapType">ヒープの種類</param>
		/// <param name="numDescriptors">デスクリプタ数</param>
		/// <param name="shaderVisible"></param>
		void Create(D3D12_DESCRIPTOR_HEAP_TYPE heapType,
			UINT numDescriptors, bool shaderVisible);

	protected:

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;
		//サイズ
		const uint32_t kDescriptorSize_;
		//デスクリプタの最大数
		const uint32_t kMaxDescriptor_;
		//インデックスが仕様済かどうか
		std::vector<bool> isUsed_;

		//デスクリプタヒープの使用数
		uint32_t index_ = 0;
		//デスクリプタヒープの最大数
		uint32_t limit_ = 0;

	};

}

