#pragma once
#include <cstdint>
#include <Windows.h>

/// <summary>
/// 基盤となる機能
/// </summary>
namespace MLEngine::Core {

	/// <summary>
	/// ウィンドウ関連
	/// </summary>
	namespace Window {

		/// <summary>
		/// 画面に表示するウィンドウを生成するクラス
		/// </summary>
		class Manager
		{
		public:

			//インスタンス取得
			static Manager* GetInstance();

			//ウィンドウプロシージャ
			static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

			//ウィンドウ生成
			void CreateGameWindow(const wchar_t* windowName = L"MyEngine",
				int32_t clientWidth = 1280, int32_t clientHeight = 720);

			//メッセージ処理
			bool ProcessMessage();

			//ウィンドウクラスの登録解除
			void TerminateGameWindow();

			/// <summary>
			/// ウィンドウハンドルの取得
			/// </summary>
			/// <returns></returns>
			HWND GetHwnd() const { return hwnd_; }

			/// <summary>
			/// ウィンドウクラスのインスタンス取得
			/// </summary>
			/// <returns></returns>
			HINSTANCE GetHInstance() const { return wc_.hInstance; }
			//横幅取得
			int32_t GetClientWidth() const { return clientWidth_; }
			//縦幅取得
			int32_t GetClientHeight() const { return clientHeight_; }

		private:

			//ウィンドウハンドル
			HWND hwnd_ = nullptr;
			//ウィンドウクラス
			WNDCLASS wc_{};
			//ウィンドウ横幅
			int32_t clientWidth_ = 1280;
			//ウィンドウ縦幅
			int32_t clientHeight_ = 720;

		private:
			Manager() = default;
			~Manager() = default;
			Manager(const Manager&) = delete;
			const Manager& operator=(const Manager&) = delete;

		};

	}

}


