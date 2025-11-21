#pragma once
#include "Vector4.h"
#include "Vector3.h"
#include <d3d12.h>
#include <wrl.h>

namespace MLEngine::Lighting {

	/// <summary>
	/// 平行光源
	/// </summary>
	class DirectionalLight
	{
	public:
		DirectionalLight();
		~DirectionalLight();

		/// <summary>
		/// レンダリング用データ
		/// </summary>
		struct CBData {
			//色
			MLEngine::Math::Vector4 color;
			//方向
			MLEngine::Math::Vector3 direction;
			//強さ
			float intensity;
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
