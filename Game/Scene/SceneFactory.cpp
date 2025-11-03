#include "SceneFactory.h"
#include "DebugScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{

	BaseScene* newScene = nullptr;

	if (sceneName == "DEBUG") {
		newScene = new DebugScene();
	}

	return newScene;
}
