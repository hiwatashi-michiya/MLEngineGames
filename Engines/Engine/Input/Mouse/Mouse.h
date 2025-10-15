#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <array>
#include <dinput.h>
#include <wrl.h>
#include <Xinput.h>
#include "Vector2.h"
#include "Core/WindowManager.h"

namespace MLEngine::Input {

	//マウスボタン
	enum MouseButton {
		kLeft,
		kRight,
		kCenter,
	};

	/// <summary>
	/// マウス入力クラス
	/// </summary>
	class Mouse {
	public:

		Mouse() = default;
		~Mouse() = default;

		//初期化
		void Initialize(MLEngine::Core::Window::Manager* winApp, IDirectInput8* dInput);
		//更新
		void Update();
		//終了処理
		void Finalize();
		//マウスボタンを押しているか
		bool PushMouse(MouseButton mouse);
		//マウスボタンを押した瞬間
		bool TriggerMouse(MouseButton mouse);
		//マウスボタンを離した瞬間
		bool ReleaseMouse(MouseButton mouse);
		//マウスの移動取得
		MLEngine::Math::Vector2 GetMouseMove();
		//マウスホイールの移動量取得
		int32_t GetMouseWheel() const { return mouse_.lZ; }
		//マウスの座標取得
		MLEngine::Math::Vector2 GetMousePosition() const { return mousePosition_; }

	private:

		Microsoft::WRL::ComPtr<IDirectInputDevice8> mouseDevice_ = nullptr;
		MLEngine::Core::Window::Manager* winApp_;

		//マウスの現在と一フレーム前の状態
		DIMOUSESTATE2 mouse_;
		DIMOUSESTATE2 preMouse_;
		//マウス座標
		MLEngine::Math::Vector2 mousePosition_{};

	};

}

