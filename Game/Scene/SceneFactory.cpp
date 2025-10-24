#include "SceneFactory.h"
#include "DebugScene.h"
#include "PlayScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{

	BaseScene* newScene = nullptr;

	if (sceneName == "DEBUG") {
		newScene = new DebugScene();
	}
	else if (sceneName == "Play") {
		newScene = new PlayScene();
	}

	return newScene;
}
