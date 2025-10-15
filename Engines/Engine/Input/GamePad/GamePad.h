#pragma once
#define DIRECTINPUT_VERSION 0x0800

#include <array>
#include <dinput.h>
#include <wrl.h>
#include <Xinput.h>
#include "Vector2.h"

namespace MLEngine::Input {

	//押し込みボタン
	enum Button {
		A = XINPUT_GAMEPAD_A,
		B = XINPUT_GAMEPAD_B,
		X = XINPUT_GAMEPAD_X,
		Y = XINPUT_GAMEPAD_Y,
		UP = XINPUT_GAMEPAD_DPAD_UP,
		DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
		RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
		LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
		RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
		RThumb = XINPUT_GAMEPAD_RIGHT_THUMB,
		LThumb = XINPUT_GAMEPAD_LEFT_THUMB,
		START = XINPUT_GAMEPAD_START,
		BACK = XINPUT_GAMEPAD_BACK,
	};
	//スティック
	enum Stick {
		LX,
		LY,
		RX,
		RY,
	};
	//スティックの上下左右判定
	enum StickArrow {
		S_UP,
		S_DOWN,
		S_LEFT,
		S_RIGHT,
	};

	/// <summary>
	/// ゲームパッド入力クラス
	/// </summary>
	class GamePad {
	public:

		GamePad() = default;
		~GamePad() = default;

		//初期化
		void Initialize();
		//更新
		void Update();

		//コントローラ起動フラグ取得
		bool GetJoyState(int32_t No, XINPUT_STATE& joyState);
		//ボタンが押されているか
		bool PushButton(Button button);
		//ボタンを押した瞬間
		bool TriggerButton(Button button);
		//ボタンを離した瞬間
		bool ReleaseButton(Button button);

		//いずれかの方向に左スティックを倒した瞬間
		bool TriggerLStick(StickArrow arrow);
		//いずれかの方向に右スティックを倒した瞬間
		bool TriggerRStick(StickArrow arrow);

		//ゲームパッド取得
		XINPUT_GAMEPAD GetGamepad() { return joyState_.Gamepad; }

		//コントローラーが繋がれているか
		bool GetIsGamepad() { return isGetController_; }

		//スティックのそれぞれの値を取得
		SHORT GetStickValue(Stick stick);

	private:

		//デッドゾーン計算
		void CalcDeadZone();

		//コントローラの現在と一フレーム前の状態
		XINPUT_STATE joyState_;
		XINPUT_STATE preJoyState_;
		//コントローラに繋がれているか
		bool isGetController_ = false;


	};

}
