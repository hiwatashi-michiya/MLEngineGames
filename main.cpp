#include <Windows.h>
#include "Engine/MLEngine.h"
#include "Game/Scene/PlayScene.h"
#include "Game/Scene/SceneFactory.h"
#include "Leak.h"

D3DResourceLeakChecker leak;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//テンプレートでゴリ押し
	MLEngine::Run<PlayScene, SceneFactory>("卒業制作");

	return 0;

}