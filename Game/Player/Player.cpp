#include "Player.h"
#include"Externals/imgui/imgui.h"
#include "../Manager/GameManager.h"

using namespace MLEngine::Math;
using namespace MLEngine::Resource;

Player::Player() {
	//必須となる情報の読み込み
	backTextureName_ = ("./Resources/Texture/player_anime_back.png");
	frontTextureName_ = ("./Resources/Texture/player_anime_front.png");
	damageTextureName_ = ("./Resources/Texture/player_anime_damage");
	//必須となる情報の読み込み
	texture_.Load("./Resources/Texture/ingame_player_defense.png");


	sprite3D_.Initialize("./Resources/texture/player_back.png", 7);
	sprite3D_.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	normalScale_ = { 17.5f,2.5f,1.0f };
	resultScale_ = { 35.0f,5.0f,2.0f };;
	sprite3D_.transform.scale = normalScale_;
	sprite3D_.isActive = true;
	sprite3D_.StartAnimation();
	vController_ = &VirtualController::GetInstance();

	//読み込み用に一旦セット
	sprite3D_.SetTexture(frontTextureName_);

	for (int32_t i = 1; i < 8; i++) {

		std::string textureName = damageTextureName_;

		textureName += std::to_string(i);
		textureName += ".png";

		sprite3D_.SetTexture(textureName);

	}

	sprite3D_.SetTexture(backTextureName_);

	input_ = MLEngine::Input::Manager::GetInstance();

	config_ = GameConfig::GetInstance();

	playerMoveSE_.Load("SE/player_move.mp3");
	playerTurnSE_.Load("SE/player_turn.mp3");
	playerBounceSE_.Load("SE/player_bounce.mp3");
	playerDamageSE_.Load("SE/player_damage.mp3");

}

Player::~Player() {

}

void Player::Initialize() {
	GlobalVariables* global = GlobalVariables::GetInstance();

	global->SetValue("PlayerState", "Life", lifeMax_);
	global->SetValue("PlayerState", "comboTime", damageCount_);
	global->SetValue("PlayerState", "recoveryValue", recoveryValue_);
	global->SetValue("PlayerState", "recoverySpeed", recoverySpeed_);
	global->SetValue("PlayerState", "resultPosition", resultPosition_);

	//ボードの調整
	global->SetValue("UIState", "RefrectPos", laneDistanceRefrect_);
	global->SetValue("UIState", "RefrectSize", refrectSize_);


	nowLine_ = config_->centerLane_;
	time_ = 0.0f;
	recoverySpeed_ = 1.0f;
	life_ = (int)lifeMax_;
	pos_ = Vector3(640.0f, -3.0f, -2.0f);
	color_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	isDead_ = false;
	bulletDamege_ = 10;

	refrectTex_.Initialize(texture_, {}, { 1.0f,1.0f,1.0f,1.0f });

#pragma region
	joyconInput = std::make_unique<JoyconManager>();
	joyconInput->Init();
#pragma endregion ジョイコン

#ifdef _SERVER
	// Client専用処理
#pragma region
	DistanceSensor_ = std::make_unique<DistanceSensor>();
	DistanceSensor_->Init();
#pragma endregion 距離センサー
#endif

}

void Player::Finalize() {

}

