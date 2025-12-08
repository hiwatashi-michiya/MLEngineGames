#include "Player.h"
#include"Externals/imgui/imgui.h"

using namespace MLEngine::Math;

Player::Player(){
	//必須となる情報の読み込み
	backTexture_.Load("./Resources/Texture/player_back.png");
	frontTexture_.Load("./Resources/Texture/player_front.png");

	sprite_.reset(MLEngine::Resource::Sprite2D::Create(backTexture_, MLEngine::Math::Vector2(pos_.x, pos_.y), color_));
	sprite_->color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vController_ = &VirtualController::GetInstance();

	input_ = MLEngine::Input::Manager::GetInstance();

	config_ = GameConfig::GetInstance();

}

Player::~Player(){

}

void Player::Initialize(){
	GlobalVariables* global = GlobalVariables::GetInstance();

	global->SetValue("PlayerState", "Life", lifeMax_);
	global->SetValue("PlayerState", "comboTime", damegeCount_);
	nowLine_ = config_->centerLane_;
	time_ = 0.0f;
	recoverySpeed_ = 1.0f;
	life_ = lifeMax_ ;
	pos_ = Vector3(640.0f, 650.0f, 0.0f);
	color_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	isDead_ = false;
	bulletDamege_ = 10;
}

void Player::Finalize(){

}

void Player::Update(const float deltaTime){
	GlobalVariables* global = GlobalVariables::GetInstance();

	deltaTime;
	SyncFromNetwork();

	ResetEvents();

	lifeMax_ = global->GetIntValue("PlayerState", "Life");
	damegeCount_ = global->GetFloatValue("PlayerState", "comboTime");
#ifdef _DEBUG
	DebugDraw();

#endif // _DEBUG

	
	
#ifdef CLIENT_BUILD
	// Client専用処理
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

	pos_.x = LaneSpecificCalculation();

	sprite_->position = Vector2(pos_.x, pos_.y);
	sprite_->size = Vector2(128.0f, 128.0f);

	if (isForward_){
		sprite_->SetTexture(backTexture_);
	}
	else {
		sprite_->SetTexture(frontTexture_);
	}

	if (life_ <= 0){
		isDead_ = true;
	}
}

void Player::Draw(){
	/*sprite_->Draw();*/
}

void Player::DebugDraw(){
#ifdef _DEBUG
	ImGui::Begin("プレイヤー");
	ImGui::DragFloat2("座標", &pos_.x, 1.0f);
	ImGui::Text("今のレーン	%d", plState_.nowLine);
	ImGui::Text("今の体力	%d", plState_.life);
	ImGui::Text("傷コンボ	%d", plState_.isDamagedFlug);
	if (ImGui::Button("体力を減らす")){
		OnCollision(20);
	}
	ImGui::End();
#endif // _DEBUG

}

void Player::OnCollision(const int damege){
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
#endif


	
	isDamaged_ = true;
}

void Player::PlayerMove(){
	if (isTitleScene_){
		return;
	}

	//左入力
	if (vController_->LeftTriger()) {
		if (nowLine_ > 0){
			nowLine_--;
			isJustMoved_ = true;
		}
		
	}
	//右入力
	if (vController_->RightTriger()) {
		if (nowLine_ < config_->maxLane_ - 1) {
			nowLine_++;
			isJustMoved_ = true;
		}
	}

	int number = 0;

	if (input_->GetKeyboard()->Trigger(DIK_1)){
		number = 0;
		if (nowLine_ != number) {
			nowLine_ = number;
			isJustMoved_ = true;
		}
	}
	else if (input_->GetKeyboard()->Trigger(DIK_2)) {
		number = 1;
		if (nowLine_ != number) {
			nowLine_ = number;
			isJustMoved_ = true;
		}
	}
	else if (input_->GetKeyboard()->Trigger(DIK_3)) {
		number = 2;
		if (nowLine_ != number) {
			nowLine_ = number;
			isJustMoved_ = true;
		}
	}
	

	//タイトルシーンでなければ反転入力
	if (vController_->Decide()) {
		isForward_ = !isForward_;
		isJustTurned_ = true;
	}

	

}

void Player::TimeProcess(const float deltaTime){
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
	}
	if (damageTime_ >= damegeCount_) {
		isDamaged_ = false;
		damageTime_ = 0.0f;
	}
}

float Player::LaneSpecificCalculation(){
	float result = 0;
	//レーンの差
	int laneDis = 0;
	//中心のレーンからの差を求める
	laneDis = config_->centerLane_ - nowLine_;
	result = (float)(config_->centerPos_ - (config_->laneDistancePlayer_ * laneDis));

	return result;
}

void Player::PlayerRecovery(){
	//時間以上で回復
	if (time_ >= recoverySpeed_){
		time_ = 0.0f;
		life_ += recoveryValue_;
	}
	
	//超過していた場合調整
	if (life_ >= lifeMax_){
		life_ = lifeMax_;
		
	}
}

void Player::PlayerInfoInsertion(){
	plState_.isDamagedFlug = isDamaged_;
	plState_.isForwardFlug = isForward_;
	plState_.life = life_;
	plState_.nowLine = nowLine_;


	
	//if (not isForward_) {
	//	//後ろを向いているなら青色
	//	sprite_->color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	//}
	//else {
	//	//前を向いているなら赤色
	//	sprite_->color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	//}
}

void Player::SyncFromNetwork(){
	NetworkManager::SendPlayerState netState{};

	// 最新の受信データを NetworkManager から取得
	if (NetworkManager::GetInstance().GetLatestPlayerState(netState)){

#ifdef CLIENT_BUILD
		// Client専用処理
		// 受信状態を自プレイヤーに適用
		life_ = netState.life;
		isForward_ = !netState.isForwardFlug;
		isDamaged_ = netState.isDamagedFlug;
		if (netState.nowLine == 0){
			nowLine_ = 2;
		}
		else if (netState.nowLine == 2){
			nowLine_ = 0;
		}
		else {
			nowLine_ = netState.nowLine;
		}
		
		plState_.isClientHited = netState.isClientHited;
		
#else
		// Server処理
		// 受信状態を自プレイヤーに適用
		plState_.isClientHited = netState.isClientHited;

		if (plState_.isClientHited){
			life_ -= bulletDamege_;
			plState_.isClientHited = false;
		}
		isDamaged_ = netState.isDamagedFlug;

#endif

		
	}
}

void Player::ResetEvents(){
	if (isJustTurned_) isJustTurned_ = false;
	if (isJustMoved_) isJustMoved_ = false;
}

