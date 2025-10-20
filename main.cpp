#include <Windows.h>
#include "Engine/MLEngine.h"
#include "Game/Scene/DebugScene.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
	MLEngine::Run<DebugScene>("卒業制作");

	return 0;

}