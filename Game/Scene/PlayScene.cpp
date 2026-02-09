#include "PlayScene.h"
#include"Externals/imgui/imgui.h"
#include "DebugScene.h"
#include "FrameTracker.h"
#include "Rand.h"

using namespace MLEngine::Math;

using namespace MLEngine::Resource;
using namespace MLEngine::Core::Render::PostEffect;
using namespace MLEngine::Core;

PlayScene::PlayScene(){
	input_ = MLEngine::Input::Manager::GetInstance();
	gameManager_ = GameManager::GetInstance();
	config_ = GameConfig::GetInstance();
	config_->Initialize();
	postEffect_ = PostEffectDrawer::GetInstance();
}

PlayScene::~PlayScene(){

}

void PlayScene::GlobalSetValue(){
	GlobalVariables* global = GlobalVariables::GetInstance();

	global->SetValue("TextureState", "TitlePos", titlePos_);
	global->SetValue("TextureState", "TitleScale", titleScale_);

	global->SetValue("TextureState", "TutorialPos3D", tutorialPos_);
	global->SetValue("TextureState", "TutorialScale3D", tutorialScale_);
	global->SetValue("TextureState", "ResultPos", resultPos_);
	global->SetValue("TextureState", "ResultScale", resultScale_);
}

void PlayScene::GlobalGetValue(){
	GlobalVariables* global = GlobalVariables::GetInstance();

	titlePos_ = global->GetVector2Value("TextureState", "TitlePos");
	titleScale_ = global->GetVector2Value("TextureState", "TitleScale");

	tutorialPos_ = global->GetVector3Value("TextureState", "TutorialPos3D");
	tutorialScale_ = global->GetVector3Value("TextureState", "TutorialScale3D");
	resultPos_ = global->GetVector2Value("TextureState", "ResultPos");
	resultScale_ = global->GetVector2Value("TextureState", "ResultScale");

}

