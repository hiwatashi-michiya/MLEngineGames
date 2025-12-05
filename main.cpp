#include <Windows.h>
#include "Engine/MLEngine.h"
#include "Game/Scene/PlayScene.h"
#include "Game/Scene/SceneFactory.h"
#include "Leak.h"

D3DResourceLeakChecker leak;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//テンプレートでゴリ押し
	
#ifdef CLIENT_BUILD
	// Client専用処理
	MLEngine::Run<PlayScene, SceneFactory>("卒業制作-クライアント");
#else
	// Server処理
	MLEngine::Run<PlayScene, SceneFactory>("卒業制作-メイン");
#endif

	return 0;

}