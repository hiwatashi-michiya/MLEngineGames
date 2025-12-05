#pragma once
#include "IDescriptorHeap.h"

namespace MLEngine::Core {

	/// <summary>
	/// シェーダーリソースビューのヒープ
	/// </summary>
	class SRVHeap : public IDescriptorHeap
	{
	public:
		SRVHeap();
		~SRVHeap();

		//SRV専用挙動。テクスチャの範囲外の未使用のインデックスを返す
		uint32_t GetUnUsedIndex() override;
		//テクスチャの未使用インデックスを返す
		uint32_t GetUnUsedTextureIndex();
		//最大テクスチャ数取得
		uint32_t GetMaxTexture() const { return kMaxTexture_; }
		//最大リソース数取得(テクスチャ除く)
		uint32_t GetMaxDescriptor() const { return kMaxDescriptor_; }

	private:

		//テクスチャの最大数
		const uint32_t kMaxTexture_;

	};

	

}
