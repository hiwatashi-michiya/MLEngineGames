#include "Input.h"
#include "Core/WindowManager.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "xinput.lib")

using namespace MLEngine::Core;
using namespace MLEngine;

Input::Manager* Input::Manager::GetInstance() {
	static Input::Manager instance;
	return &instance;
}

void Input::Manager::Initialize() {

	Window::Manager* winApp = Window::Manager::GetInstance();

	hwnd_ = winApp->GetHwnd();

	HRESULT hr = S_FALSE;

	//DirectInputの初期化
	hr = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&dInput_, nullptr);
	assert(SUCCEEDED(hr));

	gamePad_.Initialize();

	keyboard_.Initialize(winApp, dInput_.Get());

	mouse_.Initialize(winApp, dInput_.Get());
	

}

void Input::Manager::Update() {

	gamePad_.Update();

	keyboard_.Update();

	mouse_.Update();	

}

void Input::Manager::Finalize() {

	keyboard_.Finalize();
	mouse_.Finalize();

}

Input::Manager::~Manager() {

	
	

}


