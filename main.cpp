#include <Windows.h>
#include "Engine/MLEngine.h"
#include "Game/Scene/DebugScene.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
	MLEngine::Run("Test", new DebugScene());

	return 0;

}