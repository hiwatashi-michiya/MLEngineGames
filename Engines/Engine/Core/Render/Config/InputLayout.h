#pragma once
#include <d3d12.h>
#include <vector>
#include "InputElement.h"

namespace MLEngine::Core::Render::Config {

	/// <summary>
	/// インプットレイアウトを扱いやすくしたクラス
	/// </summary>
	class InputLayout
	{
	public:
		InputLayout();
		~InputLayout();
		//エレメントの集まりをセット
		void SetElements(const std::vector<D3D12_INPUT_ELEMENT_DESC>& inputElement);
		//インプットレイアウトの本体取得
		const D3D12_INPUT_LAYOUT_DESC& Get() const { return inputLayout_; }

	private:

		D3D12_INPUT_LAYOUT_DESC inputLayout_{};

	};

}
