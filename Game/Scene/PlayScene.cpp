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
	//// Client専用処理

	GameManager::GameState gameState{};

	NetworkManager::GetInstance().GetSceneState(gameState);

	gameManager_->SetState(static_cast<GameManager::GameState>(gameState));
#else
	// Server Debug処理
	gameManager_->Update(playerManager_->GetPlayer()->GetIsJustTurned(), playerManager_->GetPlayer()->GetIsJustMoved());

#endif	
	
	if (gameManager_->GetState() == GameManager::GameState::Title or gameManager_->GetState() == GameManager::GameState::Result){
		playerManager_->GetPlayer()->SetIsTitleScene(true);
	}
	else {
		playerManager_->GetPlayer()->SetIsTitleScene(false);
	}

	playerManager_->Update(gameManager_->GetDeltaTime());

	if (gameManager_->GetState() == GameManager::GameState::Playing){
		enemy_->SetIsActive(true);
		enemy_->Update();
		bulletManager_->SetIsModelActive(true);
		bulletManager_->Update();

		lifeUI_->Update();


	}
	else {
		bulletManager_->SetIsModelActive(false);
		enemy_->SetIsActive(false);
	}
	
	if (playerManager_->GetPlayer()->GetIsDead()){
		gameManager_->SetGameEnd(true);
	}
	else if (enemy_->GetIsDead()){
		gameManager_->SetGameEnd(true);
		gameManager_->SetIsClear(true);
	}


	camera_.Update();

	gameManager_->SceneUpdate();


#ifdef CLIENT_BUILD
	// Client専用処理
#else
	// Server Debug処理
	//managerを介してクライアントに送る
	GameStatePacket gamePacket{};
	gamePacket.header.type = 2;
	gamePacket.header.size = sizeof(GameStatePacket);
	gamePacket.gameState = gameManager_->GetState();

	NetworkManager::GetInstance().Send(gamePacket);
#endif	

	
#ifdef _DEBUG
	if (input_->GetKeyboard()->Trigger(DIK_0)){
		sceneManager_->ChangeScene("Play");
	}

#endif // _DEBUG

}

void PlayScene::Draw(){
	playerManager_->Draw();
}


void PlayScene::DrawImgui() {
#ifdef _DEBUG
	config_->Debug();

	gameManager_->Debug();

	ImGui::Begin("シーン");

	switch (gameManager_->GetState()) {
	case GameManager::GameState::Title:
		ImGui::Text("タイトル");

		break;
	case GameManager::GameState::Tutorial:
		ImGui::Text("チュートリアル");
		break;
	case GameManager::GameState::Playing:
		ImGui::Text("ゲームプレイ");


		break;
	case GameManager::GameState::Result:
		ImGui::Text("リザルト");

		break;
	default:
		break;
	}

	

	ImGui::End();

#endif // _DEBUG
}
