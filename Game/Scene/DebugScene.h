#pragma once
#include "Scene/BaseScene.h"
#include "Model/Model.h"
#include "Object/Camera.h"

class DebugScene : public BaseScene
{
public:
	DebugScene();
	~DebugScene();

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:

	MLEngine::Object::Camera camera_;

	MLEngine::Resource::RigidModel model_;

};