inline void PlayScene::Initialize(){
	
	dLight_.cbData->normalDirection = { 0.0f,-1.0f,0.0f };
	dLight_.cbData->normalDirection = Normalize(dLight_.cbData->normalDirection);
	dLight_.cbData->intensity = 1.0f;

	GlobalSetValue();

	gameManager_->Initialize();

	camera_.Initialize();
	camera_.position_ = { 0.0f,0.0f,-10.0f };

	playerManager_ = std::make_unique<PlayerManager>();
	playerManager_->Initialize();

	EnemyAttackTurnController::GetInstance().Initialize();
	EnemyStateController::GetInstance().Initialize();

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


	scoreUI_ = std::make_unique<ScoreNumber>();
	scoreUI_->Initialize();


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
	lanePlane_.uvLoopScale_.y = 10.0f;
	lanePlane_.transform.SetParent(planeTransform_.get());

	//必須となる情報の読み込み
	titleTexture_.Load("./Resources/Texture/title_logo.png");
	tutorialMoveTexture_ = ("./Resources/Texture/tutorial_ui_move.png");
	tutorialTurnTexture_ = ("./Resources/Texture/tutorial_ui_turn.png");
	gameClearTexture_.Load("./Resources/Texture/gameClear.png");
	gameOverTexture_.Load("./Resources/Texture/gameOver.png");

	titleSprite_.reset(MLEngine::Resource::Sprite2D::Create(titleTexture_, titlePos_, titleColor_));
	titleSprite_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

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

	skydome_.Initialize("./Resources/model/skydome/skydome.obj");
	skydomeTransform_.scale = { 1000.0f,1000.0f,1000.0f };
	skydomeTransform_.UpdateMatrix();
	skydome_.SetWorldMatrix(skydomeTransform_.worldMatrix);

	for (int32_t i = 0; i < kMaxStone_; i++) {

		stoneLeft_[i].Initialize("./Resources/model/stageStone/stage_stone.obj");
		stoneLeft_[i].materialData.enableLighting = true;
		stoneRight_[i].Initialize("./Resources/model/stageStone/stage_stone.obj");
		stoneRight_[i].materialData.enableLighting = true;

		stoneLeftTF_[i].translate = { -8.0f,-4.0f + 6.5f * i,0.0f + 70.0f * i };
		stoneLeftTF_[i].scale = { 1.0f,3.0f,3.0f };
		stoneLeftTF_[i].rotate = { -0.09f,0.0f,0.0f };
		stoneLeftTF_[i].rotateQuaternion = ConvertFromEuler(stoneLeftTF_[i].rotate);
		stoneRightTF_[i].translate = { 8.0f,-4.0f + 6.5f * i,0.0f + 70.0f * i };
		stoneRightTF_[i].scale = { 1.0f,3.0f,3.0f };
		stoneRightTF_[i].rotate = { -0.09f,0.0f,0.0f };
		stoneRightTF_[i].rotateQuaternion = ConvertFromEuler(stoneRightTF_[i].rotate);

	}

	ingameStartTex_.Load("./Resources/Texture/ingame_UI_start.png");
	ingameGameoverTex_.Load("./Resources/Texture/ingame_UI_gameOver.png");
	ingameFinishTex_.Load("./Resources/Texture/ingame_UI_gameFinish.png");
	resultScoreBackTex_.Load("./Resources/Texture/ingame_UI_scoreBack.png");
	numTex_.Load("./Resources/Texture/number.png");

	ingameStartUI_.Initialize(ingameStartTex_, {});
	ingameStartUI_.startPosition = { 2920.0f, 540.0f };
	ingameStartUI_.middlePosition = { 960.0f, 540.0f };
	ingameStartUI_.endPosition = { -1000.0f, 540.0f };
	ingameStartUI_.easingTime = 4.0f;
	ingameStartUI_.startToMiddleTime = 1.0f;
	ingameStartUI_.stayMiddleTime = 2.0f;
	ingameGameoverUI_.Initialize(ingameGameoverTex_, {});
	ingameGameoverUI_.startPosition = { 960.0f, 540.0f };
	ingameGameoverUI_.middlePosition = { 960.0f, 540.0f };
	ingameGameoverUI_.endPosition = { 960.0f, 540.0f };
	ingameGameoverUI_.startScale = { 0.0f,0.0f };
	ingameGameoverUI_.easingTime = 1.0f;
	ingameGameoverUI_.startToMiddleTime = 0.5f;
	ingameGameoverUI_.stayMiddleTime = 0.1f;
	ingameFinishUI_.Initialize(ingameFinishTex_, {});
	ingameFinishUI_.startPosition = { 2920.0f, 540.0f };
	ingameFinishUI_.middlePosition = { 960.0f, 540.0f };
	ingameFinishUI_.endPosition = { 960.0f, 540.0f };
	ingameFinishUI_.easingTime = 1.0f;
	ingameFinishUI_.startToMiddleTime = 1.0f;
	ingameFinishUI_.stayMiddleTime = 0.0f;
	resultScoreBackUI_.Initialize(resultScoreBackTex_, {});
	resultScoreBackUI_.startPosition = { -1000.0f, 540.0f };
	resultScoreBackUI_.middlePosition = { 520.0f, 540.0f };
	resultScoreBackUI_.endPosition = { 520.0f, 540.0f };
	resultScoreBackUI_.startScale = { 0.5f,0.5f };
	resultScoreBackUI_.middleScale = { 0.5f,0.5f };
	resultScoreBackUI_.endScale = { 0.5f,0.5f };
	resultScoreBackUI_.easingTime = 1.0f;
	resultScoreBackUI_.startToMiddleTime = 1.0f;
	resultScoreBackUI_.stayMiddleTime = 0.0f;

	for (int32_t i = 0; i < 2; i++) {

		resultScoreUIs_[i].Initialize(numTex_, {});
		resultScoreUIs_[i].startPosition = { -1000.0f, 540.0f };
		resultScoreUIs_[i].middlePosition = { 320.0f + i * 200.0f, 540.0f };
		resultScoreUIs_[i].endPosition = { 320.0f + i * 200.0f, 540.0f };
		resultScoreUIs_[i].startScale= { 0.02f,0.2f };
		resultScoreUIs_[i].middleScale = { 0.02f,0.2f };
		resultScoreUIs_[i].endScale = { 0.02f,0.2f };
		resultScoreUIs_[i].GetSprite()->uvScale.x = 0.1f;
		resultScoreUIs_[i].GetSprite()->color = { 0.1f,1.0f,0.1f,1.0f };

		resultScoreUIs_[i].easingTime = 1.0f;
		resultScoreUIs_[i].startToMiddleTime = 1.0f;
		resultScoreUIs_[i].stayMiddleTime = 0.0f;

	}

}

void PlayScene::Finalize(){

}

