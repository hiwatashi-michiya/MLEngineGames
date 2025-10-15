#include "Mouse.h"
#include <cassert>

using namespace MLEngine::Core;
using namespace MLEngine::Input;
using namespace MLEngine::Math;

void Mouse::Initialize(Window::Manager* winApp, IDirectInput8* dInput) {

	winApp_ = winApp;

	HRESULT hr = S_FALSE;

	//マウスデバイスの生成
	hr = dInput->CreateDevice(GUID_SysMouse, &mouseDevice_, NULL);
	assert(SUCCEEDED(hr));

	//入力データ形式のセット
	hr = mouseDevice_->SetDataFormat(&c_dfDIMouse2); //拡張8ボタン
	assert(SUCCEEDED(hr));

	//排他制御レベルのセット
	hr = mouseDevice_->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(hr));

}

void Mouse::Update() {

	//マウスの取得開始
	mouseDevice_->Acquire();

	//前回の入力の保存
	preMouse_ = mouse_;

	//マウス入力
	std::memset(&mouse_, 0, sizeof(mouse_));
	mouseDevice_->GetDeviceState(sizeof(mouse_), &mouse_);

	//スクリーン座標の取得
	POINT mousePosition;
	GetCursorPos(&mousePosition);

	//ウィンドウ座標へ変換
	ScreenToClient(winApp_->GetHwnd(), &mousePosition);
	mousePosition_.x = float(mousePosition.x);
	mousePosition_.y = float(mousePosition.y);

}

void Mouse::Finalize() {

	if (mouseDevice_) {
		mouseDevice_->Unacquire();
	}

}

bool Mouse::PushMouse(MouseButton mouse) {

	return (mouse_.rgbButtons[mouse] & 0x80) != 0;

}

bool Mouse::TriggerMouse(MouseButton mouse) {

	return (mouse_.rgbButtons[mouse] & 0x80) != 0 and
		(preMouse_.rgbButtons[mouse] & 0x80) == 0;

}

bool Mouse::ReleaseMouse(MouseButton mouse) {

	return (mouse_.rgbButtons[mouse] & 0x80) == 0 and
		(preMouse_.rgbButtons[mouse] & 0x80) != 0;

}

Vector2 Mouse::GetMouseMove() {
	Vector2 tmp{};
	tmp.x = float(mouse_.lX);
	tmp.y = float(mouse_.lY);
	return tmp;
}


