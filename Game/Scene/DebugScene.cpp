#include "DebugScene.h"

DebugScene::DebugScene()
{
}

DebugScene::~DebugScene()
{
}

inline void DebugScene::Initialize()
{

	camera_.Initialize();
	camera_.position_ = { 0.0f,0.0f,-30.0f };

	model_.Initialize("./Resources/EngineResources/testObjects/axis.obj");

}

void DebugScene::Finalize()
{
}

void DebugScene::Update()
{

	
	camera_.Update();

}

void DebugScene::Draw()
{

	model_.Draw(&camera_);

}