void Player::Update(const float deltaTime) {
	GlobalVariables* global = GlobalVariables::GetInstance();
	GameManager* gameManager = GameManager::GetInstance();

	deltaTime;
	SyncFromNetwork();

	ResetEvents();

	lifeMax_ = global->GetIntValue("PlayerState", "Life");
	damageCount_ = global->GetFloatValue("PlayerState", "comboTime");
	recoveryValue_ = global->GetIntValue("PlayerState", "recoveryValue");
	recoverySpeed_ = global->GetFloatValue("PlayerState", "recoverySpeed");
	resultPosition_ = global->GetVector3Value("PlayerState", "resultPosition");

	laneDistanceRefrect_ = global->GetFloatValue("UIState", "RefrectPos");
	refrectSize_ = global->GetVector2Value("UIState", "RefrectSize");

#ifdef _DEBUG
	DebugDraw();

#endif // _DEBUG

#pragma region
	joyconInput->Update();

#pragma endregion Joycon

#ifdef CLIENT_BUILD
	// Client専用処理

	if (isResultScene_) {
		sprite3D_.isActive = false;
	}
	else {
		sprite3D_.isActive = true;
	}

#else
	// Server Debug処理
	TimeProcess(deltaTime);
	PlayerRecovery();
	PlayerMove();
#endif


	PlayerInfoInsertion();
	//managerを介してクライアントに送る
	NetworkManager::PlayerStatePacket plPacket{};
	plPacket.header.type = 1;
	plPacket.header.size = sizeof(NetworkManager::PlayerStatePacket);
	plPacket.state = plState_;
	NetworkManager::GetInstance().Send(plPacket);

	if (isResultScene_) {

		//ダメージを受けていた場合
		if (isDamaged_) {
			//被弾状態を強制解除
			isDamaged_ = false;
			damageTime_ = 0.0f;
			damageBlinkingCount_ = 0.0f;
		}

		sprite3D_.transform.translate = resultPosition_;
		sprite3D_.transform.scale = resultScale_;

		//ノーダメージ(スコア0)のとき
		if (gameManager->GetScore() <= 0) {
			sprite3D_.SetTexture(frontTextureName_);
		}
		//スコアが1以上の時
		else {

			std::string textureName = damageTextureName_;

			textureName += std::to_string(gameManager->GetScoreLevel());
			textureName += ".png";

			sprite3D_.SetTexture(textureName);

		}

	}
	else {

		pos_.x = LaneSpecificCalculation();

		sprite3D_.transform.translate = pos_;
		sprite3D_.transform.scale = normalScale_;

	if (isForward_) {
		sprite3D_.SetTexture(backTextureName_);
	}
	else {
		sprite3D_.SetTexture(frontTextureName_);
	}
		if (isForward_) {
			sprite3D_.SetTexture(backTextureName_);
		}
		else {

			//ノーダメージ(スコア0)のとき
			if (gameManager->GetScore() <= 0) {
				sprite3D_.SetTexture(frontTextureName_);
			}
			//スコアが1以上の時
			else {

				std::string textureName = damageTextureName_;

				textureName += std::to_string(gameManager->GetScoreLevel());
				textureName += ".png";

				sprite3D_.SetTexture(textureName);

			}

		}

	}

	if (isJustRefrected_) isJustRefrected_ = false;


	sprite3D_.UpdateAnimation();

	if (isRefrect_){
		refrectCount_ += deltaTime;
	}

	refrectPos_.x = LaneSpecificCalculationRefrect();
	refrectPos_.y = refrectPosY_;

	refrectTex_.startPosition = refrectPos_;
	refrectTex_.middlePosition = refrectPos_;
	refrectTex_.endPosition = refrectPos_;
	refrectTex_.startScale = Vector2();
	refrectTex_.middleScale = refrectSize_ * 1.2f;
	if (isRefrect_){
		refrectTex_.endScale = refrectSize_;
	}
	else {
		refrectTex_.endScale = Vector2();

	}

	refrectTex_.easingTime = 0.3f;
	refrectTex_.startToMiddleTime = 0.15f;
	refrectTex_.stayMiddleTime = 0.0f;
	refrectTex_.Update();

	if (refrectCount_ >= refrectTimer_) {
		isRefrect_ = false;
		refrectCount_ = 0.0f;
	}
	if (life_ <= 0) {
		isDead_ = true;
	}

	//最大値が0でない場合
	if (lifeMax_ != 0.0f) {
		lifeRatio_ = float(life_) / float(lifeMax_);
	}

}

void Player::Draw() {
	/*sprite_->Draw();*/
}

void Player::DebugDraw() {
#ifdef _DEBUG
	ImGui::Begin("プレイヤー");
	ImGui::DragFloat3("座標", &pos_.x, 1.0f);
	ImGui::Text("今のレーン	%d", plState_.nowLine);
	ImGui::Text("今の体力	%d", plState_.life);
	ImGui::Text("傷コンボ	%d", plState_.isDamagedFlug);
	sprite3D_.Debug();
	if (ImGui::Button("体力を減らす")) {

		OnCollision(5);
		//GameManager::GetInstance()->AddScore(plState_.isDamagedFlug);
	}
	ImGui::End();

	ImGui::Begin("反射板");
	ImGui::DragInt("反射板のy座標", &refrectPosY_);
	if (ImGui::Button("イージング開始")){
		Refrect();
	}

	ImGui::End();
#endif // _DEBUG

}

