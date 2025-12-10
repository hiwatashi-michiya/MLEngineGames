#pragma once
#include "Vector4.h"
#include "Vector3.h"
#include <d3d12.h>
#include <wrl.h>

namespace MLEngine::Lighting {

	/// <summary>
	/// 点光源
	/// </summary>
	class PointLight
	{
	public:
		PointLight();
		~PointLight();

		/// <summary>
		/// レンダリング用データ
		/// </summary>
		struct CBData {
			MLEngine::Math::Vector4 color; //ライトの色
			MLEngine::Math::Vector3 position; //ライトの位置
			float intensity; //輝度
			float radius; //ライトの届く最大距離
			float decay; //減衰率
			float padding[2];
		};

		//バッファ取得
		Microsoft::WRL::ComPtr<ID3D12Resource> GetBuffer() { return buffer_; }
		//コマンドリストにセット
		void SetLightCBV(UINT index);
		//平行光源の変数を格納したデータ
		CBData* cbData;

	private:

		//平行光源バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> buffer_;

	};

}
