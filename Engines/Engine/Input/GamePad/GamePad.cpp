#include "GamePad.h"
#include "Core/WindowManager.h"

using namespace MLEngine::Core;
using namespace MLEngine::Input;

void GamePad::Initialize() {

}

void GamePad::Update() {

	preJoyState_ = joyState_;

	if (GetJoyState(0, joyState_)) {
		isGetController_ = true;
	}
	else {
		isGetController_ = false;
	}

	CalcDeadZone();

}

bool GamePad::GetJoyState(int32_t No, XINPUT_STATE& joyState) {

	if (XInputGetState(No, &joyState) == ERROR_SUCCESS) {
		return true;
	}

	return false;

}

bool GamePad::PushButton(Button button) {

	if (isGetController_) {

		if (joyState_.Gamepad.wButtons & button) {
			return true;
		}

	}

	return false;

}

bool GamePad::TriggerButton(Button button) {

	if (isGetController_) {

		if ((joyState_.Gamepad.wButtons & button) and (preJoyState_.Gamepad.wButtons & button) == 0) {
			return true;
		}

	}

	return false;

}

bool GamePad::ReleaseButton(Button button) {

	if (isGetController_) {

		if ((joyState_.Gamepad.wButtons & button) == 0 and (preJoyState_.Gamepad.wButtons & button)) {
			return true;
		}

	}

	return false;

}

void GamePad::CalcDeadZone() {

	if (fabsf(joyState_.Gamepad.sThumbLX) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		joyState_.Gamepad.sThumbLX = 0;
	}
	if (fabsf(joyState_.Gamepad.sThumbLY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
		joyState_.Gamepad.sThumbLY = 0;
	}
	if (fabsf(joyState_.Gamepad.sThumbRX) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		joyState_.Gamepad.sThumbRX = 0;
	}
	if (fabsf(joyState_.Gamepad.sThumbRY) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) {
		joyState_.Gamepad.sThumbRY = 0;
	}

}

bool GamePad::TriggerLStick(StickArrow arrow) {

	switch (arrow)
	{
	case StickArrow::S_UP:
		return joyState_.Gamepad.sThumbLY > 0 and !preJoyState_.Gamepad.sThumbLY;
		break;
	case StickArrow::S_DOWN:
		return joyState_.Gamepad.sThumbLY < 0 and !preJoyState_.Gamepad.sThumbLY;
		break;
	case StickArrow::S_LEFT:
		return joyState_.Gamepad.sThumbLX < 0 and !preJoyState_.Gamepad.sThumbLX;
		break;
	case StickArrow::S_RIGHT:
		return joyState_.Gamepad.sThumbLX > 0 and !preJoyState_.Gamepad.sThumbLX;
		break;
	default:
		return false;
		break;
	}

}

bool GamePad::TriggerRStick(StickArrow arrow) {

	switch (arrow)
	{
	case StickArrow::S_UP:
		return joyState_.Gamepad.sThumbRY > 0 and !preJoyState_.Gamepad.sThumbRY;
		break;
	case StickArrow::S_DOWN:
		return joyState_.Gamepad.sThumbRY < 0 and !preJoyState_.Gamepad.sThumbRY;
		break;
	case StickArrow::S_LEFT:
		return joyState_.Gamepad.sThumbRX < 0 and !preJoyState_.Gamepad.sThumbRX;
		break;
	case StickArrow::S_RIGHT:
		return joyState_.Gamepad.sThumbRX > 0 and !preJoyState_.Gamepad.sThumbRX;
		break;
	default:
		return false;
		break;
	}

}

SHORT GamePad::GetStickValue(Stick stick) {

	//いずれかのスティックの値を返す
	switch (stick)
	{
	case Stick::LX:
		return joyState_.Gamepad.sThumbLX;
		break;
	case Stick::LY:
		return joyState_.Gamepad.sThumbLY;
		break;
	case Stick::RX:
		return joyState_.Gamepad.sThumbRX;
		break;
	case Stick::RY:
		return joyState_.Gamepad.sThumbRY;
		break;
	default:
		return 0;
		break;
	}

}
