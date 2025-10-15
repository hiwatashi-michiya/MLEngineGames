#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <array>
#include <dinput.h>
#include <wrl.h>
#include <Xinput.h>
#include "Vector2.h"
#include "GamePad/GamePad.h"
#include "Keyboard/Keyboard.h"
#include "Mouse/Mouse.h"

namespace MLEngine::Input {

	class Manager
	{
	public:

		//インスタンス取得
		static Manager* GetInstance();

		//初期化
		void Initialize();
		//更新
		void Update();
		//終了処理
		void Finalize();

		//キーボード取得
		Keyboard* GetKeyboard() { return &keyboard_; }
		//コントローラー取得
		GamePad* GetGamePad() { return &gamePad_; }
		//マウス取得
		Mouse* GetMouse() { return &mouse_; }
	private:

		//シングルトン化
		Manager() = default;
		~Manager();
		Manager(const Manager&) = delete;
		const Manager& operator=(const Manager&) = delete;

	private:

		Microsoft::WRL::ComPtr<IDirectInput8> dInput_ = nullptr;

		GamePad gamePad_;

		Keyboard keyboard_;

		Mouse mouse_;

		HWND hwnd_;

	};

}


