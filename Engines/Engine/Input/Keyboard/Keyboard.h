#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <array>
#include <dinput.h>
#include <wrl.h>
#include <Xinput.h>
#include "Vector2.h"
#include "Core/WindowManager.h"

namespace MLEngine::Input {

	/// <summary>
	/// キーボード入力クラス
	/// </summary>
	class Keyboard {
	public:

		Keyboard() = default;
		~Keyboard() = default;

		//初期化
		void Initialize(MLEngine::Core::Window::Manager* winApp, IDirectInput8* dInput);
		//更新
		void Update();
		//終了処理
		void Finalize();
		/// <summary>
		/// キーの押下をチェック
		/// </summary>
		/// <param name="keyNumber"></param>
		/// <returns></returns>
		bool PushKey(BYTE keyNumber);
		//キーが押された瞬間
		bool TriggerKey(BYTE keyNumber);
		//キーが離された瞬間
		bool ReleaseKey(BYTE keyNumber);

	private:

		Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard_ = nullptr;

		//キーの現在と一フレーム前の状態
		std::array<BYTE, 256> key_;
		std::array<BYTE, 256> preKey_;

	};

}
