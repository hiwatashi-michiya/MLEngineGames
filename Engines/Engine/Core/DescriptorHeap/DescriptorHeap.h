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
	class DescriptorHeap
	{
	public:
		DescriptorHeap();
		~DescriptorHeap();

		void Create(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType,
			UINT numDescriptors, bool shaderVisible, const std::string& name);
		//ポインタ取得
		ID3D12DescriptorHeap* Get() { return heap_.Get(); }
		//現在の使用数を返す
		const uint32_t& GetCurrentIndex();
		//現在の使用数を返し、使用数をインクリメントする
		uint32_t AddIndex();
		//先頭のCPUハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandleStart() { return heap_->GetCPUDescriptorHandleForHeapStart(); }
		//先頭のGPUハンドルを取得
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandleStart() { return heap_->GetGPUDescriptorHandleForHeapStart(); }
		//未使用のインデックスを使用済みにして返す
		uint32_t GetUnUsedIndex();
		//インデックスを未使用に戻す
		void SetIndexUnused(uint32_t index);
	private:

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;
		//インデックスが仕様済かどうか
		std::vector<bool> isUsed_;

		//デスクリプタヒープの使用数
		uint32_t index_ = 0;
		//デスクリプタヒープの最大数
		uint32_t limit_ = 0;

	};

}