void Player::Refrect(){
	isRefrect_ = true;
	refrectCount_ = 0.0f;
	refrectTex_.ReStart();
	isJustRefrected_ = true;
	plState_.isRefrected = isJustRefrected_;

	NetworkManager::PlayerStatePacket plPacket{};
	plPacket.header.type = 1;
	plPacket.header.size = sizeof(NetworkManager::PlayerStatePacket);
	plPacket.state = plState_;
	NetworkManager::GetInstance().Send(plPacket);
}

void Player::OnCollision(const int damege) {
#ifdef CLIENT_BUILD
	// Client専用処理
	plState_.isClientHited = true;
	NetworkManager::PlayerStatePacket plPacket{};
	plPacket.header.type = 1;
	plPacket.header.size = sizeof(NetworkManager::PlayerStatePacket);
	plPacket.state = plState_;
	NetworkManager::GetInstance().Send(plPacket);
#else
	// Server Debug処理
	life_ -= damege;
	GameManager::GetInstance()->AddScore(isDamaged_);
#endif
	damageTime_ = 0.0f;
	bulletDamege_ = damege;

	isDamaged_ = true;
	playerDamageSE_.Play(Audio::SEVolume);

}
void Player::PlayerMove(){
	if (isTitleScene_ or isResultScene_){
		return;
	}

	//左入力
	if (vController_->LeftTriger()) {
		if (nowLine_ > 0) {
			nowLine_--;
			standTime_ = 0;
			isJustMoved_ = true;
			playerMoveSE_.Play(Audio::SEVolume);
		}

	}
	//右入力
	if (vController_->RightTriger()) {
		if (nowLine_ < config_->maxLane_ - 1) {
			nowLine_++;
			standTime_ = 0;
			isJustMoved_ = true;
			playerMoveSE_.Play(Audio::SEVolume);
		}
	}
#ifdef _SERVER
	// Client専用処理


	int movenum = DistanceSensor_->CheckPosition();
	//#ifndef CLIENT_BUILD
	if (premovenum != movenum) {
		playerMoveSE_.Play(Audio::SEVolume);
		standTime_ = 0;
		if (movenum == position::pLEFT) {
			nowLine_ = movenum;
			isJustMoved_ = true;
		}
		else if (movenum == position::pRIGHT) {
			nowLine_ = movenum;
			isJustMoved_ = true;
		}
		else if (movenum == position::pMID) {
			nowLine_ = movenum;
			isJustMoved_ = true;
		}
	}
	premovenum = movenum;
#endif

	//#endif
	int number = 0;

	if (input_->GetKeyboard()->Trigger(DIK_1)) {
		number = 0;
		if (nowLine_ != number) {
			nowLine_ = number;
			standTime_ = 0;
			isJustMoved_ = true;
			playerMoveSE_.Play(Audio::SEVolume);
		}
	}
	else if (input_->GetKeyboard()->Trigger(DIK_2)) {
		number = 1;
		if (nowLine_ != number) {
			nowLine_ = number;
			standTime_ = 0;
			isJustMoved_ = true;
			playerMoveSE_.Play(Audio::SEVolume);
		}
	}
	else if (input_->GetKeyboard()->Trigger(DIK_3)) {
		number = 2;
		if (nowLine_ != number) {
			nowLine_ = number;
			standTime_ = 0;
			isJustMoved_ = true;
			playerMoveSE_.Play(Audio::SEVolume);
		}
	}

	//タイトルシーンでなければ反転入力
	if (vController_->Decide()) {
		isForward_ = !isForward_;
		isJustTurned_ = true;
		playerTurnSE_.Play(Audio::SEVolume);
	}

	direction dir = joyconInput->CheakRadius();
	//#ifndef CLIENT_BUILD
	if (predir != dir) {
		if (dir == direction::front) {
			isForward_ = true;
			isJustTurned_ = true;
		}
		else if (dir == direction::back) {
			isForward_ = false;
			isJustTurned_ = true;
		}
#ifdef _DEBUG
		else if (dir == direction::no) {
				ImGui::Begin("Joycon");
				ImGui::Text("NOconectJoycon");
				ImGui::End();
			}
#endif	
	}

	predir = dir;
	//#endif


}

