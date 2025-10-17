#include <Windows.h>
#include "Engine/MLEngine.h"
#include "Game/Scene/DebugScene.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
	MLEngine::Run("卒業制作", new DebugScene());

	return 0;

}