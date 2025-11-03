#include "Player.h"
#include"Externals/imgui/imgui.h"

using namespace MLEngine::Math;

Player::Player(){
	//必須となる情報の読み込み
	texture_.Load("./Resources/white.png");

	sprite_.reset(MLEngine::Resource::Sprite::Create(texture_, MLEngine::Math::Vector2(pos_.x, pos_.y), color_));

	input_ = MLEngine::Input::Manager::GetInstance();
	config_ = GameConfig::GetInstance();

}

Player::~Player(){

}

void Player::Initialize(){
	nowLine_ = config_->centerLane_;
	time_ = 0.0f;
	recoverySpeed_ = 1.0f;
	life_ = lifeMax_ ;
	pos_ = Vector3(640.0f, 650.0f, 0.0f);
	color_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
}

void Player::Finalize(){

}

void Player::Update(const float deltaTime){
	DebugDraw();
	TimeProcess(deltaTime);
	
	PlayerRecovery();
	PlayerMove();

	pos_.x = LaneSpecificCalculation();

	sprite_->position_ = Vector2(pos_.x, pos_.y);
	sprite_->size_ = Vector2(128.0f, 128.0f);
}

void Player::Draw(){
	sprite_->Draw();
}

void Player::DebugDraw(){
#ifdef _DEBUG
	ImGui::Begin("プレイヤー");
	ImGui::DragFloat2("座標", &pos_.x, 1.0f);
	ImGui::Text("今のレーン	%d", nowLine_);
	ImGui::Text("今の体力	%d", life_);
	ImGui::Text("傷コンボ	%d", isDamaged_);
	if (ImGui::Button("体力を減らす")){
		isDamaged_ = true;
		life_ -= 20;
	}
	ImGui::End();
#endif // _DEBUG

}

void Player::OnCollision(const int damege){
	life_ -= damege;
	isDamaged_ = true;
}

void Player::PlayerMove(){
	//左入力
	if (input_->GetKeyboard()->Trigger(DIK_A) || input_->GetKeyboard()->Trigger(DIK_LEFT)) {
		if (nowLine_ > 0){
			nowLine_--;
		}
		
	}
	//右入力
	if (input_->GetKeyboard()->Trigger(DIK_D) || input_->GetKeyboard()->Trigger(DIK_RIGHT)) {
		if (nowLine_ < config_->maxLane_ - 1) {
			nowLine_++;
		}
	}

	//反転入力
	if (input_->GetKeyboard()->Trigger(DIK_SPACE) || input_->GetKeyboard()->Trigger(DIK_RETURN)) {
		isforward_ = !isforward_;
	}

	if (not isforward_){
		//後ろを向いているなら青色
 		sprite_->color_ = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
	}
	else {
		//前を向いているなら赤色
		sprite_->color_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
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
