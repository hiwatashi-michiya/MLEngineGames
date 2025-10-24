#include <Windows.h>
#include "Engine/MLEngine.h"
#include "Game/Scene/DebugScene.h"
#include "Game/Scene/PlayScene.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
	MLEngine::Run("卒業制作", new PlayScene());

	return 0;

}