void Player::TimeProcess(const float deltaTime){
	standTime_ += deltaTime;

	//回復のタイマー
	if (lifeMax_ <= life_) {
		isLifeMax_ = true;
		time_ = 0.0f;
	}
	else {
		isLifeMax_ = false;
	}

	if (!isLifeMax_) {
		time_ += deltaTime;
	}
	//被弾のタイマー
	if (isDamaged_) {
		damageTime_ += deltaTime;
		damageBlinkingCount_ += deltaTime;
	}

	//被弾時の点滅
	if (damageBlinkingCount_ >= damageBlinkingTime_) {
		damageBlinkingCount_ = 0.0f;
		damageBlinkingTime_ = 0.5f * ((damageCount_ - damageTime_) / damageCount_) + 0.1f;
		sprite3D_.color = { 10.0f,10.0f,10.0f,1.0f };
	}
	else {
		sprite3D_.color = { 1.0f,1.0f,1.0f,1.0f };
	}
	//被弾後のコンボ受付時間を超えたら
	if (damageTime_ >= damageCount_) {
		isDamaged_ = false;
		damageTime_ = 0.0f;
		damageBlinkingCount_ = 0.0f;
	}
}

float Player::LaneSpecificCalculation() {
	float result = 0;
	//レーンの差
	int laneDis = 0;
	//中心のレーンからの差を求める
	laneDis = config_->centerLane_ - nowLine_;
	result = (float)(config_->centerPos_ - (config_->laneDistancePlayer_ * laneDis));

	return result;
}

float Player::LaneSpecificCalculationRefrect(){
	float result = 0;
	//レーンの差
	int laneDis = 0;
	//中心のレーンからの差を求める
	laneDis = config_->centerLane_ - nowLine_;
	result = (float)(960.0f - (laneDistanceRefrect_ * laneDis));

	return result;
}

void Player::PlayerRecovery() {
	//時間以上で回復
	if (time_ >= recoverySpeed_) {
		time_ = 0.0f;
		if (standTime_ >= recoveryDoubleUpCount_){
			life_ += (int)(recoveryValue_ * 2.0f);
		}
		else if (standTime_ >= recoveryUpCount_){
			life_ += (int)(recoveryValue_ * 1.5f);
		}
		else {
			life_ += (int)(recoveryValue_);
		}
	}

	//超過していた場合調整
	if (life_ >= lifeMax_) {
		life_ = (int)lifeMax_;

	}
}

void Player::PlayerInfoInsertion() {
	plState_.isDamagedFlug = isDamaged_;
	plState_.isForwardFlug = isForward_;
	plState_.life = life_;
	plState_.nowLine = nowLine_;
	plState_.isRefrected = isJustRefrected_;

}

void Player::SyncFromNetwork() {
	NetworkManager::SendPlayerState netState{};

	// 最新の受信データを NetworkManager から取得
	if (NetworkManager::GetInstance().GetLatestPlayerState(netState)) {

#ifdef CLIENT_BUILD
		// Client専用処理
		// 受信状態を自プレイヤーに適用
		life_ = netState.life;
		isForward_ = !netState.isForwardFlug;
		isDamaged_ = netState.isDamagedFlug;
		if (netState.nowLine == 0) {
			nowLine_ = 2;
		}
		else if (netState.nowLine == 2) {
			nowLine_ = 0;
		}
		else {
			nowLine_ = netState.nowLine;
		}

		if (netState.isRefrected){
			isRefrect_ = true;
			refrectCount_ = 0.0f;
			refrectTex_.ReStart();
		}

		plState_.isClientHited = netState.isClientHited;

#else
		// Server処理
		// 受信状態を自プレイヤーに適用
		plState_.isClientHited = netState.isClientHited;

		if (plState_.isClientHited) {
			life_ -= bulletDamege_;
			GameManager::GetInstance()->AddScore(isDamaged_);
			isDamaged_ = netState.isClientHited;
			plState_.isClientHited = false;
		}
		
		if (netState.isRefrected) {
			isRefrect_ = true;
			refrectCount_ = 0.0f;
			refrectTex_.ReStart();
		}
#endif


	}
}

void Player::ResetEvents() {
	if (isJustTurned_) isJustTurned_ = false;
	if (isJustMoved_) isJustMoved_ = false;
}

