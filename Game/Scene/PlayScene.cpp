#include "PlayScene.h"
#include"Externals/imgui/imgui.h"
#include "DebugScene.h"

using namespace MLEngine::Math;

using namespace MLEngine::Resource;

PlayScene::PlayScene(){
	input_ = MLEngine::Input::Manager::GetInstance();
	gameManager_ = GameManager::GetInstance();
	config_ = GameConfig::GetInstance();
	config_->Initialize();
}

PlayScene::~PlayScene(){

}

void PlayScene::GlobalSetValue(){
	GlobalVariables* global = GlobalVariables::GetInstance();

	global->SetValue("TextureState", "TitlePos", titlePos_);
	global->SetValue("TextureState", "TitleScale", titleScale_);
	global->SetValue("TextureState", "ScoreBordPos", scoreBordPos_);
	global->SetValue("TextureState", "ScoreBordScale", scoreBordScale_);

	global->SetValue("TextureState", "TutorialPos3D", tutorialPos_);
	global->SetValue("TextureState", "TutorialScale3D", tutorialScale_);
	global->SetValue("TextureState", "ResultPos", resultPos_);
	global->SetValue("TextureState", "ResultScale", resultScale_);
}

void PlayScene::GlobalGetValue(){
	GlobalVariables* global = GlobalVariables::GetInstance();

	titlePos_ = global->GetVector2Value("TextureState", "TitlePos");
	titleScale_ = global->GetVector2Value("TextureState", "TitleScale");
	scoreBordPos_ = global->GetVector2Value("TextureState", "ScoreBordPos");
	scoreBordScale_ = global->GetVector2Value("TextureState", "ScoreBordScale");

	tutorialPos_ = global->GetVector3Value("TextureState", "TutorialPos3D");
	tutorialScale_ = global->GetVector3Value("TextureState", "TutorialScale3D");
	resultPos_ = global->GetVector2Value("TextureState", "ResultPos");
	resultScale_ = global->GetVector2Value("TextureState", "ResultScale");

}

inline void PlayScene::Initialize(){
	
	GlobalSetValue();

	gameManager_->Initialize();

	camera_.Initialize();
	camera_.position_ = { 0.0f,0.0f,-10.0f };

	playerManager_ = std::make_unique<PlayerManager>();
	playerManager_->Initialize();

	EnemyAttackTurnController::GetInstance().Initialize();

	enemy_ = std::make_unique<Enemy>();
	enemy_->Initialize();
	enemy_->SetCamera(&camera_);

	bulletManager_ = std::make_unique<BulletManager>();
	bulletManager_->Initialize(playerManager_->GetPlayer(), enemy_.get());

	bulletManager_->SetPlayer(playerManager_->GetPlayer());
	bulletManager_->SetEnemy(enemy_.get());

	EnemyAttackTurnController::GetInstance().SetBulletCaveat(bulletManager_->GetBulletCaveat());

	enemy_->SetBulletManager(bulletManager_.get());

	BaseScene::SetCamera();

	lifeUI_ = std::make_unique<LifeUI>(playerManager_->GetPlayer());
	lifeUI_->Initialize();

	groundTexture_ = "./Resources/Texture/ingame_stage.png";
	laneTexture_ = "./Resources/Texture/ingame_stageLine.png";

	planeTransform_ = std::make_unique<MLEngine::Object::Transform>();

	groundPlane_.Initialize(groundTexture_, 1);
	groundPlane_.transform.translate = { 0.0f, 0.0f, 0.0f };
	groundPlane_.transform.scale = { 30.0f,10.0f,1.0f };
	groundPlane_.transform.SetParent(planeTransform_.get());

	lanePlane_.Initialize(laneTexture_, 1);
	lanePlane_.transform.translate = { 0.0f, 0.0f, -0.01f };
	lanePlane_.transform.scale = { 1.0f, 10.0f, 1.0f };
	lanePlane_.transform.SetParent(planeTransform_.get());

	//必須となる情報の読み込み
	titleTexture_.Load("./Resources/Texture/title_logo.png");
	scoreBordTexture_.Load("./Resources/Texture/ingame_UI_scoreBack.png");
	tutorialMoveTexture_ = ("./Resources/Texture/tutorial_ui_move.png");
	tutorialTurnTexture_ = ("./Resources/Texture/tutorial_ui_turn.png");
	gameClearTexture_.Load("./Resources/Texture/gameClear.png");
	gameOverTexture_.Load("./Resources/Texture/gameOver.png");

	titleSprite_.reset(MLEngine::Resource::Sprite2D::Create(titleTexture_, titlePos_, titleColor_));
	titleSprite_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	scoreBordSprite_.reset(MLEngine::Resource::Sprite2D::Create(scoreBordTexture_, scoreBordPos_, scoreBordColor_));
	scoreBordSprite_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);


	tutorialTransform_ = std::make_unique<MLEngine::Object::Transform>();

	tutorialSprite_.Initialize(tutorialMoveTexture_, 1);
	tutorialSprite_.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	tutorialSprite_.isActive = false;
	tutorialSprite_.transform.translate.y = 1.0f;
	tutorialSprite_.transform.SetParent(tutorialTransform_.get());

	resultSprite_.reset(MLEngine::Resource::Sprite2D::Create(gameOverTexture_, resultPos_, resultColor_));
	resultSprite_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	resultSprite_->isActive = false;


	titlePos_ = { 640.0f,120.0f };
	titleScale_ = { 510.0f,505.0f };

	tutorialPos_ = { 1015.0f,200.0f };
	tutorialScale_ = { 550.0f,385.0f };

	resultPos_ = { 640.0f,120.0f };
	resultScale_ = { 512.0f,128.0f };

	translate_.y = -4.0f;
	rotate_.x = 1.48f;
	scale_ = { 10.0f,30.0f,1.0f };

}

