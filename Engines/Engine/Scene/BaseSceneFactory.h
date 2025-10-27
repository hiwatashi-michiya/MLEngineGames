#pragma once
#include <string>

class BaseScene;

/// <summary>
/// 基底のシーン切り替えクラス
/// </summary>
class BaseSceneFactory
{
public:

	virtual ~BaseSceneFactory() = default;

	virtual BaseScene* CreateScene(const std::string& sceneName) = 0;

private:



};
