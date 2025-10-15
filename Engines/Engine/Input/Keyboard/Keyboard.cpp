#include "Keyboard.h"
#include "Core/WindowManager.h"
#include <cassert>

using namespace MLEngine::Core;
using namespace MLEngine::Input;

void Keyboard::Initialize(Window::Manager* winApp, IDirectInput8* dInput) {

	HRESULT hr = S_FALSE;

	//キーボードデバイスの生成
	hr = dInput->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(hr));

	//入力データ形式のセット
	hr = keyboard_->SetDataFormat(&c_dfDIKeyboard); //標準形式
	assert(SUCCEEDED(hr));

	//排他制御レベルのセット
	hr = keyboard_->SetCooperativeLevel(
		winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

}

void Keyboard::Update() {

	//キーボード情報の取得開始
	keyboard_->Acquire();

	//前回入力の保存
	preKey_ = key_;

	//キー入力
	key_.fill(0);
	keyboard_->GetDeviceState(sizeof(key_), key_.data());

}

void Keyboard::Finalize() {

	if (keyboard_) {
		keyboard_->Unacquire();
	}

}

bool Keyboard::PushKey(BYTE keyNumber) {

	if (key_[keyNumber]) {
		return true;
	}

	return false;

}

bool Keyboard::TriggerKey(BYTE keyNumber) {

	if (key_[keyNumber] and preKey_[keyNumber] == false) {
		return true;
	}

	return false;

}

bool Keyboard::ReleaseKey(BYTE keyNumber) {

	if (key_[keyNumber] == false and preKey_[keyNumber]) {
		return true;
	}

	return false;

}