void PlayScene::Finalize(){

}

void PlayScene::Update(){

	DrawImgui();

	config_->Update();

	GlobalGetValue();
#ifdef CLIENT_BUILD
	//// Client専用処理

	GameManager::GameState gameState{};

	NetworkManager::GetInstance().GetSceneState(gameState);
	//タイトルに戻ったときに初期化できるように
	if (gameManager_->GetState() == GameManager::GameState::Result and gameState == GameManager::GameState::Title){
		MLEngine::Scene::Manager::GetInstance()->ChangeScene("Play");
	}
	
	gameManager_->SetState(static_cast<GameManager::GameState>(gameState));

	titleSprite_->isActive = false;
	tutorialSprite_->isActive = false;
	resultSprite_->isActive = false;



#else
	// Server 処理
	gameManager_->Update(playerManager_->GetPlayer()->GetIsJustTurned(), playerManager_->GetPlayer()->GetIsJustMoved());

	if (gameManager_->GetState() == GameManager::GameState::Title){
		titleSprite_->isActive = true;
	}
	else {
		titleSprite_->isActive = false;
	}

	if (gameManager_->GetState() == GameManager::GameState::Tutorial) {
		tutorialSprite_.isActive = true;
	}
	else {
		tutorialSprite_.isActive = false;
	}

	if (gameManager_->GetTutorialState() == GameManager::TutorialState::LaneMove) {
		tutorialSprite_.SetTexture(tutorialMoveTexture_);
	}
	else if(gameManager_->GetTutorialState() == GameManager::TutorialState::FlontBack) {
		tutorialSprite_.SetTexture(tutorialTurnTexture_);
	}
	else {
		tutorialSprite_.isActive = false;
	}

	if (gameManager_->GetState() == GameManager::GameState::Result) {
		resultSprite_->isActive = true;
		if (gameManager_->GetIsClear()){
			resultSprite_->SetTexture(gameClearTexture_);
		}
		else {
			resultSprite_->SetTexture(gameOverTexture_);
		}

	}
	else {
		resultSprite_->isActive = false;
	}

	if (!playerManager_->GetPlayer()->GetIsDamaged()){
		GameManager::GetInstance()->ResetCombo();
	}

	NetworkManager::GetInstance().GetEnemyDeadFlug(isClientEnemyDead_);
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
		EnemyAttackTurnController::GetInstance().Update();
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
	else if (enemy_->GetIsDead() and isClientEnemyDead_){
		gameManager_->SetGameEnd(true);
		gameManager_->SetIsClear(true);
	}
	// トランスフォーム更新

	planeTransform_->translate = translate_;
	planeTransform_->scale = scale_;
	planeTransform_->rotateQuaternion = MLEngine::Math::ConvertFromEuler(rotate_);
	planeTransform_->UpdateMatrix();

	titleSprite_->position = titlePos_;
	titleSprite_->size = titleScale_;
	
	scoreBordSprite_->position = scoreBordPos_;
	scoreBordSprite_->size = scoreBordScale_;

	
	tutorialTransform_->translate = tutorialPos_;
	tutorialTransform_->rotateQuaternion = MLEngine::Math::ConvertFromEuler(tutorialRotate_);
	tutorialTransform_->scale = tutorialScale_;
	tutorialTransform_->UpdateMatrix();
	tutorialSprite_.UpdateAnimation();

	resultSprite_->position = resultPos_;
	resultSprite_->size = resultScale_;

	camera_.Update();

	gameManager_->SceneUpdate();


#ifdef CLIENT_BUILD
	// Client専用処理
	EnemyFlugPacket enemyPacket{};
	enemyPacket.header.type = 4;
	enemyPacket.header.size = sizeof(EnemyFlugPacket);
	enemyPacket.isEnemyDead = enemy_->GetIsDead();

	NetworkManager::GetInstance().Send(enemyPacket);

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
	if (input_->GetKeyboard()->Push(DIK_LCONTROL) and input_->GetKeyboard()->Trigger(DIK_0)){
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

	planeTransform_->Debug();

	ImGui::Begin("テクスチャ");
	ImGui::Text("床");
	ImGui::DragFloat3("床の座標", &translate_.x, 0.01f);
	ImGui::DragFloat3("床の回転", &rotate_.x, 0.01f);
	ImGui::DragFloat3("床の大きさ", &scale_.x, 0.01f);

	ImGui::Text("タイトル");
	ImGui::DragFloat2("タイトル座標", &titlePos_.x, 1.0f);
	ImGui::DragFloat2("タイトル大きさ", &titleScale_.x, 1.0f);

	ImGui::Text("チュートリアル");
	ImGui::DragFloat3("チュートリアル座標", &tutorialPos_.x, 0.1f);
	ImGui::DragFloat3("チュートリアル回転", &tutorialRotate_.x, 0.01f);
	ImGui::DragFloat3("チュートリアル大きさ", &tutorialScale_.x, 0.1f);

	ImGui::DragFloat3("チュートリアル2座標", &tutorialSprite_.transform.translate.x, 0.1f);
	ImGui::DragFloat3("チュートリアル2回転", &tutorialSprite_.transform.rotate.x, 0.01f);
	ImGui::DragFloat3("チュートリアル2大きさ", &tutorialSprite_.transform.scale.x, 0.1f);
	
	ImGui::End();

	

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
