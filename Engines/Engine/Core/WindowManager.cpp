#include "WindowManager.h"
#include "resource.h"
#include "ImGuiManager.h"

#ifdef _DEBUG

#include "externals/imgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // _DEBUG

using namespace MLEngine::Core::Window;

Manager* Manager::GetInstance() {
	static Manager instance;
	return &instance;
}

//ウィンドウプロシージャ
LRESULT Manager::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

#ifdef _DEBUG

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}

#endif // _DEBUG

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{

		//ウィンドウが破棄された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);

}

void Manager::CreateGameWindow(const wchar_t* windowName, int32_t clientWidth, int32_t clientHeight) {

	

	//ウィンドウプロシージャ
	wc_.lpfnWndProc = (WNDPROC)WindowProc;

	//ウィンドウクラス名(なんでも良い)
	wc_.lpszClassName = windowName;

	//インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);

	//カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//アイコン
	wc_.hIcon = LoadIcon(wc_.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	//ウィンドウクラスを登録する
	RegisterClass(&wc_);

	//ウィンドウサイズセット
	clientWidth_ = clientWidth;
	clientHeight_ = clientHeight;

	//ウィンドウスタイル保存
	windowStyle_ = WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME);

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0,0, clientWidth_, clientHeight_ };

	//クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	//ウィンドウの生成
	hwnd_ = CreateWindow(
		wc_.lpszClassName,      //利用するクラス名
		windowName,                //タイトルバーの文字(何でも良い)
		WS_OVERLAPPEDWINDOW,   //よく見るウィンドウスタイル
		CW_USEDEFAULT,         //表示X座標(Windowsに任せる)
		CW_USEDEFAULT,         //表示Y座標(WindowsOSに任せる)
		wrc.right - wrc.left,  //ウィンドウ横幅
		wrc.bottom - wrc.top,  //ウィンドウ縦幅
		nullptr,               //親ウィンドウハンドル
		nullptr,               //メニューハンドル
		wc_.hInstance,          //インスタンスハンドル
		nullptr);              //オプション

	//ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);

}

void Manager::SetWindowMode()
{

	if (currentMode_ == WindowMode::Window) { return; }

	currentMode_ = WindowMode::Window;

	SetWindowLong(hwnd_, GWL_STYLE, windowStyle_);

	SetWindowPos(hwnd_, HWND_NOTOPMOST, windowRect_.left, windowRect_.top,
		windowRect_.right - windowRect_.left, windowRect_.bottom - windowRect_.top,
		SWP_FRAMECHANGED | SWP_NOACTIVATE);

	ShowWindow(hwnd_, SW_NORMAL);

#ifdef _DEBUG

	ImGuiManager::GetInstance()->SetDisplay();

#endif // _DEBUG


}

void Manager::SetFullScreenMode()
{

	if (currentMode_ == WindowMode::FullScreen) { return; }

	currentMode_ = WindowMode::FullScreen;

	//ウィンドウ情報を保存
	GetWindowRect(hwnd_, &windowRect_);
	//フルスクリーン化
	SetWindowLong(hwnd_, GWL_STYLE, windowStyle_ &
		~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME));


	RECT fullScreenRect{ 0 };
	HMONITOR hMonitor = MonitorFromWindow(hwnd_, MONITOR_DEFAULTTONEAREST);
	MONITORINFO monitorInfo;
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfo(hMonitor, &monitorInfo);

	fullScreenRect.right = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
	fullScreenRect.bottom = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

	SetWindowPos(
		hwnd_, HWND_NOTOPMOST, fullScreenRect.left, fullScreenRect.top,
		fullScreenRect.right, fullScreenRect.bottom, SWP_FRAMECHANGED | SWP_NOACTIVATE);

	ShowWindow(hwnd_, SW_MAXIMIZE);

#ifdef _DEBUG

	ImGuiManager::GetInstance()->SetDisplay();

#endif // _DEBUG

}

bool Manager::ProcessMessage() {

	//メッセージ
	MSG msg{};

	//メッセージがあるか確認
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg); //キー入力メッセージの処理
		DispatchMessage(&msg); //ウィンドウプロシージャにメッセージを送る
	}

	//終了メッセージが来たらループを抜ける
	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;

}

void Manager::TerminateGameWindow() {

	UnregisterClass(wc_.lpszClassName, wc_.hInstance);

}