void PlayScene::Update(){

	DrawImgui();

	config_->Update();

	GlobalGetValue();

	gameManager_->ScoreUpdate();
#ifdef CLIENT_BUILD
	//// Client専用処理

	NetworkManager::SendGameState gameState{};

	NetworkManager::GetInstance().GetGameStatesState(gameState);

	int size = sizeof(gameState);

	//タイトルに戻ったときに初期化できるように
	if (gameManager_->GetState() == GameManager::GameState::Result and static_cast<GameManager::GameState>(gameState.gameState)== GameManager::GameState::Title){
		MLEngine::Scene::Manager::GetInstance()->ChangeScene("Play");
	}

	//タイトルに戻ったときに初期化できるように
	if (gameManager_->GetState() == GameManager::GameState::Playing and static_cast<GameManager::GameState>(gameState.gameState) == GameManager::GameState::Title) {
		MLEngine::Scene::Manager::GetInstance()->ChangeScene("Play");
	}
	
	if (gameManager_->GetScore() != gameState.score){
		gameManager_->SetIsGetScored(true);
	}
	gameManager_->SetState(static_cast<GameManager::GameState>(gameState.gameState));
	gameManager_->SetScore(gameState.score);
	gameManager_->SetCombo(gameState.combo);


	titleSprite_->isActive = false;
	tutorialSprite_.isActive = false;
	resultSprite_->isActive = false;



#else

	//体力が一定以下になったら
	if (playerManager_->GetPlayer()->GetLifeRatio() <= vignetteConfig_.startRatio) {

		//ビネットをかける
		postEffect_->AddApplyEffect(PostEffectType::kVignette);

		float a = (vignetteConfig_.startRatio - playerManager_->GetPlayer()->GetLifeRatio());

		float b = (1.0f / (vignetteConfig_.startRatio - vignetteConfig_.endRatio));

		float t = a * b;

		float powerRange = Lerp(vignetteConfig_.minPowerRange, vignetteConfig_.maxPowerRange, t);

		float power = Lerp(vignetteConfig_.minPower, vignetteConfig_.maxPower, t);

		float scalingTime = Lerp(vignetteConfig_.scalingMaxTime, vignetteConfig_.scalingMinTime, t);

		vignetteConfig_.currentTime += FrameTracker::GetInstance()->GetDeltaTimeF();

		if (vignetteConfig_.currentTime >= scalingTime) {
			vignetteConfig_.currentTime = 0.0f;
		}

		float resultPower = Lerp(power - powerRange, power + powerRange, 1.0f - std::clamp((vignetteConfig_.currentTime / scalingTime), 0.0f, 1.0f));

		//ビネットのパラメータを設定
		if (auto* vignette = dynamic_cast<Vignette*>(postEffect_->GetPostEffects()[PostEffectType::kVignette].get())) {

			vignette->parameter_->color = vignetteConfig_.color;
			vignette->parameter_->power = resultPower;

		}

	}

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

	//リザルト更新
	if (gameManager_->GetState() == GameManager::GameState::Result) {
		
		playerManager_->GetPlayer()->SetIsResultScene(true);

		if (not resultScoreBackUI_.GetIsStartEasing() and not resultScoreBackUI_.GetIsEndEasing()) {

			for (int32_t i = 0; i < 2; i++) {
				resultScoreUIs_[i].Start();
			}

			resultScoreBackUI_.Start();

		}

		for (int32_t i = 0; i < kMaxStone_; i++) {

			stoneLeft_[i].isActive = false;
			stoneRight_[i].isActive = false;

		}

		int score = gameManager_->GetScore();

		for (int32_t i = 0; i < 2; i++) {
			
			resultScoreUIs_[i].SetIsActive(true);

			int num;

			//スコア表示
			if (gameManager_->GetIsEndShuffle()) {

				int devideNum = int(std::powf(10, 1 - i));

				num = score / devideNum;

				resultScoreUIs_[i].GetSprite()->uvTranslate.x = 0.1f * num;

				score = score % devideNum;

			}
			//シャッフル中の数字表示
			else {

				num = RandomInt(0, 9);

				resultScoreUIs_[i].GetSprite()->uvTranslate.x = 0.1f * num;

			}

		}

		resultScoreBackUI_.SetIsActive(true);

	}
	else {

		for (int32_t i = 0; i < kMaxStone_; i++) {

			stoneLeft_[i].isActive = true;
			stoneRight_[i].isActive = true;

		}

		for (int32_t i = 0; i < 2; i++) {
			resultScoreUIs_[i].SetIsActive(false);
		}

		resultScoreBackUI_.SetIsActive(false);

		resultSprite_->isActive = false;
		playerManager_->GetPlayer()->SetIsResultScene(false);
	}

	if (!playerManager_->GetPlayer()->GetIsDamaged()){
		GameManager::GetInstance()->ResetCombo();
	}

	NetworkManager::GetInstance().GetEnemyDeadFlug(isClientEnemyDead_);
#endif	
	
	if (gameManager_->GetState() == GameManager::GameState::Title){
		playerManager_->GetPlayer()->SetIsTitleScene(true);
	}
	else {
		playerManager_->GetPlayer()->SetIsTitleScene(false);
	}

	playerManager_->Update(gameManager_->GetDeltaTime());

	

	if (gameManager_->GetState() == GameManager::GameState::Playing){
		bulletManager_->SetIsModelActive(true);

		ingameStartUI_.SetIsActive(true);
		ingameGameoverUI_.SetIsActive(true);
		ingameFinishUI_.SetIsActive(true);
		scoreUI_->SetIsActive(true);

		//イージングが開始していない場合、開始させる
		if (not ingameStartUI_.GetIsStartEasing() and not ingameStartUI_.GetIsEndEasing()) {
			ingameStartUI_.Start();
		}

		if (gameManager_->GetIsGameOver() and
			not ingameGameoverUI_.GetIsStartEasing() and not ingameGameoverUI_.GetIsEndEasing()) {
			ingameGameoverUI_.Start();
		}

		if (gameManager_->GetIsClear() and
			not ingameFinishUI_.GetIsStartEasing() and not ingameFinishUI_.GetIsEndEasing()) {
			ingameFinishUI_.Start();
		}

		//開始UIが動き終わったら更新。ゲーム終了時は更新を止める
		if (ingameStartUI_.GetIsEndEasing() and not gameManager_->GetIsGameOver() and
			not gameManager_->GetIsClear()) {

			if (!enemy_->GetIsActive()) {
				enemy_->SetIsActive(true);
			}

			EnemyAttackTurnController::GetInstance().Update();
			enemy_->Update();
			bulletManager_->Update();

			lifeUI_->Update();

		}

	}
	else {
		bulletManager_->SetIsModelActive(false);
		enemy_->SetIsActive(false);
		ingameStartUI_.SetIsActive(false);
		ingameGameoverUI_.SetIsActive(false);
		ingameFinishUI_.SetIsActive(false);
		scoreUI_->SetIsActive(false);
	}
	
	if (playerManager_->GetPlayer()->GetIsDead()){
		gameManager_->SetIsGameOver(true);
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

	skydomeTransform_.UpdateMatrix();
	skydome_.SetWorldMatrix(skydomeTransform_.worldMatrix);

	for (int32_t i = 0; i < kMaxStone_; i++) {

		stoneLeftTF_[i].UpdateMatrix();
		stoneRightTF_[i].UpdateMatrix();
		stoneLeft_[i].SetWorldMatrix(stoneLeftTF_[i].worldMatrix);
		stoneRight_[i].SetWorldMatrix(stoneRightTF_[i].worldMatrix);

	}

	titleSprite_->position = titlePos_;
	titleSprite_->size = titleScale_;
	
	tutorialTransform_->translate = tutorialPos_;
	tutorialTransform_->rotateQuaternion = MLEngine::Math::ConvertFromEuler(tutorialRotate_);
	tutorialTransform_->scale = tutorialScale_;
	tutorialTransform_->UpdateMatrix();
	tutorialSprite_.UpdateAnimation();

	resultSprite_->position = resultPos_;
	resultSprite_->size = resultScale_;

	camera_.Update();

	gameManager_->SceneUpdate();

	if (gameManager_->GetIsGetScore()) {
		scoreUI_->ScoreEase();
		if (playerManager_->GetPlayer()->GetIsDamaged()){
			scoreUI_->ComboEase();
		}
	}

	ingameStartUI_.Update();
	ingameGameoverUI_.Update();
	ingameFinishUI_.Update();
	resultScoreBackUI_.Update();
	
	for (int32_t i = 0; i < 2; i++) {
		resultScoreUIs_[i].Update();
	}

	scoreUI_->Update();

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
	NetworkManager::GameStatePacket gamePacket{};
	gamePacket.header.type = 2;
	gamePacket.header.size = sizeof(NetworkManager::GameStatePacket);
	gamePacket.gameState.gameState = gameManager_->GetStateToInt();
	gamePacket.gameState.score = gameManager_->GetScore();
	gamePacket.gameState.combo = gameManager_->GetCombo();

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

	postEffect_->Debug();

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

	ImGui::Begin("縁石");

	for (int32_t i = 0; i < kMaxStone_; i++) {

		std::string leftStr = "左" + std::to_string(i);
		std::string rightStr = "右" + std::to_string(i);

		if (ImGui::TreeNode(leftStr.c_str())) {

			stoneLeftTF_[i].Debug();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode(rightStr.c_str())) {
			stoneRightTF_[i].Debug();
			ImGui::TreePop();
		}

	}

	ImGui::End();

	ImGui::Begin("平行光源");
	dLight_.Debug();
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
