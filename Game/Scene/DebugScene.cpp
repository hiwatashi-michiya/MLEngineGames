#include "DebugScene.h"
#include"Externals/imgui/imgui.h"

DebugScene::DebugScene()
{
}

DebugScene::~DebugScene()
{
}

inline void DebugScene::Initialize()
{
	//お試しプッシュ

	camera_.Initialize();
	camera_.position_ = { 0.0f,0.0f,-30.0f };

	model_.Initialize("./Resources/EngineResources/testObjects/axis.obj");

}

void DebugScene::Finalize()
{
}

void DebugScene::Update()
{
	DrawImgui();
	
	camera_.Update();

}

void DebugScene::Draw()
{

	model_.Draw(&camera_);

}

void DebugScene::DrawImgui(){
	ImGui::Begin("お試し");

	ImGui::Text("テスト");

	ImGui::End();

}
