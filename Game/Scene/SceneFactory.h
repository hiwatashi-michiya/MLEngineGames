#pragma once
#include "BaseSceneFactory.h"

/// <summary>
/// ゲーム固有のシーン切り替えクラス
/// </summary>
class SceneFactory:  public BaseSceneFactory
{
public:

	BaseScene* CreateScene(const std::string& sceneName) override;

private:



};


