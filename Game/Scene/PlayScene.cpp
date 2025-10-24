#include "PlayScene.h"
#include"Externals/imgui/imgui.h"

using namespace MLEngine::Resource;

PlayScene::PlayScene(){
	input_ = MLEngine::Input::Manager::GetInstance();
	gameManager_ = GameManager::GetInstance();
	config_ = GameConfig::GetInstance();
	config_->Initialize();
}

PlayScene::~PlayScene(){

}

inline void PlayScene::Initialize(){
	gameManager_->Initialize();
	//お試しプッシュ

	camera_.Initialize();
	camera_.position_ = { 0.0f,0.0f,-10.0f };

	playerManager_ = std::make_unique<PlayerManager>();
	playerManager_->Initialize();
}

void PlayScene::Finalize(){

}

void PlayScene::Update(){

	DrawImgui();

	config_->Update();

	gameManager_->Update(0.0f);

	playerManager_->Update(0.0f);

	camera_.Update();

}

void PlayScene::Draw(){
	playerManager_->Draw();

}


void PlayScene::DrawImgui() {
#ifdef _DEBUG
	config_->Debug();

	ImGui::Begin("お試しプレイ");

	ImGui::Text("テスト");

	ImGui::End();

#endif // _DEBUG
}
