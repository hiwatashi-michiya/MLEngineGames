#include "PlayScene.h"
#include"Externals/imgui/imgui.h"
#include "DebugScene.h"

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

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();

	bulletManager_ = std::make_unique<BulletManager>();
	bulletManager_->Initialize();

	bulletManager_->SetPlayer(playerManager_->GetPlayer());
	bulletManager_->SetEnemy(enemy_.get());

	enemy_->SetBulletManager(bulletManager_.get());

	BaseScene::SetCamera();

	lifeUI_ = std::make_unique<LifeUI>(playerManager_->GetPlayer());
	lifeUI_->Initialize();
}

void PlayScene::Finalize(){

}

void PlayScene::Update(){

	DrawImgui();

	config_->Update();

	
#ifdef CLIENT_BUILD
	// Client専用処理
#else
	// Server Debug処理
	gameManager_->Update();
#endif	

	playerManager_->Update(gameManager_->GetDeltaTime());

	enemy_->Update();

	bulletManager_->Update();

	lifeUI_->Update();

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
